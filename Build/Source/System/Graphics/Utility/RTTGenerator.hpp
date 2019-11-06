#pragma once
#include "../../Math/Math.hpp"
#include "Color.hpp"

namespace Engine5
{
    class Light;
    class Mesh;
    class Mirror;
    class Sprite;
    class Water;
    class TextSprite;
    class ParticleEmitter;
    class Scene;

    class RTTGenerator
    {
    public:
        RTTGenerator();
        ~RTTGenerator();

        //render elements 
        void Render(Scene* scene);
        void Render(Mesh* mesh);
        void Render(Light* light);
        void Render(Sprite* sprite);
        void Render(ParticleEmitter* particle_emitter);
        void Render(TextSprite* text_sprite);



    private:

    };
    
}