#pragma once

namespace Engine5
{
    class Light
    {
    public:
        Light()
        {
        }

        virtual ~Light()
        {
        }

        virtual void Initialize() = 0;

    private:
    };

    enum class eLightType
    {
        DirectionalLight
      , PointLight
      , SpotLight
      , CapsuleLight
    };
}
