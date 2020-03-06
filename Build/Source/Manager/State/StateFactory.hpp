#pragma once

namespace Engine5
{
    class State;

    class StateFactory
    {
    public:
        StateFactory();
        ~StateFactory();

        State* Create();
    private:
    };
}
