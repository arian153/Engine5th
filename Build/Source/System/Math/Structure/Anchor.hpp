#pragma once

namespace Engine5
{
    enum class Anchor2D
    {
        TL, TC, TR,
        CL, CC, CR,
        BL, BC, BR,
        Invalid
    };

    //Top-Bottom, Left-Right, Front-Back
    enum class Anchor3D
    {
        TLF, TLC, TLB,
        TCF, TCC, TCB,
        TRF, TRC, TRB,

        CLF, CLC, CLB,
        CCF, CCC, CCB,
        CRF, CRC, CRB,

        BLF, BLC, BLB,
        BCF, BCC, BCB,
        BRF, BRC, BRB,
        Invalid
    };

    enum class AnchorDirection
    {
        Top, Bottom,
        Left, Right,
        Front, Back,
        Center, Invalid
    };

    class Anchor
    {
    public:
        Anchor();
        ~Anchor();

        static Anchor2D ToAnchor2D(AnchorDirection horizontal, AnchorDirection vertical);
        static Anchor3D ToAnchor3D(AnchorDirection horizontal, AnchorDirection vertical, AnchorDirection front_rear);
    private:

    };

}
