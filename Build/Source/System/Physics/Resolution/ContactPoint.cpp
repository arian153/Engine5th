#include "ContactPoint.hpp"
#include "../../Core/Utility/CoreUtility.hpp"

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
        tangent.SetZero();
        bitangent.SetZero();
        depth                 = 0.0f;
        normal_impulse_sum    = 0.0f;
        tangent_a_impulse_sum = 0.0f;
        tangent_b_impulse_sum = 0.0f;
        b_valid               = true;
        b_persistent          = false;
    }

    ContactPoint& ContactPoint::operator=(const ContactPoint& rhs)
    {
        if (this != &rhs)
        {
            this->collider_a            = rhs.collider_a;
            this->collider_b            = rhs.collider_b;
            this->global_position_a     = rhs.global_position_a;
            this->global_position_b     = rhs.global_position_b;
            this->local_position_a      = rhs.local_position_a;
            this->local_position_b      = rhs.local_position_b;
            this->normal                = rhs.normal;
            this->tangent             = rhs.tangent;
            this->bitangent             = rhs.bitangent;
            this->depth                 = rhs.depth;
            this->normal_impulse_sum    = rhs.normal_impulse_sum;
            this->tangent_a_impulse_sum = rhs.tangent_a_impulse_sum;
            this->tangent_b_impulse_sum = rhs.tangent_b_impulse_sum;
            this->b_valid               = rhs.b_valid;
            this->b_persistent          = rhs.b_persistent;
        }
        return *this;
    }

    bool ContactPoint::operator==(const ContactPoint& rhs) const
    {
        return (normal == rhs.normal && depth == rhs.depth && global_position_a == rhs.global_position_a);
    }

    void ContactPoint::Swap()
    {
        Math::Swap(collider_a, collider_b);
        Math::Swap(global_position_a, global_position_b);
        Math::Swap(local_position_a, local_position_b);
        Basis basis;
        basis.CalculateBasisQuaternion(-normal);
        normal    = basis.i;
        tangent = basis.j;
        bitangent = basis.k;
    }

    ContactPoint ContactPoint::SwappedContactPoint() const
    {
        Basis basis;
        basis.CalculateBasisQuaternion(-normal);
        ContactPoint result;
        result.normal            = basis.i;
        result.tangent         = basis.j;
        result.bitangent         = basis.k;
        result.depth             = this->depth;
        result.collider_a        = this->collider_b;
        result.collider_b        = this->collider_a;
        result.global_position_a = this->global_position_b;
        result.global_position_b = this->global_position_a;
        result.local_position_a  = this->local_position_b;
        result.local_position_b  = this->local_position_a;
        result.b_valid           = this->b_valid;
        return result;
    }
}
