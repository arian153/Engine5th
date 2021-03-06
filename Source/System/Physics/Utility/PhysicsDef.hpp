#pragma once

namespace Engine5
{
    enum class eColliderType : int
    {
        Circle
      , Ellipse
      , Polygon
      , Rectangle
      , Triangle
      , Box
      , Capsule
      , Cone
      , Cylinder
      , Dome
      , Ellipsoid
      , Polyhedron
      , Sphere
      , Tetrahedron
      , Truncated
      , InvalidPrimitive
    };

    enum class eMotionMode : int
    {
        Dynamic
      , Static
      , Kinematic
    };

    enum class eBroadPhaseMode : int
    {
        DynamicBVH
      , StaticBVH
      , NSquared
      , GridPartition
    };

    enum class eCollisionDetectionMode : int
    {
        Discrete
      , ContinuousStatic
      , ContinuousFull
    };

    namespace Physics
    {
        namespace Dynamics
        {
            const Real ELASTIC_THRESHOLD = 1.0f;
            const Real BAUMGRATE         = 0.2f;
        }

        namespace Collision
        {
            const Real   BROAD_PHASE_MARGIN           = 0.1f;
            const Real   PERSISTENT_THRESHOLD         = 0.1f;
            const Real   PERSISTENT_THRESHOLD_SQUARED = PERSISTENT_THRESHOLD * PERSISTENT_THRESHOLD;
            const Real   LINEAR_SLOP                  = 0.01f;
            const Real   ANGULAR_SLOP                 = Math::RADIAN * 2.0f;
            const Real   MAX_LINEAR_CORRECTION        = 0.2f;
            const Real   SEPARATION_SLOP              = 0.02f;
            const size_t MAX_MANIFOLD_POINT_COUNT     = 4;
        }
    }
}
