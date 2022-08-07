#include "ManualController.h"

#include "Pointer.h"


ManualController::ManualController()
    :
    m_Rotation(0.0f, 0.0f, 0.0f),
    m_Translation(0.0f, 0.0f, 0.0f)
{
}

void ManualController::Init()
{
    const DirectX::XMFLOAT4X4& transformation = GetTransformation();

    m_Rotation = { 0.0f, 0.0f, 0.0f };
    m_Translation.x = transformation(3, 0);
    m_Translation.y = transformation(3, 1);
    m_Translation.z = transformation(3, 2);
}

const DirectX::XMFLOAT3& ManualController::GetRotation() const
{
    return m_Rotation;
}

const DirectX::XMFLOAT3& ManualController::GetTranslation() const
{
    return m_Translation;
}

void ManualController::Rotate(float dx, float dy, float dz)
{
    m_Rotation.x = WrapAngle(m_Rotation.x + dx);
    m_Rotation.y = WrapAngle(m_Rotation.y + dy);
    m_Rotation.z = WrapAngle(m_Rotation.z + dz);
}

void ManualController::Translate(float dx, float dy, float dz)
{
    DirectX::XMFLOAT3 offsets = { dx, dy, dz };
    DirectX::XMStoreFloat3(
        &m_Translation,
        DirectX::XMVector3Transform(
            DirectX::XMLoadFloat3(&offsets),
            DirectX::XMLoadFloat4x4(&GetTransformation())
        )
    );
}

void ManualController::UpdateTransformation()
{
    DirectX::XMStoreFloat4x4(
        &GetTransformation(),
        DirectX::XMMatrixRotationRollPitchYaw(
            DirectX::XMConvertToRadians(m_Rotation.x),
            DirectX::XMConvertToRadians(m_Rotation.y),
            DirectX::XMConvertToRadians(m_Rotation.z)
        ) *
        DirectX::XMMatrixTranslation(
            m_Translation.x,
            m_Translation.y,
            m_Translation.z
        )
    );
}

DirectX::XMFLOAT4X4& ManualController::GetTransformation() const
{
    const int32_t index = Utility::Pointer(0x013FC8E0).Field(0x6F5D38).As<int32_t>();
    return Utility::Pointer(0x013FC8E0).Field(0x711350 + index * 0x1A0).As<DirectX::XMFLOAT4X4>();
}

float ManualController::WrapAngle(float angle) const
{
    if (angle > 360.0f)
    {
        angle -= 360.0f;
    }
    else if (angle < 0.0f)
    {
        angle += 360.0f;
    }

    return angle;
}
