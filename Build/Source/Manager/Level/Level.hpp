#pragma once
#include <string>
#include <vector>
#include <functional>

namespace Engine5
{
    class Space;

    class Level
    {
    public:
        explicit Level(const std::string& name);
        ~Level();

        Space* GetGlobalSpace() const;

    private:
        Space*              m_global_space = nullptr;
        Space*              m_world_space  = nullptr;
        Space*              m_ui_space     = nullptr;

        std::vector<Space*> m_spaces;
        std::string         m_name = "Default";
    };
}
