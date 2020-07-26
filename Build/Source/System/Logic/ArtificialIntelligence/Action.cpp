#include "Action.hpp"

namespace Engine5
{
    Action::Action()
    {
    }

    Action::~Action()
    {
    }

    void Action::SetBlackBoard(BlackBoard* black_board)
    {
        m_black_board = black_board;
    }
}
