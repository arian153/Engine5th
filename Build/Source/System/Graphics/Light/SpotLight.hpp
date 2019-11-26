#pragma once
#include "../../Math/Algebra/Vector3.hpp"
#include "../Utility/Color.hpp"

namespace Engine5
{
    class SpotLight
    {
    public:
        SpotLight();
        ~SpotLight();

    private:

        Color   m_ambient;
        Color   m_diffuse;
        Color   m_specular;
        Vector3 m_position;
        Real    m_range = 1.0f;
        Vector3 m_direction;
        Real    m_spot = 0.0f;
        Vector3 m_attenuation;
    };
}
