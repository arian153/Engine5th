#include "Anchor.hpp"
#include "../Utility/VectorDef.hpp"

namespace Engine5
{
    Anchor::Anchor()
    {
    }

    Anchor::~Anchor()
    {
    }

    Anchor2D Anchor::ToAnchor2D(AnchorLR left_right, AnchorUD up_down)
    {
        return static_cast<Anchor2D>(static_cast<size_t>(left_right) + static_cast<size_t>(up_down));
    }

    Anchor3D Anchor::ToAnchor3D(AnchorLR left_right, AnchorUD up_down, AnchorFB front_back)
    {
        return static_cast<Anchor3D>(static_cast<size_t>(left_right) + static_cast<size_t>(up_down) + static_cast<size_t>(front_back));
    }

    Vector2 Anchor::AnchorVector2(Anchor2D anchor)
    {
        size_t anchor_code = static_cast<size_t>(anchor);
        size_t x           = anchor_code / 100;
        size_t yz          = anchor_code % 100;
        size_t y           = yz / 10;
        return ToVector2(static_cast<AnchorLR>(x * 100), static_cast<AnchorUD>(y * 10));
    }

    Vector3 Anchor::AnchorVector3(Anchor3D anchor)
    {
        size_t anchor_code = static_cast<size_t>(anchor);
        size_t x           = anchor_code / 100;
        size_t yz          = anchor_code % 100;
        size_t y           = yz / 10;
        size_t z           = yz % 10;
        return ToVector3(static_cast<AnchorLR>(x * 100), static_cast<AnchorUD>(y * 10), static_cast<AnchorFB>(z));
    }

    Vector2 Anchor::ToVector2(AnchorLR left_right, AnchorUD up_down)
    {
        Vector2 result;
        switch (left_right)
        {
        case Engine5::AnchorLR::Left:
            result -= Math::Vector2::X_AXIS;
            break;
        case Engine5::AnchorLR::Center:
            break;
        case Engine5::AnchorLR::Right:
            result += Math::Vector2::X_AXIS;
            break;
        default:
            break;
        }
        switch (up_down)
        {
        case Engine5::AnchorUD::Up:
            result += Math::Vector2::Y_AXIS;
            break;
        case Engine5::AnchorUD::Center:
            break;
        case Engine5::AnchorUD::Down:
            result -= Math::Vector2::Y_AXIS;
            break;
        default:
            break;
        }
        return result;
    }

    Vector3 Anchor::ToVector3(AnchorLR left_right, AnchorUD up_down, AnchorFB front_back)
    {
        Vector3 result;
        switch (left_right)
        {
        case Engine5::AnchorLR::Left:
            result -= Math::Vector3::X_AXIS;
            break;
        case Engine5::AnchorLR::Center:
            break;
        case Engine5::AnchorLR::Right:
            result += Math::Vector3::X_AXIS;
            break;
        default:
            break;
        }
        switch (up_down)
        {
        case Engine5::AnchorUD::Up:
            result += Math::Vector3::Y_AXIS;
            break;
        case Engine5::AnchorUD::Center:
            break;
        case Engine5::AnchorUD::Down:
            result -= Math::Vector3::Y_AXIS;
            break;
        default:
            break;
        }
        switch (front_back)
        {
        case Engine5::AnchorFB::Front:
            result += Math::Vector3::Z_AXIS;
            break;
        case Engine5::AnchorFB::Center:
            break;
        case Engine5::AnchorFB::Back:
            result -= Math::Vector3::Z_AXIS;
            break;
        default:
            break;
        }
        return result;
    }
}
