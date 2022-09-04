#include "ManualController.h"

#include "Pointer.h"


ManualController::ManualController()
    :
    m_Scaling(0.0f, 0.0f, 0.0f),
    m_Rotation(0.0f, 0.0f, 0.0f),
    m_Translation(0.0f, 0.0f, 0.0f)
{
}

void ManualController::Init()
{
    DirectX::XMVECTOR scaling = {};
    DirectX::XMVECTOR rotation = {};
    DirectX::XMVECTOR translation = {};
    DirectX::XMMatrixDecompose(
        &scaling,
        &rotation,
        &translation,
        DirectX::XMLoadFloat4x4(GetTransformation())
    );

    DirectX::XMStoreFloat3(&m_Scaling, scaling);
    m_Rotation = GetEulerAnglesFromQuaternion(rotation);
    DirectX::XMStoreFloat3(&m_Translation, translation);
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
    DirectX::XMFLOAT3 offsets(dx, dy, dz);
    DirectX::XMStoreFloat3(
        //&offsets,
        &m_Translation,
        DirectX::XMVector3Transform(
            DirectX::XMLoadFloat3(&offsets),
            DirectX::XMLoadFloat4x4(GetTransformation())
        )
    );

    /*
    m_Translation.x += offsets.x;
    m_Translation.y += offsets.y;
    m_Translation.z += offsets.z;
    */
}

void ManualController::UpdateTransformation()
{
    DirectX::XMStoreFloat4x4(
        GetTransformation(),
        DirectX::XMMatrixAffineTransformation(
            DirectX::XMLoadFloat3(&m_Scaling),
            DirectX::XMVectorZero(),
            DirectX::XMQuaternionRotationRollPitchYaw(
                DirectX::XMConvertToRadians(m_Rotation.x),
                DirectX::XMConvertToRadians(m_Rotation.y),
                DirectX::XMConvertToRadians(m_Rotation.z)
            ),
            DirectX::XMLoadFloat3(&m_Translation)
        )
    );
}

DirectX::XMFLOAT4X4* ManualController::GetTransformation()
{
    const int32_t index = Utility::Pointer(0x013FC8E0).Field(0x6F5D38).As<int32_t>();
    return Utility::Pointer(0x013FC8E0).Field(0x711350 + index * 0x1A0).GetAddress<DirectX::XMFLOAT4X4*>();
}

float ManualController::WrapAngle(float angle)
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

DirectX::XMFLOAT3 ManualController::GetEulerAnglesFromQuaternion(const DirectX::XMVECTOR& rotation)
{
    DirectX::XMFLOAT4 quaternion = {};
    DirectX::XMStoreFloat4(&quaternion, rotation);

    return
    {
        DirectX::XMConvertToDegrees(asinf(2.0f * (quaternion.w * quaternion.x - quaternion.z * quaternion.y))),
        DirectX::XMConvertToDegrees(atan2f(2.0f * (quaternion.w * quaternion.y + quaternion.x * quaternion.z), 1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.x * quaternion.x))),
        DirectX::XMConvertToDegrees(atan2f(2.0f * (quaternion.w * quaternion.z + quaternion.y * quaternion.x), 1.0f - 2.0f * (quaternion.x * quaternion.x + quaternion.z * quaternion.z)))
    };
}