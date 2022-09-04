#include "Gui.h"

#include "imgui.h"

#include "Pointer.h"


Gui::Gui(SavedParameters& savedParameters, ManualController& manualController)
    :
    m_SavedParameters(savedParameters),
    m_ManualController(manualController)
{
    ImGui::StyleColorsClassic();
}

void Gui::Render(const std::string& title)
{
    if (ImGui::Begin(title.c_str()))
    {
        static constexpr const char* const author = "PISros0724 (Matty)";
        static constexpr int version = 2;
        const ImGuiIO& io = ImGui::GetIO();
        
        ImGui::Text("Mod by %s", author);
        ImGui::Text("Version: %d", version);
        ImGui::Text("Framerate: %.3f", io.Framerate);
        
        RenderMisc();
        RenderBehavior();
        RenderParameters();
        RenderManualController();
    }
    ImGui::End();
}

void Gui::RenderMisc()
{
    if (ImGui::CollapsingHeader("Misc"))
    {
        ImGui::Checkbox("Allow Super / Mega Jump Moment", &Utility::Pointer(0x013FC8E0).Field(0x71919D).As<bool>());
        ImGui::Checkbox("Allow Slow Motion", &Utility::Pointer(0x013FC8E0).Field(0x7175E8).As<bool>());
        ImGui::Checkbox("Disable Depth of Field", &Utility::Pointer(0x013FC8E0).Field(0x7191A8).As<bool>());
    }
}

void Gui::RenderBehavior()
{
    if (ImGui::CollapsingHeader("Behavior"))
    {
        const int32_t index = Utility::Pointer(0x013FC8E0).Field(0x6F5D38).As<int32_t>();
        const Utility::Pointer behavior = Utility::Pointer(0x013FC8E0).Field(0x711340 + index * 0x1A0).Field(0x0);

        ImGui::Checkbox("Disable Collision Clipping", &behavior.At(0x9).As<bool>());
        ImGui::Checkbox("Disable Automatic Rotation", &behavior.At(0xBC3).As<bool>());

        static constexpr ImGuiSliderFlags flagsLinear = ImGuiSliderFlags_AlwaysClamp;
        ImGui::SliderFloat("Pitch", &behavior.At(0x40).As<float>(), -10.0f, 10.0f, "%.2f", flagsLinear);
        ImGui::SliderFloat("Yaw", &behavior.At(0x20).As<float>(), -180.0f, 180.0f, "%.2f", flagsLinear);
    }
}

void Gui::RenderParameters()
{
    if (ImGui::CollapsingHeader("Parameters"))
    {
        static constexpr ImGuiSliderFlags flagsLinear = ImGuiSliderFlags_AlwaysClamp;
        static constexpr ImGuiSliderFlags flagsLogarithmic = ImGuiSliderFlags_AlwaysClamp | ImGuiSliderFlags_Logarithmic | ImGuiSliderFlags_NoRoundToFormat;
        ImGui::SliderFloat("Pitch Spring", &Utility::Pointer(0x013FC8E0).Field(0x716604).As<float>(), 0.0f, 1.0f, "%.2f", flagsLinear);
        ImGui::SliderFloat("Yaw Spring", &Utility::Pointer(0x013FC8E0).Field(0x716608).As<float>(), 0.0f, 1.0f, "%.2f", flagsLinear);
        ImGui::SliderFloat("Pivot Y", &Utility::Pointer(0x013FC8E0).Field(0x716614).As<float>(), 0.1f, 10.0f, "%.2f", flagsLogarithmic);
        ImGui::SliderFloat("Pivot Z", &Utility::Pointer(0x013FC8E0).Field(0x716618).As<float>(), 0.1f, 50.0f, "%.2f", flagsLogarithmic);
        ImGui::SliderFloat("FOV", &Utility::Pointer(0x013FC8E0).Field(0x716634).As<float>(), 1.0f, 179.0f, "%.2f", flagsLinear);
        ImGui::SliderFloat("In Front FOV Max", &Utility::Pointer(0x013FC8E0).Field(0x716638).As<float>(), 1.0f, 179.0f, "%.2f", flagsLinear);
        ImGui::SliderFloat("Front In Amount", &Utility::Pointer(0x013FC8E0).Field(0x71663C).As<float>(), -5.0f, 5.0f, "%.2f", flagsLogarithmic);
        ImGui::SliderFloat("Drift Yaw Spring", &Utility::Pointer(0x013FC8E0).Field(0x716654).As<float>(), 0.0f, 1.0f, "%.2f", flagsLinear);
        ImGui::SliderFloat("Boost FOV Zoom Compensation", &Utility::Pointer(0x013FC8E0).Field(0x716658).As<float>(), -5.0f, 5.0f, "%.2f", flagsLogarithmic);
        ImGui::SliderFloat("Down Angle", &Utility::Pointer(0x013FC8E0).Field(0x71665C).As<float>(), 0.0f, 90.0f, "%.2f", flagsLinear);
        ImGui::SliderFloat("Drop Factor", &Utility::Pointer(0x013FC8E0).Field(0x716670).As<float>(), 0.0f, 1.0f, "%.2f", flagsLinear);
        if (ImGui::Button("Reset Default Parameters"))
        {
            m_SavedParameters.ResetDefaultParameters();
        }

        ImGui::Separator();

        static char name[64] = {};
        ImGui::InputText("Name", name, IM_ARRAYSIZE(name));
        if (ImGui::Button("Save Current Parameters"))
        {
            if (name[0] != '\0')
            {
                m_SavedParameters.AddCurrentParameters(name);
                m_SavedParameters.SaveParameters();
                name[0] = '\0';
            }
        }

        ImGui::Separator();

        static ImGuiTextFilter filter;
        if (ImGui::BeginListBox("Saved Parameters"))
        {
            int id = 0;
            for (const Parameters& parameters : m_SavedParameters.GetParameters())
            {
                if (filter.PassFilter(parameters.Name.c_str()))
                {
                    ImGui::PushID(id);
                    if (ImGui::Selectable(parameters.Name.c_str()))
                    {
                        m_SavedParameters.SetCurrentParameters(parameters);
                    }
                    ImGui::PopID();
                }
                
                ++id;
            }

            ImGui::EndListBox();
        }
        filter.Draw();
        if (ImGui::Button("Reload Saved Parameters"))
        {
            m_SavedParameters.LoadParameters();
        }
    }
}

void Gui::RenderManualController()
{
    if (ImGui::CollapsingHeader("Manual Controller"))
    {
        static bool overrideController = false;
        static bool lockTransformation = false;
        if (ImGui::Checkbox("Override Controller", &overrideController))
        {
            Utility::Pointer(0x013FC8E0).Field(0x716674).As<bool>() = !overrideController;
            lockTransformation = false;
            m_ManualController.Init();
        }
        
        if (overrideController)
        {
            ImGui::Checkbox("Lock Transformation", &lockTransformation);

            static constexpr ImGuiSliderFlags flagsLinear = ImGuiSliderFlags_AlwaysClamp;
            static float rotation[3] = {};
            static float translation[3] = {};
            ImGui::DragFloat3("Rotate", rotation, lockTransformation ? 0.05f : 1.0f, lockTransformation ? -1.0f : -10.f, lockTransformation ? 1.0f : 10.0f, "%.2f", flagsLinear);
            ImGui::DragFloat3("Translate", translation, lockTransformation ? 0.05f : 1.0f, lockTransformation ? -1.0f : -10.f, lockTransformation ? 1.0f : 10.0f, "%.2f", flagsLinear);

            m_ManualController.Rotate(rotation[0], rotation[1], rotation[2]);
            m_ManualController.Translate(translation[0], translation[1], translation[2]);
            m_ManualController.UpdateTransformation();
            if (!lockTransformation)
            {
                rotation[0] = rotation[1] = rotation[2] = 0.0f;
                translation[0] = translation[1] = translation[2] = 0.0f;
            }

            ImGui::Separator();

            ImGui::Text("Rotation");
            ImGui::Text("  x: %.2f", m_ManualController.GetRotation().x);
            ImGui::Text("  y: %.2f", m_ManualController.GetRotation().y);
            ImGui::Text("  z: %.2f", m_ManualController.GetRotation().z);
            ImGui::Text("Translation");
            ImGui::Text("  x: %.2f", m_ManualController.GetTranslation().x);
            ImGui::Text("  y: %.2f", m_ManualController.GetTranslation().y);
            ImGui::Text("  z: %.2f", m_ManualController.GetTranslation().z);
        }
    }
}