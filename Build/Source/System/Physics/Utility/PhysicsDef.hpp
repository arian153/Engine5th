#pragma once

namespace Engine5
{
    enum class ColliderType : int
    {
        Box
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

    enum class MotionMode : int
    {
        Dynamic
      , Static
      , Kinematic
    };

    enum class BroadPhaseMode : int
    {
        DynamicBVH
      , StaticBVH
      , NSquared
      , GridPartition
    };

    enum class CollisionDetectionMode : int
    {
        Discrete
      , ContinuousStatic
      , ContinuousFull
    };
}
