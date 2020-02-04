#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include <unordered_map>
#include "MaterialDef.hpp"

namespace Engine5
{
    class FrictionCoefficient
    {
    public:
        explicit FrictionCoefficient(Real static_friction = 0.5f, Real dynamic_friction = 0.4f);
        ~FrictionCoefficient();

    public:
        Real static_friction;
        Real dynamic_friction;
    };

    class FrictionUtility
    {
    public:
        FrictionUtility();
        ~FrictionUtility();

        void                Initialize();
        FrictionCoefficient Find(Physics::eMaterial a, Physics::eMaterial b);

    private:
        std::unordered_map<size_t, FrictionCoefficient> m_friction_map;
    };
}
