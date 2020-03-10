#pragma once
#include <string>
#include <vector>
#include <functional>
#include "../../System/Math/Utility/MathDef.hpp"
#include "../Space/SubsystemFlag.hpp"

namespace Engine5
{
    class Space;

    class Level
    {
    public:
        explicit Level(const std::string& name);
        ~Level();

        void Initialize() const;
        void Update(Real dt) const;
        void FixedUpdate(Real dt) const;
        void Shutdown() const;
        void Load() const;
        void Unload() const;

        Space* GetGlobalSpace() const;
        Space* GetUISpace() const;
        Space* GetWorldSpace() const;
        Space* GetSpace(size_t index) const;

        void UpdateSpace(Real dt, size_t index, eSubsystemFlag flag) const;
        void UpdateSubsystem(Real dt, eSubsystemFlag flag) const;
        void FixedUpdateSubsystem(Real dt, eSubsystemFlag flag);

    private:
        friend class LevelManager;

    private:
        Space* m_global_space = nullptr;
        Space* m_world_space  = nullptr;
        Space* m_ui_space     = nullptr;

        eSubsystemFlag m_world_flag = eSubsystemFlag::None;
        eSubsystemFlag m_ui_flag    = eSubsystemFlag::None;

        std::vector<Space*> m_spaces;
        std::string         m_name = "Default";
    };
}