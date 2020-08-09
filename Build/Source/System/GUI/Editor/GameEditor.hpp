#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "LevelEditor.hpp"
#include "SpaceEditor.hpp"

namespace Engine5
{
    class GameEditor
    {
    public:
        GameEditor();
        ~GameEditor();

        void Initialize();
        void Update(Real dt);
        void Shutdown();

    private:
        LevelEditor m_level_editor;
        SpaceEditor m_space_editor;
    };
}
