#pragma once

namespace Engine5
{
    enum class eLightType
    {
        DirectionalLight,
        PointLight,
        SpotLight,
        CapsuleLight
    };

    enum class eLightingMethod
    {
        None,
        Forward,
        Deferred
    };
}
