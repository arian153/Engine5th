#pragma once

#include "../../../Math/Algebra/Matrix44.hpp"
#include "../../../Math/Algebra/Vector3.hpp"
#include "../../../Math/Algebra/Vector2.hpp"
#include "../../../Math/Utility/MathDef.hpp"
#include "../../DataType/Color.hpp"

namespace Engine5
{
    struct ColorBufferData
    {
        Color color;
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
        Color ambient_color;
        Color ambient_range;
        Color diffuse_color;
        Color specular_color;
        Real    specular_power;
        Vector3 direction;
        Vector3 position;
        Real    range;
        Vector3 attenuation;
        Real    length;
        Real    spot;
        int     type;
        Vector2 padding;
    };

    struct MatrixBufferData
    {
        Matrix44 view;
        Matrix44 proj;
    };

    struct InstanceBufferData
    {
        Matrix44 model;
        Color    color;
    };
}
