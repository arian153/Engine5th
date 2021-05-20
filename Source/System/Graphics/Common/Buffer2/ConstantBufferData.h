#pragma once

#include "../../../Math/Algebra/Matrix44.hpp"
#include "../../../Math/Algebra/Vector3.hpp"
#include "../../../Math/Algebra/Vector4.hpp"
#include "../../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    struct ColorBufferData
    {
        Vector4 color;
    };

    struct CameraBufferData
    {
        Vector3 camera_position;
        Real    padding;
    };

    struct GammaBufferData
    {
        Real    gamma_correction;
        Vector3 padding;
    };

    struct LightBufferData
    {
        Vector4 ambient_color;
        Vector4 diffuse_color;
        Vector4 specular_color;
        Real    specular_power;
        Vector3 light_direction;
    };

    struct MatrixBufferData
    {
        Matrix44 model;
        Matrix44 view;
        Matrix44 proj;
    };
}
