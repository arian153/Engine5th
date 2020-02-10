#pragma once
#include "../../System/Math/Utility/MathDef.hpp"
#include <vector>

namespace Engine5
{
    class Space;

    class SpaceManager
    {
    public:
        SpaceManager();
        ~SpaceManager();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

        void Activate(Space* space);
        void Deactivate(Space* space);

    private:
        Space*              m_global_space = nullptr;
        std::vector<Space*> m_active_spaces;
        std::vector<Space*> m_inactive_spaces;
    };
}
