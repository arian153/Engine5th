#include "State.hpp"

namespace Engine5
{
    Level::Level(const std::string& name)
    {
    }

    Level::~Level()
    {
    }

    Space* Level::GetGlobalSpace() const
    {
        return m_global_space;
    }
}
