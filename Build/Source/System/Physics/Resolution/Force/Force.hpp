#pragma once
#include "../../../Math/Utility/MathDef.hpp"

namespace Json
{
    class Value;
}

namespace Engine5
{
    class RigidBody;
    class Force;

    class ForceFactory
    {
    public:
        ForceFactory()
        {
        }

        virtual ~ForceFactory()
        {
        }

        virtual Force* Create() = 0;

    protected:
        friend class Resolution;
        std::string type;
    };

    class Force
    {
    public:
        Force()
        {
        }

        virtual ~Force()
        {
        }

        virtual void Initialize() = 0;
        virtual void Update(RigidBody* body, Real dt) = 0;
        virtual void Shutdown() = 0;
        virtual void Load(const Json::Value& data) = 0;
        virtual void Save(const Json::Value& data) = 0;

    protected:
        std::string type;
    };
}
