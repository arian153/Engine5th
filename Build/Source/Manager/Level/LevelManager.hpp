#pragma once
#include "../../System/Math/Utility/MathDef.hpp"
#include <list>
#include <unordered_map>

namespace Engine5
{
    class LevelResource;
    class Level;
    class SpaceManager;

    class LevelManager
    {
    public:
        LevelManager();
        ~LevelManager();

        void Initialize();
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

    private:
        void   UpdatePausedLevel(Real dt);
        void   RemovePausedLevel();
        Level* CreateLevel(const std::string& level_name);

        //loop state
        void InitializeLevel(Level* level);
        void UpdateLevel(Level* level, Real dt);
        void FixedUpdateLevel(Level* level, Real dt);
        void ShutdownLevel(Level* level);
        void LoadLevel(Level* level);
        void UnloadLevel(Level* level);

    private:
        Real m_fixed_time_step = 0.0f;
        Real m_elapsed_time    = 0.0f;

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
        SpaceManager*        m_space_manager = nullptr;
        //resource
        std::unordered_map<std::string, LevelResource*> m_level_resources;
    };
}
