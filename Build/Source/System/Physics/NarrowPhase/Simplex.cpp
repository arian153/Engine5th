#include "Simplex.hpp"
#include <algorithm>

namespace Engine5
{
    Simplex::Simplex()
    {
    }

    Simplex::~Simplex()
    {
    }

    SupportPoint& Simplex::operator[](size_t index)
    {
        switch (index)
        {
        case 0:
            return simplex_vertex_d;
        case 1:
            return simplex_vertex_c;
        case 2:
            return simplex_vertex_b;
        case 3:
            return simplex_vertex_a;
        default:
            return simplex_vertex_a;
        }
    }

    SupportPoint Simplex::operator[](size_t index) const
    {
        switch (index)
        {
        case 0:
            return simplex_vertex_d;
        case 1:
            return simplex_vertex_c;
        case 2:
            return simplex_vertex_b;
        case 3:
            return simplex_vertex_a;
        default:
            return simplex_vertex_a;
        }
    }

    bool Simplex::DoSimplexPoint(Vector3& dir)
    {
        dir              = -simplex_vertex_a.global;
        simplex_vertex_b = simplex_vertex_a;
        count            = 1;
        return false;
    }

    bool Simplex::DoSimplexLine(Vector3& dir)
    {
        Vector3 ab       = simplex_vertex_b.global - simplex_vertex_a.global;
        Vector3 ao       = -simplex_vertex_a.global;
        dir              = ab.CrossProductTwice(ao);
        simplex_vertex_c = simplex_vertex_b;
        simplex_vertex_b = simplex_vertex_a;
        count            = 2;
        return false;
    }

    bool Simplex::DoSimplexTriangle(Vector3& dir)
    {
        Vector3 ao  = -simplex_vertex_a.global;
        Vector3 ab  = simplex_vertex_b.global - simplex_vertex_a.global;
        Vector3 ac  = simplex_vertex_c.global - simplex_vertex_a.global;
        Vector3 abc = ab.CrossProduct(ac);

        //point is can't be behind/in the direction of B,C or BC
        Vector3 ab_abc = ab.CrossProduct(abc);
        // is the origin away from ab edge? in the same plane
        //if a0 is in that direction than
        if (ab_abc.DotProduct(ao) > 0.0f)
        {
            //change points
            simplex_vertex_c = simplex_vertex_b;
            simplex_vertex_b = simplex_vertex_a;

            //dir is not ab_abc because it's not point towards the origin
            dir = ab.CrossProductTwice(ao);

            //direction change; can't build tetrahedron
            return false;
        }
        Vector3 abc_ac = abc.CrossProduct(ac);

        // is the origin away from ac edge? or it is in abc?
        //if a0 is in that direction than
        if (abc_ac.DotProduct(ao) > 0.0f)
        {
            //keep c the same
            simplex_vertex_b = simplex_vertex_a;

            //dir is not abc_ac because it's not point towards the origin
            dir = ac.CrossProductTwice(ao);

            //direction change; can't build tetrahedron
            return false;
        }

        //now can build tetrahedron; check if it's above or below
        if (abc.DotProduct(ao) > 0.0f)
        {
            //base of tetrahedron
            simplex_vertex_d = simplex_vertex_c;
            simplex_vertex_c = simplex_vertex_b;
            simplex_vertex_b = simplex_vertex_a;

            //new direction
            dir = abc;
        }
        else
        {
            //upside down tetrahedron
            simplex_vertex_d = simplex_vertex_b;
            simplex_vertex_b = simplex_vertex_a;
            dir              = -abc;
        }
        count = 3;
        return false;
    }

    bool Simplex::DoSimplexTetrahedron(Vector3& dir)
    {
        Vector3 ao  = -simplex_vertex_a.global;
        Vector3 ab  = simplex_vertex_b.global - simplex_vertex_a.global;
        Vector3 ac  = simplex_vertex_c.global - simplex_vertex_a.global;
        Vector3 ad  = simplex_vertex_d.global - simplex_vertex_a.global;
        Vector3 bcd = (simplex_vertex_c.global - simplex_vertex_b.global).CrossProduct(simplex_vertex_d.global - simplex_vertex_b.global);
        if (dir.DotProduct(bcd) > 0)
        {
            // A is above BCD. normal of ABC, ADB, ACD are candidates
            dir = ab.CrossProduct(ac);
            if (ao.DotProduct(dir) > 0.0f)
            {
                simplex_vertex_d = simplex_vertex_c;
                simplex_vertex_c = simplex_vertex_b;
                simplex_vertex_b = simplex_vertex_a;
            }
            else
            {
                dir = ad.CrossProduct(ab);
                if (ao.DotProduct(dir) > 0.0f)
                {
                    simplex_vertex_c = simplex_vertex_b;
                    simplex_vertex_b = simplex_vertex_a;
                }
                else
                {
                    dir = ac.CrossProduct(ad);
                    if (ao.DotProduct(dir) > 0.0f)
                    {
                        simplex_vertex_b = simplex_vertex_a;
                    }
                    else
                    {
                        // origin is in the tetrahedron ABCD
                        count = 4;
                        return true;
                    }
                }
            }
        }
        else
        {
            // A is under BCD. normal of ACB, ABD, ADC are candidates
            dir = ac.CrossProduct(ab);
            if (ao.DotProduct(dir) > 0.0f)
            {
                simplex_vertex_d = simplex_vertex_c;
                simplex_vertex_c = simplex_vertex_b;
                simplex_vertex_b = simplex_vertex_a;
            }
            else
            {
                dir = ab.CrossProduct(ad);
                if (ao.DotProduct(dir) > 0.0f)
                {
                    simplex_vertex_c = simplex_vertex_b;
                    simplex_vertex_b = simplex_vertex_a;
                }
                else
                {
                    dir = ad.CrossProduct(ac);
                    if (ao.DotProduct(dir) > 0.0f)
                    {
                        simplex_vertex_b = simplex_vertex_a;
                    }
                    else
                    {
                        // origin is in the tetrahedron ABCD
                        count = 4;
                        return true;
                    }
                }
            }
        }
        count = 3;
        return false;
    }

    bool Simplex::IsContainOrigin(Vector3& dir)
    {
        if (count == 1)
        {
            return DoSimplexLine(dir);
        }
        if (count == 2)
        {
            return DoSimplexTriangle(dir);
        }
        if (count == 3)
        {
            return DoSimplexTetrahedron(dir);
        }
        return false;
    }

    void Simplex::Push(const SupportPoint& p)
    {
        count            = std::min(count + 1, 4);
        simplex_vertex_d = simplex_vertex_c;
        simplex_vertex_c = simplex_vertex_b;
        simplex_vertex_b = simplex_vertex_a;
        simplex_vertex_a = p;
    }

    void Simplex::Set(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c, const SupportPoint& d)
    {
        count                  = 4;
        this->simplex_vertex_a = a;
        this->simplex_vertex_b = b;
        this->simplex_vertex_c = c;
        this->simplex_vertex_d = d;
    }

    void Simplex::Set(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c)
    {
        count                  = 3;
        this->simplex_vertex_a = a;
        this->simplex_vertex_b = b;
        this->simplex_vertex_c = c;
    }

    void Simplex::Set(const SupportPoint& a, const SupportPoint& b)
    {
        count                  = 2;
        this->simplex_vertex_a = a;
        this->simplex_vertex_b = b;
    }

    void Simplex::Set(const SupportPoint& a)
    {
        count                  = 1;
        this->simplex_vertex_a = a;
    }

    bool Simplex::UpdateSimplexTriangle(Vector3& dir)
    {
        //triangle's normal
        Vector3 n  = (simplex_vertex_b.global - simplex_vertex_a.global).CrossProduct(simplex_vertex_c.global - simplex_vertex_a.global);
        Vector3 AO = -simplex_vertex_a.global; //direction to origin
        //Determine which feature is closest to origin, make that the new simplex
        count = 2;
        if ((simplex_vertex_b.global - simplex_vertex_a.global).CrossProduct(n).DotProduct(AO) > 0.0f)
        {
            //Closest to edge AB
            simplex_vertex_c = simplex_vertex_a;
            dir              = (simplex_vertex_b.global - simplex_vertex_a.global).CrossProductTwice(AO);
            return false;
        }
        if (n.CrossProduct(simplex_vertex_c.global - simplex_vertex_a.global).DotProduct(AO) > 0.0f)
        {
            //Closest to edge AC
            simplex_vertex_b = simplex_vertex_a;
            dir              = (simplex_vertex_c.global - simplex_vertex_a.global).CrossProductTwice(AO);
            return false;
        }
        count = 3;
        if (n.DotProduct(AO) > 0.0f)
        {
            //Above triangle
            simplex_vertex_d = simplex_vertex_c;
            simplex_vertex_c = simplex_vertex_b;
            simplex_vertex_b = simplex_vertex_a;
            dir              = n;
            return false;
        }

        //Below triangle
        simplex_vertex_d = simplex_vertex_b;
        simplex_vertex_b = simplex_vertex_a;
        dir              = -n;
        return false;
    }

    bool Simplex::UpdateSimplexTetrahedron(Vector3& dir)
    {
        // a is peak/tip of pyramid, BCD is the base (counterclockwise winding order)
        //We know a prior that origin is above BCD and below a Get normals of three new faces
        Vector3 ABC = (simplex_vertex_b.global - simplex_vertex_a.global).CrossProduct(simplex_vertex_c.global - simplex_vertex_a.global);
        Vector3 ACD = (simplex_vertex_c.global - simplex_vertex_a.global).CrossProduct(simplex_vertex_d.global - simplex_vertex_a.global);
        Vector3 ADB = (simplex_vertex_d.global - simplex_vertex_a.global).CrossProduct(simplex_vertex_b.global - simplex_vertex_a.global);
        Vector3 AO  = -simplex_vertex_a.global; //dir to origin
        count       = 3; //hoisting this just cause
        //Plane-test origin with 3 faces
        if (ABC.DotProduct(AO) > 0.0f)
        { 
            //In front of ABC
            simplex_vertex_d = simplex_vertex_c;
            simplex_vertex_c = simplex_vertex_b;
            simplex_vertex_b = simplex_vertex_a;
            dir              = ABC;
            return false;
        }
        if (ACD.DotProduct(AO) > 0.0f)
        { 
            //In front of ACD
            simplex_vertex_b = simplex_vertex_a;
            dir              = ACD;
            return false;
        }
        if (ADB.DotProduct(AO) > 0.0f)
        { 
            //In front of ADB
            simplex_vertex_c = simplex_vertex_d;
            simplex_vertex_d = simplex_vertex_b;
            simplex_vertex_b = simplex_vertex_a;
            dir              = ADB;
            return false;
        }

        //else inside tetrahedron; enclosed!
        return true;
    }
}
