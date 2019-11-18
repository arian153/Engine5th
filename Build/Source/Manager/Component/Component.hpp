#pragma once
#include "../../System/Math/Utility/MathDef.hpp"
#include "../../System/Core/Utility/CoreDef.hpp"

namespace Engine5
{
    class Component
    {
    public:
        Component();
        virtual ~Component();

        virtual void Initialize() = 0;
        virtual void Update(Real dt) = 0;
        virtual void Shutdown() = 0;

        String Type() const;
        bool   IsLoaded() const;
        bool   IsUnloaded() const;

    protected:
        virtual void Load() = 0;
        virtual void Unload() = 0;
        virtual void Subscribe() = 0;
        virtual void Unsubscribe() = 0;

    protected:
        String m_type;
        bool   m_b_loaded   = false;
        bool   m_b_unloaded = false;
    };
}
