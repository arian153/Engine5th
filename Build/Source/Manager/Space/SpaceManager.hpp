#pragma once
#include "../../System/Math/Utility/MathDef.hpp"
#include <vector>

namespace Engine5
{
    class Space;
    class SpaceFactory;

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

        void SetGlobalOrder(bool b_first);

    private:
        void UpdateFirst(Real dt);
        void UpdateLast(Real dt);

    private:
        bool m_b_global_first = true;
        bool m_b_next_order = true;

        SpaceFactory*       m_space_factory = nullptr;
        Space*              m_global_space  = nullptr;
        std::vector<Space*> m_active_spaces;
        std::vector<Space*> m_inactive_spaces;
    };
}
