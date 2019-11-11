#include "World.hpp"
#include "../ColliderPrimitive/ColliderBox.hpp"
#include "../ColliderPrimitive/ColliderCapsule.hpp"
#include "../ColliderPrimitive/ColliderCone.hpp"
#include "../ColliderPrimitive/ColliderCylinder.hpp"
#include "../ColliderPrimitive/ColliderDome.hpp"
#include "../ColliderPrimitive/ColliderEllipsoid.hpp"
#include "../ColliderPrimitive/ColliderPolyhedron.hpp"
#include "../ColliderPrimitive/ColliderSphere.hpp"
#include "../ColliderPrimitive/ColliderTetrahedron.hpp"
#include "../ColliderPrimitive/ColliderTruncated.hpp"

namespace Engine5
{
    World::World()
    {
    }

    World::~World()
    {
    }

    void World::Initialize()
    {
    }

    void World::Update(Real dt)
    {
    }

    void World::Shutdown()
    {
    }

    RigidBody* World::AddRigidBody(RigidBody* body)
    {
        m_bodies.push_back(body);
        return body;
    }

    ColliderPrimitive* World::AddCollider(RigidBody* body, ColliderType type)
    {
        ColliderPrimitive* primitive;

        switch (type)
        {
        case Engine5::ColliderType::Box:
            primitive = new ColliderBox();
            break;
        case Engine5::ColliderType::Capsule:
            primitive = new ColliderCapsule();
            break;
        case Engine5::ColliderType::Cone:
            primitive = new ColliderCone();
            break;
        case Engine5::ColliderType::Cylinder:
            primitive = new ColliderCylinder();
            break;
        case Engine5::ColliderType::Dome:
            primitive = new ColliderDome();
            break;
        case Engine5::ColliderType::Ellipsoid:
            primitive = new ColliderEllipsoid();
            break;
        case Engine5::ColliderType::Polyhedron:
            primitive = new ColliderPolyhedron();
            break;
        case Engine5::ColliderType::Sphere:
            primitive = new ColliderSphere();
            break;
        case Engine5::ColliderType::Tetrahedron:
            primitive = new ColliderTetrahedron();
            break;
        case Engine5::ColliderType::Truncated:
            primitive = new ColliderTruncated();
            break;
        default:
            return nullptr;
        }

        if(body != nullptr)
        {
            primitive->m_rigid_body = body;
            body->m_collider_list->push_back(primitive);
            body->UpdateMassData();
        }

        m_coliders.push_back(primitive);
        return primitive;
    }
}
