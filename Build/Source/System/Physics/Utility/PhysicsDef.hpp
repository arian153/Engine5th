#pragma once

namespace Engine5
{
    enum class ColliderType : int
    {
        Box
      , Capsule
      , Cone
      , Cylinder
      , Ellipsoid
      , Polyhedron
      , Sphere
      , Tetrahedron
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
        DynamicBVH,
        StaticBVH,
        NSquared,
        GridPartition
    };

    enum class CollisionDetectionMode : int
    {
        Discrete,
        ContinuousStatic,
        ContinuousFull
    };
}
