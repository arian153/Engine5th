#pragma once
#include "../../Math/Algebra/Vector3.hpp"
#include "../DataType/Color.hpp"

namespace Engine5
{
    class DirectionalLight
    {
    public:
        DirectionalLight();
        ~DirectionalLight();

    private:
        Color   m_ambient;
        Color   m_diffuse;
        Color   m_specular;
        Vector3 m_direction;
    };
}
