#include "SpaceFactory.hpp"
#include "Space.hpp"

namespace Engine5
{
    SpaceFactory::SpaceFactory()
    {
    }

    SpaceFactory::~SpaceFactory()
    {
    }

    Space* SpaceFactory::Create()
    {
        return new Space();
    }
}
