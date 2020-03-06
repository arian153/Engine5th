#pragma once
#include "../../System/Math/Utility/MathDef.hpp"

namespace Engine5
{
    class Level;
    class SpaceManager;

    class LevelManager
    {
    public:
        LevelManager();
        ~LevelManager();

        void Init();
        void Update(Real dt);
        void Shutdown();

        void AddLevel(const std::string& level_name);
        void SetInitialLevel(const std::string& level_name);
        void SetLoadingDataInLevel(bool b_loading_data_in_level = false);
        void SetQuit();

        //state change methods
        void ChangeLevel(const std::string& level_name);
        void RestartLevel();
        void ReloadLevel();
        void PauseLevel();
        void PauseAndChangeLevel(const std::string& level_name);
        bool ResumeLevel();
        bool ResumeAndRestartLevel();

        std::string GetCurrentLevelName() const;
        Level*      GetCurrentLevel() const;

    private:
    };
}
