#include "ContactPoint.hpp"

namespace Engine5
{
    ContactPoint::ContactPoint()
    {
    }

    ContactPoint::~ContactPoint()
    {
    }

    void ContactPoint::Clear()
    {
        global_position_a.SetZero();
        global_position_b.SetZero();
        local_position_a.SetZero();
        local_position_b.SetZero();
        normal.SetZero();
        tangent_a.SetZero();
        tangent_b.SetZero();
        depth                 = 0.0f;
        normal_impulse_sum    = 0.0f;
        tangent_a_impulse_sum = 0.0f;
        tangent_b_impulse_sum = 0.0f;
        is_valid              = true;
        is_collide            = false;
        is_persistent         = false;
    }

    ContactPoint& ContactPoint::operator=(const ContactPoint& rhs)
    {
        if (this != &rhs)
        {
            this->global_position_a     = rhs.global_position_a;
            this->global_position_b     = rhs.global_position_b;
            this->local_position_a      = rhs.local_position_a;
            this->local_position_b      = rhs.local_position_b;
            this->normal                = rhs.normal;
            this->tangent_a             = rhs.tangent_a;
            this->tangent_b             = rhs.tangent_b;
            this->depth                 = rhs.depth;
            this->normal_impulse_sum    = rhs.normal_impulse_sum;
            this->tangent_a_impulse_sum = rhs.tangent_a_impulse_sum;
            this->tangent_b_impulse_sum = rhs.tangent_b_impulse_sum;
        }
        return *this;
    }

    bool ContactPoint::operator==(const ContactPoint& rhs) const
    {
        return (normal == rhs.normal && depth == rhs.depth && global_position_a == rhs.global_position_a);
    }
}
