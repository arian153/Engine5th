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
        if (m_b_set_initial_level == false)
        {
            return;
        }
        LoadPhase();
        InitializePhase();
        UpdatePhase();
        ShutdownPhase();
        UnloadPhase();
    }

    void LevelManager::Shutdown()
    {
        delete m_current_level;
        m_current_level = nullptr;
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

    void LevelManager::ChangeNextLevel(const std::string& level_name)
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

    void LevelManager::PauseAndNextLevel(const std::string& level_name)
    {
        m_b_pause = true;
        PauseInfo info;
        info.type = m_current;
        info.level = m_current_level;
        m_pause_levels.push_front(info);
        m_next = level_name;
    }

    bool LevelManager::ResumeLevel()
    {
        if (m_pause_levels.empty() == false)
        {
            m_b_resume = true;
            m_next = m_pause_levels.front().type;
            return true;
        }
        return false;
    }

    bool LevelManager::ResumeAndRestartLevel()
    {
        if (m_pause_levels.empty() == false)
        {
            m_next = m_pause_levels.front().type;
            m_b_restart = true;
            m_b_resume = true;
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

    void LevelManager::ChangeLevel()
    {
        m_current = m_next;
        m_b_restart = false;

        if (m_current_level != nullptr)
        {
            delete m_current_level;
            m_current_level = nullptr;
        }

        //m_current_level = m_level_factory->CreateLevel(m_current, this);
    }

    void LevelManager::UpdatePausedLevel(Real dt)
    {
        if (m_pause_levels.empty() == false)
        {
            for (auto info : m_pause_levels)
            {
                //UpdateLevel(info.level, dt);
            }
        }
    }

    void LevelManager::RemovePausedLevel()
    {
        while (m_pause_levels.empty() == false)
        {
            auto info = m_pause_levels.front();
            //ShutdownLevel(info.level);
            //UnloadLevel(info.level);
            delete info.level;
            m_pause_levels.pop_front();
        }
    }

    void LevelManager::InitializePhase()
    {
    }

    void LevelManager::UpdatePhase()
    {
        //while (m_b_set_quit == false && m_b_restart == false && m_current == m_next)
        {
            //m_application->UpdateApplication();
            //Real time_step = m_application->m_timer->DeltaTime();
            //UpdateLevel(m_level, time_step);
            //m_elapsed_time += time_step;
            if (m_elapsed_time >= m_fixed_time_step)
            {
                //Update Fixed Update
                m_elapsed_time = 0.0f;
            }
        }
    }

    void LevelManager::ShutdownPhase()
    {
    }

    void LevelManager::LoadPhase()
    {
    }

    void LevelManager::UnloadPhase()
    {
    }
}
