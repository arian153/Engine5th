#include "Anchor.hpp"

namespace Engine5
{
    Anchor::Anchor()
    {
    }

    Anchor::~Anchor()
    {
    }

    Anchor2D Anchor::ToAnchor2D(AnchorDirection horizontal, AnchorDirection vertical)
    {
        if (horizontal == AnchorDirection::Left)
        {
            if (vertical == AnchorDirection::Top)
            {
                return Anchor2D::TL;
            }
            if (vertical == AnchorDirection::Center)
            {
                return Anchor2D::CL;
            }
            if (vertical == AnchorDirection::Bottom)
            {
                return Anchor2D::BL;
            }
        }
        else if (horizontal == AnchorDirection::Center)
        {
            if (vertical == AnchorDirection::Top)
            {
                return Anchor2D::TC;
            }
            if (vertical == AnchorDirection::Center)
            {
                return Anchor2D::CC;
            }
            if (vertical == AnchorDirection::Bottom)
            {
                return Anchor2D::BC;
            }
        }
        else if (horizontal == AnchorDirection::Right)
        {
            if (vertical == AnchorDirection::Top)
            {
                return Anchor2D::TR;
            }
            if (vertical == AnchorDirection::Center)
            {
                return Anchor2D::CR;
            }
            if (vertical == AnchorDirection::Bottom)
            {
                return Anchor2D::BR;
            }
        }

        return Anchor2D::Invalid;
    }

    Anchor3D Anchor::ToAnchor3D(AnchorDirection horizontal, AnchorDirection vertical, AnchorDirection front_rear)
    {
        if (horizontal == AnchorDirection::Left)
        {
            if (vertical == AnchorDirection::Top)
            {
                if (front_rear == AnchorDirection::Front)
                {
                    return Anchor3D::TLF;
                }
                if (front_rear == AnchorDirection::Center)
                {
                    return Anchor3D::TLC;
                }
                if (front_rear == AnchorDirection::Back)
                {
                    return Anchor3D::TLB;
                }
            }
            else if (vertical == AnchorDirection::Center)
            {
                if (front_rear == AnchorDirection::Front)
                {
                    return Anchor3D::CLF;
                }
                if (front_rear == AnchorDirection::Center)
                {
                    return Anchor3D::CLC;
                }
                if (front_rear == AnchorDirection::Back)
                {
                    return Anchor3D::CLB;
                }
            }
            else if (vertical == AnchorDirection::Bottom)
            {
                if (front_rear == AnchorDirection::Front)
                {
                    return Anchor3D::BLF;
                }
                if (front_rear == AnchorDirection::Center)
                {
                    return Anchor3D::BLC;
                }
                if (front_rear == AnchorDirection::Back)
                {
                    return Anchor3D::BLB;
                }
            }
        }
        else if (horizontal == AnchorDirection::Center)
        {
            if (vertical == AnchorDirection::Top)
            {
                if (front_rear == AnchorDirection::Front)
                {
                    return Anchor3D::TCF;
                }
                if (front_rear == AnchorDirection::Center)
                {
                    return Anchor3D::TCC;
                }
                if (front_rear == AnchorDirection::Back)
                {
                    return Anchor3D::TCB;
                }
            }
            else if (vertical == AnchorDirection::Center)
            {
                if (front_rear == AnchorDirection::Front)
                {
                    return Anchor3D::CCF;
                }
                if (front_rear == AnchorDirection::Center)
                {
                    return Anchor3D::CCC;
                }
                if (front_rear == AnchorDirection::Back)
                {
                    return Anchor3D::CCB;
                }
            }
            else if (vertical == AnchorDirection::Bottom)
            {
                if (front_rear == AnchorDirection::Front)
                {
                    return Anchor3D::BCF;
                }
                if (front_rear == AnchorDirection::Center)
                {
                    return Anchor3D::BCC;
                }
                if (front_rear == AnchorDirection::Back)
                {
                    return Anchor3D::BCB;
                }
            }
        }
        else if (horizontal == AnchorDirection::Right)
        {
            if (vertical == AnchorDirection::Top)
            {
                if (front_rear == AnchorDirection::Front)
                {
                    return Anchor3D::TRF;
                }
                if (front_rear == AnchorDirection::Center)
                {
                    return Anchor3D::TRC;
                }
                if (front_rear == AnchorDirection::Back)
                {
                    return Anchor3D::TRB;
                }
            }
            else if (vertical == AnchorDirection::Center)
            {
                if (front_rear == AnchorDirection::Front)
                {
                    return Anchor3D::CRF;
                }
                if (front_rear == AnchorDirection::Center)
                {
                    return Anchor3D::CRC;
                }
                if (front_rear == AnchorDirection::Back)
                {
                    return Anchor3D::CRB;
                }
            }
            else if (vertical == AnchorDirection::Bottom)
            {
                if (front_rear == AnchorDirection::Front)
                {
                    return Anchor3D::BRF;
                }
                if (front_rear == AnchorDirection::Center)
                {
                    return Anchor3D::BRC;
                }
                if (front_rear == AnchorDirection::Back)
                {
                    return Anchor3D::BRB;
                }
            }
        }

        return Anchor3D::Invalid;
    }
}
