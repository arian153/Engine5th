#include "LevelManager.hpp"

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

    void LevelManager::Update(Real dt)
    {
    }

    void LevelManager::Shutdown()
    {
    }

    void LevelManager::AddLevel(const std::string& level_name)
    {
    }

    void LevelManager::SetInitialLevel(const std::string& level_name)
    {
    }

    void LevelManager::SetLoadingDataInLevel(bool b_loading_data_in_level)
    {
    }

    void LevelManager::SetQuit()
    {
    }

    void LevelManager::ChangeLevel(const std::string& level_name)
    {
    }

    void LevelManager::RestartLevel()
    {
    }

    void LevelManager::ReloadLevel()
    {
    }

    void LevelManager::PauseLevel()
    {
    }

    void LevelManager::PauseAndChangeLevel(const std::string& level_name)
    {
    }

    bool LevelManager::ResumeLevel()
    {
    }

    bool LevelManager::ResumeAndRestartLevel()
    {
    }

    std::string LevelManager::GetCurrentLevelName() const
    {
    }

    Level* LevelManager::GetCurrentLevel() const
    {
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
