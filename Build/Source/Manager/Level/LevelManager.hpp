#pragma once
#include "../../System/Math/Utility/MathDef.hpp"
#include <list>
#include <unordered_map>

namespace Engine5
{
    class RenderSystem;
    enum class eSubsystemFlag : unsigned long long;
    class LevelResource;
    class Level;
    class SpaceManager;

    class LevelManager
    {
    public:
        LevelManager();
        ~LevelManager();

        void Initialize(SpaceManager* space_manager);
        void Update();
        void Shutdown();

        void AddLevel(const std::string& level_name, LevelResource* level_resource = nullptr);
        void SetInitialLevel(const std::string& level_name);
        void EnableLoadPhase(bool b_enable_load_phase = false);
        void SetQuit();

        //state change methods
        void ChangeLevel(const std::string& level_name);
        void RestartLevel();
        void ReloadLevel();
        void PauseAndChangeLevel(const std::string& level_name);
        bool ResumeLevel();
        bool ResumeAndRestartLevel();

        std::string GetCurrentLevelName() const;
        Level*      GetCurrentLevel() const;

        void UpdatePausedSpace(Real dt, const std::string& level_name, size_t space_index, eSubsystemFlag flag);
        void UpdatePausedLevels(Real dt);

    private:
        void   RemovePausedLevel();
        Level* CreateLevel(const std::string& level_name);

        //loop state
        void InitializeLevel(Level* level) const;
        void UpdateLevel(Level* level, Real dt) const;
        void FixedUpdateLevel(Level* level, Real dt) const;
        void RenderLevel(Level* level, Real dt);
        void ShutdownLevel(Level* level) const;
        void LoadLevel(Level* level) const;
        void UnloadLevel(Level* level) const;

    private:
        Real m_fixed_time_step = 0.0f;
        Real m_elapsed_time    = 0.0f;
        bool m_b_fixed_update  = false;

        std::string m_current;
        std::string m_next;
        Level*      m_level = nullptr;

        bool m_b_set_initial_level  = false;
        bool m_b_enable_load_phase  = false;
        bool m_b_quit_state_machine = false;

        bool m_b_reload           = false;
        bool m_b_restart          = false;
        bool m_b_pause_and_change = false;
        bool m_b_resume           = false;
        bool m_b_resume_restart   = false;

    private:
        struct PauseInfo
        {
            std::string type;
            Level*      level = nullptr;
        };

    private:
        std::list<PauseInfo> m_pause_levels;
        SpaceManager*        m_space_manager  = nullptr;
        RenderSystem*        m_render_system  = nullptr;

        //resource
        std::unordered_map<std::string, LevelResource*> m_level_resources;
    };
}
