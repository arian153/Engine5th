#pragma once

namespace Engine5
{
    class Level;

    class LevelFactory
    {
    public:
        LevelFactory();
        ~LevelFactory();

        Level* Create();
    private:
    };
}
