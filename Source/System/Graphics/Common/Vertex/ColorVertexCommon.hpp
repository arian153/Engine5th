#pragma once
#include "../../GraphicsAPI.hpp"
#include IncludeColorVertexAPI


namespace Engine5
{
    class Vector3;
    class Color;

    class ColorVertexCommon : public ColorVertexAPI
    {
    public:
        ColorVertexCommon();
        ColorVertexCommon(const Vector3& position, const Color& color);

        Vector3 GetPosition() const;
        Color   GetColor() const;

    public:
    };
}
