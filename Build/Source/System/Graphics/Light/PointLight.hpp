#pragma once
#include "../../Math/Algebra/Vector3.hpp"
#include "../DataType/Color.hpp"

namespace Engine5
{
    class PointLight
    {
    public:
        PointLight();
        ~PointLight();

        void Set();
    private:
        Color   m_ambient;
        Color   m_diffuse;
        Color   m_specular;
        Vector3 m_position;
        Real    m_range = 1.0f;
        Vector3 m_attenuation;
    };
}
