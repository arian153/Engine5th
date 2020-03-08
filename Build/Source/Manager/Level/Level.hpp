#pragma once
#include <string>
#include <vector>
#include <functional>
#include "../../System/Math/Utility/MathDef.hpp"

namespace Engine5
{
    enum class eSubsystemFlag : unsigned long long;
    class Space;

    class Level
    {
    public:
        explicit Level(const std::string& name);
        ~Level();

        Space* GetGlobalSpace() const;
        void Initialize();
        void Update(Real dt) const;
        void FixedUpdate(Real dt) const;
        void Shutdown();

        void UpdateSpace(Real dt, size_t index, eSubsystemFlag flag) const;
        void FixedUpdateSpace(Real dt, size_t index, eSubsystemFlag flag) const;


    private:
        Space*              m_global_space = nullptr;
        Space*              m_world_space  = nullptr;
        Space*              m_ui_space     = nullptr;

        std::vector<Space*> m_spaces;
        std::string         m_name = "Default";
    };
}
