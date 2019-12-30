#pragma once
#include "../Algebra/Vector2.hpp"
#include "../Algebra/Vector3.hpp"

namespace Engine5
{
    //Left-Right, Up-Down
    enum class Anchor2D : size_t
    {
        LU = 110, CU = 210, RU = 310,
        LC = 120, CC = 220, RC = 320,
        LD = 130, CD = 230, RD = 330,
        Invalid = 000
    };

    //Left-Right, Up-Down, Front-Back
    enum class Anchor3D : size_t
    {
        LUF = 111, CUF = 211, RUF = 311,
        LUC = 112, CUC = 212, RUC = 312,
        LUB = 113, CUB = 213, RUB = 313,

        LCF = 121, CCF = 221, RCF = 321,
        LCC = 122, CCC = 222, RCC = 322,
        LCB = 123, CCB = 223, RCB = 323,

        LDF = 131, CDF = 231, RDF = 331,
        LDC = 132, CDC = 232, RDC = 332,
        LDB = 133, CDB = 233, RDB = 333,
        Invalid = 000
    };

    enum class AnchorLR : size_t
    {
        Left = 100, Center = 200, Right = 300
    };

    enum class AnchorUD : size_t
    {
        Up = 10, Center = 20, Down = 30
    };

    enum class AnchorFB : size_t
    {
        Front = 1, Center = 2, Back = 3
    };

    class Anchor
    {
    public:
        Anchor();
        ~Anchor();

        static Anchor2D ToAnchor2D(AnchorLR left_right, AnchorUD up_down);
        static Anchor3D ToAnchor3D(AnchorLR left_right, AnchorUD up_down, AnchorFB front_back);
        static Vector2 AnchorVector2(Anchor2D anchor);
        static Vector3 AnchorVector3(Anchor3D anchor);

        static Vector2 ToVector2(AnchorLR left_right, AnchorUD up_down);
        static Vector3 ToVector3(AnchorLR left_right, AnchorUD up_down, AnchorFB front_back);
        
    private:

    };

}
