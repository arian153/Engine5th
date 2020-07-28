#pragma once

namespace Engine5
{
    class BlackBoard;

    class ActionList
    {
    public:
        ActionList();
        ~ActionList();

    private:
        BlackBoard* m_black_board = nullptr;
    };
}
