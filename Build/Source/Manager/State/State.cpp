#include "State.hpp"

namespace Engine5
{
    State::State(const std::string& name)
    {
    }

    State::~State()
    {
    }

    Space* State::GetGlobalSpace() const
    {
        return m_global_space;
    }
}
