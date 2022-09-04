#include "Gui.h"

#include "imgui.h"

#include "AutoActions.h"

#include "Pointer.h"


Gui::Gui(BlacklistedPlayers& blacklistedPlayers)
    :
    m_BlacklistedPlayers(blacklistedPlayers)
{
    ImGui::StyleColorsClassic();
}

void Gui::Render(const std::string& title)
{
    if (ImGui::Begin(title.c_str()))
    {
        static constexpr const char* const author = "PISros0724 (Matty)";
        static constexpr int version = 1;
        const ImGuiIO& io = ImGui::GetIO();

        ImGui::Text("Mod by %s", author);
        ImGui::Text("Version: %d", version);
        ImGui::Text("Framerate: %.3f", io.Framerate);

        RenderMisc();
        RenderPlayers();
        RenderBlacklistedPlayers();
    }
    ImGui::End();
}

void Gui::RenderMisc()
{
    if (ImGui::CollapsingHeader("Misc"))
    {
        ImGui::Checkbox("Enable Bully Repellent", &g_BullyRepellentEnabled);
    }
}

void Gui::RenderPlayers()
{
    if (ImGui::CollapsingHeader("Players"))
    {
        static constexpr int columnsCount = 4;
        static constexpr ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_NoBordersInBodyUntilResize;
        if (ImGui::BeginTable("Players##table", columnsCount, flags))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 175.0f);
            ImGui::TableSetupColumn("Headset Status", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableSetupColumn("Muted", ImGuiTableColumnFlags_WidthFixed, 40.0f);
            ImGui::TableSetupColumn("Actions");
            ImGui::TableHeadersRow();

            if (Utility::Pointer(0x013FC8E0).Field(0x8EFF30).As<bool>())
            {
                const int32_t onlinePlayersCount = Utility::Pointer(0x013FC8E0).Field(0x8F625C).As<int32_t>();
                for (int32_t i = 0; i < onlinePlayersCount; ++i)
                {
                    ImGui::TableNextRow();
                    const Utility::Pointer playerData = Utility::Pointer(0x013FC8E0).Field(0x8F6268 + 0x138 * i);
            
                    ImGui::TableNextColumn();
                    const char* playerName = playerData.At(0xF0).GetAddress<char*>();
                    ImGui::TextUnformatted(playerName);
            
                    ImGui::TableNextColumn();
                    const int32_t headsetStatus = playerData.At(0x11C).As<int32_t>();
                    static constexpr const char* const headsetStatuses[] =
                    {
                        "none",
                        "has headset",
                        "talking",
                        "muted"
                    };
                    ImGui::TextUnformatted(headsetStatuses[headsetStatus]);

                    ImGui::TableNextColumn();
                    ImGui::BeginDisabled();
                    ImGui::PushID(i);
                    ImGui::Checkbox("##muted", &playerData.At(0x130).As<bool>());
                    ImGui::PopID();
                    ImGui::EndDisabled();

                    ImGui::TableNextColumn();
                    if (!playerData.At(0x12D).As<bool>())
                    {
                        const uint64_t playerID = playerData.At(0x110).As<uint64_t>();
                        ImGui::PushID(i);
                        if (ImGui::Button("Autokick"))
                        {
                            Player* const player = m_BlacklistedPlayers.GetPlayer(playerID);
                            if (player == nullptr)
                            {
                                m_BlacklistedPlayers.AddPlayer(
                                    {
                                        .ID = playerID,
                                        .Name = playerName,
                                        .Autokick = true,
                                        .Automute = false
                                    }
                                );
                            }
                            else
                            {
                                player->Name = playerName;
                                player->Autokick = true;
                            }
                            m_BlacklistedPlayers.SavePlayers();
                        }
                        ImGui::PopID();
                        ImGui::SameLine();
                        ImGui::PushID(i);
                        if (ImGui::Button("Automute"))
                        {
                            Player* const player = m_BlacklistedPlayers.GetPlayer(playerID);
                            if (player == nullptr)
                            {
                                m_BlacklistedPlayers.AddPlayer(
                                    {
                                        .ID = playerID,
                                        .Name = playerName,
                                        .Autokick = false,
                                        .Automute = true
                                    }
                                );
                            }
                            else
                            {
                                player->Name = playerName;
                                player->Automute = true;
                            }
                            m_BlacklistedPlayers.SavePlayers();
                        }
                        ImGui::PopID();
                    }
                }
            }

            ImGui::EndTable();
        }
    }
}

void Gui::RenderBlacklistedPlayers()
{
    if (ImGui::CollapsingHeader("Blacklisted Players"))
    {
        static ImGuiTextFilter filter;
        
        static constexpr int columnsCount = 3;
        static constexpr ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_NoBordersInBodyUntilResize;
        if (ImGui::BeginTable("Blacklisted Players##table", columnsCount, flags))
        {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 180.0f);
            ImGui::TableSetupColumn("Autokick");
            ImGui::TableSetupColumn("Automute");
            ImGui::TableHeadersRow();

            int id = 0;
            for (Player& player : m_BlacklistedPlayers.GetPlayers())
            {
                if (filter.PassFilter(player.Name.c_str()))
                {
                    ImGui::TableNextRow();

                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(player.Name.c_str());

                    ImGui::TableNextColumn();
                    ImGui::PushID(id);
                    if (ImGui::Checkbox("##autokick", &player.Autokick))
                    {
                        m_BlacklistedPlayers.SavePlayers();
                    }
                    ImGui::PopID();

                    ImGui::TableNextColumn();
                    ImGui::PushID(id);
                    if (ImGui::Checkbox("##automute", &player.Automute))
                    {
                        m_BlacklistedPlayers.SavePlayers();
                    }
                    ImGui::PopID();
                }

                ++id;
            }
            
            ImGui::EndTable();
        }
        filter.Draw();
        if (ImGui::Button("Reload Blacklisted Players"))
        {
            m_BlacklistedPlayers.LoadPlayers();
        }
    }
}