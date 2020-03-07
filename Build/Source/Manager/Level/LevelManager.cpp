#include "LevelManager.hpp"
#include "Level.hpp"
#include "../../System/Core/Utility/CoreUtility.hpp"
#include "../Resource/ResourceType/LevelResource.hpp"

namespace Engine5
{
    LevelManager::LevelManager()
    {
    }

    LevelManager::~LevelManager()
    {
    }

    void LevelManager::Init()
    {
    }

    void LevelManager::Update()
    {
        //early quit
        if (m_b_set_initial_level == false)
        {
            return;
        }
        //load phase
        if ((m_b_restart == false && m_b_resume == false) || m_b_reload == true)
        {
            m_current   = m_next;
            m_b_restart = false;
            if (m_current_level != nullptr)
            {
                delete m_current_level;
                m_current_level = nullptr;
            }
            m_current_level = CreateLevel(m_current);
            LoadLevel(m_current_level);
            m_b_reload  = false;
            m_b_restart = false;
        }
        else
        {
            m_b_restart = false;
        }
        //init phase
        if (m_b_resume == true)
        {
            m_b_resume = false;
            delete m_current_level;
            m_current_level = m_pause_levels.front().level;
            m_pause_levels.pop_front();
            m_current = m_next;
            if (m_b_resume_restart == true)
            {
                ShutdownLevel(m_current_level);
                m_b_resume_restart = false;
                InitLevel(m_current_level);
            }
        }
        else
        {
            InitLevel(m_current_level);
        }
        //update phase
        while (m_b_quit_state_machine == false && m_b_restart == false && m_current == m_next)
        {
            //m_application->UpdateApplication();
            Real time_step = 0.0f;//m_application->m_timer->DeltaTime();
            UpdateLevel(m_current_level, time_step);
            m_elapsed_time += time_step;
            if (m_elapsed_time >= m_fixed_time_step)
            {
                //Update Fixed Update
                m_elapsed_time = 0.0f;
            }
        }
        //shutdown phase
        if (m_b_pause == false)
        {
            ShutdownLevel(m_current_level);
        }
        else
        {
            m_b_pause       = false;
            m_current_level = nullptr;
        }
        if (m_b_pause == false)
        {
            if (m_b_restart == false || m_b_resume_restart == true || m_b_reload == true)
            {
                UnloadLevel(m_current_level);
            }
        }
    }

    void LevelManager::Shutdown()
    {
        delete m_current_level;
        m_current_level = nullptr;
        RemovePausedLevel();
    }

    void LevelManager::AddLevel(const std::string& level_name, LevelResource* level_resource)
    {
        auto found = m_level_resources.find(level_name);
        if (found == m_level_resources.end())
        {
            if (level_resource == nullptr)
            {
                level_resource = new LevelResource(StringToWString(level_name));
            }
            m_level_resources.emplace(level_name, level_resource);
        }
    }

    void LevelManager::SetInitialLevel(const std::string& level_name)
    {
        m_b_set_initial_level = true;
        m_current             = level_name;
        m_next                = level_name;
    }

    void LevelManager::EnableLoadPhase(bool b_enable_load_phase)
    {
        m_b_enable_load_phase = b_enable_load_phase;
    }

    void LevelManager::SetQuit()
    {
        m_b_quit_state_machine = true;
    }

    void LevelManager::ChangeLevel(const std::string& level_name)
    {
        m_next = level_name;
        RemovePausedLevel();
    }

    void LevelManager::RestartLevel()
    {
        m_b_restart = true;
    }

    void LevelManager::ReloadLevel()
    {
        m_b_reload = true;
    }

    void LevelManager::PauseAndChangeLevel(const std::string& level_name)
    {
        m_b_pause = true;
        PauseInfo info;
        info.type  = m_current;
        info.level = m_current_level;
        m_pause_levels.push_front(info);
        m_next = level_name;
    }

    bool LevelManager::ResumeLevel()
    {
        if (m_pause_levels.empty() == false)
        {
            m_b_resume = true;
            m_next     = m_pause_levels.front().type;
            return true;
        }
        return false;
    }

    bool LevelManager::ResumeAndRestartLevel()
    {
        if (m_pause_levels.empty() == false)
        {
            m_next             = m_pause_levels.front().type;
            m_b_restart        = true;
            m_b_resume         = true;
            m_b_resume_restart = true;
            return true;
        }
        return false;
    }

    std::string LevelManager::GetCurrentLevelName() const
    {
        return m_current;
    }

    Level* LevelManager::GetCurrentLevel() const
    {
        return m_current_level;
    }

    void LevelManager::UpdatePausedLevel(Real dt)
    {
        if (m_pause_levels.empty() == false)
        {
            for (auto info : m_pause_levels)
            {
                //Todo add Space Info
                UpdateLevel(info.level, dt);
            }
        }
    }

    void LevelManager::RemovePausedLevel()
    {
        while (m_pause_levels.empty() == false)
        {
            auto info = m_pause_levels.front();
            ShutdownLevel(info.level);
            UnloadLevel(info.level);
            delete info.level;
            m_pause_levels.pop_front();
        }
    }

    void LevelManager::InitLevel(Level* level)
    {
    }

    void LevelManager::UpdateLevel(Level* level, Real dt)
    {
    }

    void LevelManager::ShutdownLevel(Level* level)
    {
    }

    void LevelManager::LoadLevel(Level* level)
    {
    }

    void LevelManager::UnloadLevel(Level* level)
    {
    }

    Level* LevelManager::CreateLevel(const std::string& level_name)
    {
        Level* created = nullptr;
        auto   found   = m_level_resources.find(level_name);
        if (found != m_level_resources.end())
        {
            created = new Level(level_name);
            //Todo initialize level ...
        }
        return created;
    }
}
