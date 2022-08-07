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
    DirectX::XMFLOAT4X4& GetTransformation() const;
    float WrapAngle(float angle) const;

private:
    DirectX::XMFLOAT3 m_Rotation;
    DirectX::XMFLOAT3 m_Translation;
};