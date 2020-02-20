#pragma once

namespace Engine5
{
    class Space;
    class SpaceFactory
    {
    public:
        SpaceFactory();
        ~SpaceFactory();

        Space* Create();

    private:
    };

}
