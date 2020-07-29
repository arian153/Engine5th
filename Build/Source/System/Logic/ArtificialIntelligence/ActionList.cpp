#include "ActionList.hpp"

namespace Engine5
{
    ActionList::ActionList(BlackBoard& global_board)
        : m_global_board(global_board)
    {
    }

    ActionList::~ActionList()
    {
    }

    void ActionList::Initialize()
    {
        m_black_board.Clear();
    }

    void ActionList::Update(Real dt)
    {
    }

    void ActionList::Shutdown()
    {
        m_black_board.Clear();
    }

    void ActionList::PushFront(Action* action)
    {
    }

    void ActionList::PushBack(Action* action)
    {
    }

    void ActionList::PopFront()
    {
    }

    void ActionList::PopBack()
    {
    }
}
