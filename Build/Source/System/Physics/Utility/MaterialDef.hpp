#pragma once
#include "../../Math/Utility/MathDef.hpp"

namespace Engine5
{
    namespace Physics
    {
        enum class eMaterial
        {
            Rock = 1
          , Wood = 2
          , Metal = 4
          , BouncyBall = 8
          , SuperBall = 16
          , Pillow = 32
          , Static = 64
          , Concrete = 128
          , Ice = 256
          , Glass = 512
          , Lubricant = 1024
          , Rubber = 2048
          , Velcro = 4096
          , UserType = 8192
        };

        class MaterialCoefficient
        {
        public:
            explicit MaterialCoefficient(eMaterial type = eMaterial::Rock, Real user_density = 1.0f, Real user_restitution = 1.0f);
            ~MaterialCoefficient();

        public:
            eMaterial type;
            Real      density;
            Real      restitution;
        };

        inline MaterialCoefficient::MaterialCoefficient(eMaterial type, Real user_density, Real user_restitution)
            : type(type)
        {
            switch (type)
            {
            case eMaterial::Rock:
                density = 0.6f;
                restitution = 0.4f;
                break;
            case eMaterial::Wood:
                density = 0.3f;
                restitution = 0.2f;
                break;
            case eMaterial::Metal:
                density = 1.2f;
                restitution = 0.05f;
                break;
            case eMaterial::BouncyBall:
                density = 0.3f;
                restitution = 0.8f;
                break;
            case eMaterial::SuperBall:
                density = 0.3f;
                restitution = 0.95f;
                break;
            case eMaterial::Pillow:
                density = 0.1f;
                restitution = 0.2f;
                break;
            case eMaterial::Static:
                density = 0.0f;
                restitution = 0.4f;
                break;
            case eMaterial::Concrete:
                density = 2.4f;
                restitution = 0.4f;
                break;
            case eMaterial::Ice:
                density = 0.9167f;
                restitution = 0.4f;
                break;
            case eMaterial::Glass:
                density = 2.4f;
                restitution = 0.4f;
                break;
            case eMaterial::Lubricant:
                density = 0.7f;
                restitution = 0.4f;
                break;
            case eMaterial::Rubber:
                density = 1.522f;
                restitution = 0.4f;
                break;
            case eMaterial::Velcro:
                density = 1.52f;
                restitution = 0.4f;
                break;
            case eMaterial::UserType:
                density = user_density;
                restitution = user_restitution;
                break;
            default:
                density = 0.6f;
                restitution = 0.4f;
                break;
            }
        }

        inline MaterialCoefficient::~MaterialCoefficient()
        {
        }

        namespace Material
        {
            const MaterialCoefficient ROCK        = MaterialCoefficient(eMaterial::Rock);
            const MaterialCoefficient WOOD        = MaterialCoefficient(eMaterial::Wood);
            const MaterialCoefficient METAL       = MaterialCoefficient(eMaterial::Metal);
            const MaterialCoefficient BOUNCY_BALL = MaterialCoefficient(eMaterial::BouncyBall);
            const MaterialCoefficient SUPER_BALL  = MaterialCoefficient(eMaterial::SuperBall);
            const MaterialCoefficient PILLOW      = MaterialCoefficient(eMaterial::Pillow);
            const MaterialCoefficient STATIC      = MaterialCoefficient(eMaterial::Static);
            const MaterialCoefficient CONCRETE    = MaterialCoefficient(eMaterial::Concrete);
            const MaterialCoefficient ICE         = MaterialCoefficient(eMaterial::Ice);
            const MaterialCoefficient GLASS       = MaterialCoefficient(eMaterial::Glass);
            const MaterialCoefficient LUBRICANT   = MaterialCoefficient(eMaterial::Lubricant);
            const MaterialCoefficient RUBBER      = MaterialCoefficient(eMaterial::Rubber);
            const MaterialCoefficient VELCRO      = MaterialCoefficient(eMaterial::Velcro);
        }
    }
}
