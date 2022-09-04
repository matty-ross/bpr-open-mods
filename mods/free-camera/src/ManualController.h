#pragma once


#include <DirectXMath.h>


class ManualController
{
public:
    ManualController();

    void Init();
    const DirectX::XMFLOAT3& GetRotation() const;
    const DirectX::XMFLOAT3& GetTranslation() const;
    void Rotate(float dx, float dy, float dz);
    void Translate(float dx, float dy, float dz);
    void UpdateTransformation();

private:
    static DirectX::XMFLOAT4X4* GetTransformation();
    static float WrapAngle(float angle);
    static DirectX::XMFLOAT3 GetEulerAnglesFromQuaternion(const DirectX::XMVECTOR& quaternion);

private:
    DirectX::XMFLOAT3 m_Scaling;
    DirectX::XMFLOAT3 m_Rotation;
    DirectX::XMFLOAT3 m_Translation;
};