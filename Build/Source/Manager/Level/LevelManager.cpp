#include "LevelManager.hpp"
#include "Level.hpp"
#include "../../System/Core/Utility/CoreUtility.hpp"
#include "../Resource/ResourceType/LevelResource.hpp"
#include "../Space/SpaceManager.hpp"
#include "../Space/Space.hpp"
#include "../../System/Graphics/RenderSystem.hpp"
#include "../../System/Core/OS-API/Application.hpp"
#include "../../System/Core/Utility/TimeUtility.hpp"
#include "../../System/Core/OS-API/OSCommon.hpp"
#include "../../System/Core/Utility/FrameUtility.hpp"

namespace Engine5
{
    LevelManager::LevelManager()
    {
    }

    LevelManager::~LevelManager()
    {
    }

    void LevelManager::Initialize(Application* application)
    {
        m_application       = application;
        m_space_manager     = application->GetSpaceManager();
        m_render_system     = application->GetRenderSystem();
        m_operating_system  = application->GetOperatingSystem();
        m_application_timer = application->GetApplicationTimer();
        m_frame_utility     = application->GetFrameUtility();
    }

    void LevelManager::Update()
    {
        //early quit
        if (m_b_set_initial_level == false)
        {
            return;
        }
        //load phase
        if (m_b_restart == false && m_b_resume == false || m_b_reload == true)
        {
            m_b_reload = false;
            m_current  = m_next;
            if (m_level != nullptr)
            {
                delete m_level;
                m_level = nullptr;
            }
            m_level = CreateLevel(m_current);
            LoadLevel(m_level);
        }
        else
        {
            m_b_restart = false;
        }
        //init phase
        if (m_b_resume == true)
        {
            m_b_resume = false;
            delete m_level;
            m_level = m_pause_levels.front().level;
            m_pause_levels.pop_front();
            m_current = m_next;
            //only resume and restart case
            if (m_b_resume_restart == true)
            {
                m_b_resume_restart = false;
                ShutdownLevel(m_level);
                InitializeLevel(m_level);
            }
        }
        else
        {
            InitializeLevel(m_level);
        }
        //update phase
        while (m_b_quit_state_machine == false && m_b_restart == false && m_b_reload == false && m_current == m_next)
        {


            m_operating_system->DispatchMessagePump();



            //m_game_input->ProcessGamePad();
            //m_game_input->ProcessPressed();
            //m_keyboard_input->ProcessPressed();
            //m_mouse_input->ProcessPressed();
            m_application_timer->Tick();
            if (m_operating_system->IsPaused() == false)
            {
                m_frame_utility->CalculateFrameStatus(m_application_timer->TotalTime());
            }
            Real time_step = m_application_timer->DeltaTime();
            UpdateLevel(m_level, time_step);
            m_elapsed_time += time_step;
            if (m_elapsed_time >= m_fixed_time_step)
            {
                FixedUpdateLevel(m_level, m_fixed_time_step);
                m_elapsed_time   = 0.0f;
                m_b_fixed_update = true;
            }
            RenderLevel(m_level, time_step);
        }
        //shutdown phase
        if (m_b_pause_and_change == false)
        {
            ShutdownLevel(m_level);
            //unload phase
            if (m_b_restart == false || m_b_resume_restart == true || m_b_reload == true)
            {
                UnloadLevel(m_level);
            }
        }
        else
        {
            m_b_pause_and_change = false;
            m_level              = nullptr;
        }
        m_operating_system->SetQuit(m_b_quit_state_machine);
    }

    void LevelManager::Shutdown()
    {
        delete m_level;
        m_level = nullptr;
        RemovePausedLevel();
        for (auto& resource : m_level_resources)
        {
            delete resource.second;
            resource.second = nullptr;
        }
        m_level_resources.clear();
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
        m_b_pause_and_change = true;
        PauseInfo info;
        info.type  = m_current;
        info.level = m_level;
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
        return m_level;
    }

    void LevelManager::UpdatePausedSpace(Real dt, const std::string& level_name, size_t space_index, eSubsystemFlag flag)
    {
        if (m_pause_levels.empty() == false)
        {
            for (auto info : m_pause_levels)
            {
                if (info.type == level_name)
                {
                    info.level->UpdateSpace(dt, space_index, flag);
                }
            }
        }
    }

    void LevelManager::UpdatePausedLevels(Real dt)
    {
        if (m_pause_levels.empty() == false)
        {
            for (auto info : m_pause_levels)
            {
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

    void LevelManager::InitializeLevel(Level* level) const
    {
        level->Initialize();
        level->m_global_space = m_space_manager->GetGlobalSpace();
        level->m_world_space  = m_space_manager->CreateSpace(level->m_world_flag);
        level->m_ui_space     = m_space_manager->CreateSpace(level->m_ui_flag);
    }

    void LevelManager::UpdateLevel(Level* level, Real dt) const
    {
        level->Update(dt);
        //update logic
        //update physics
        level->UpdateSubsystem(dt, eSubsystemFlag::World);
        //update animation
        //update sound
    }

    void LevelManager::FixedUpdateLevel(Level* level, Real dt) const
    {
        level->FixedUpdate(dt);
        //update logic
        //update physics
        level->FixedUpdateSubsystem(dt, eSubsystemFlag::World);
        //update animation
        //update sound
    }

    void LevelManager::RenderLevel(Level* level, Real dt)
    {
        //update scene
        m_render_system->BeginUpdate();
        level->UpdateSubsystem(dt, eSubsystemFlag::Scene);
        if (m_b_fixed_update == true)
        {
            level->FixedUpdateSubsystem(m_fixed_time_step, eSubsystemFlag::Scene);
            m_b_fixed_update = false;
        }
        m_render_system->EndUpdate();
    }

    void LevelManager::ShutdownLevel(Level* level) const
    {
        level->Shutdown();
        for (auto& space : level->m_spaces)
        {
            m_space_manager->RemoveSpace(space);
        }
    }

    void LevelManager::LoadLevel(Level* level) const
    {
        if (m_b_enable_load_phase)
        {
            level->Load();
        }
    }

    void LevelManager::UnloadLevel(Level* level) const
    {
        if (m_b_enable_load_phase)
        {
            level->Unload();
        }
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

    void LevelManager::UpdateActive()
    {
    }

    void LevelManager::UpdatePaused()
    {
    }
}
