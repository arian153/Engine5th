#include "Tetrahedron.hpp"
#include "../../Utility/Utility.hpp"
#include "../../../Core/Utility/CoreDef.hpp"
#include "../../../Graphics/Utility/PrimitiveRenderer.hpp"

namespace Engine5
{
    Tetrahedron::Tetrahedron()
    {
        type = PrimitiveType::Tetrahedron;
    }

    Tetrahedron::~Tetrahedron()
    {
    }

    void Tetrahedron::Initialize()
    {
        SetUnit();
    }

    void Tetrahedron::Shutdown()
    {
    }

    void Tetrahedron::SetUnit()
    {
        vertices[0] = Vector3::Origin();
        vertices[1] = Vector3::AxisX();
        vertices[2] = Vector3::AxisY();
        vertices[3] = Vector3::AxisZ();
    }

    Vector3 Tetrahedron::Support(const Vector3& direction)
    {
        Real    p = Math::REAL_NEGATIVE_MAX;
        Vector3 result;
        for (size_t i = 0; i < 4; ++i)
        {
            Real projection = vertices[i].DotProduct(direction);
            if (projection > p)
            {
                result = vertices[i];
                p      = projection;
            }
        }
        return result;
    }

    bool Tetrahedron::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        minimum_t = local_ray.direction.DotProduct(local_ray.position);
        maximum_t = -1.0f;
        return false;
    }

    Vector3 Tetrahedron::GetNormal(const Vector3& local_point_on_primitive)
    {
        return local_point_on_primitive;
    }

    void Tetrahedron::DrawPrimitive(PrimitiveRenderer* renderer, RenderingMode mode, const Color& color)
    {
        I32 index = static_cast<I32>(renderer->VerticesSize(mode));
        renderer->ReserveVertices(4, mode);
        Vector3 world_vertices[4];
        std::memcpy(world_vertices, vertices, sizeof(vertices));
        for (auto& vertex : world_vertices)
        {
            //local space to world space
            vertex = orientation.Rotate(vertex);
            vertex += position;

            //push to renderer
            renderer->PushVertex(vertex, mode, color);
        }

        //add indices
        if (mode == RenderingMode::Dot)
        {
            for (I32 i = 0; i < 4; ++i)
            {
                renderer->PushIndex(index + i, mode);
            }
        }
        else if (mode == RenderingMode::Line)
        {
            renderer->PushLineIndices(index + 0, index + 1);
            renderer->PushLineIndices(index + 1, index + 2);
            renderer->PushLineIndices(index + 2, index + 3);
            renderer->PushLineIndices(index + 3, index + 0);
        }
        else if (mode == RenderingMode::Face)
        {
            renderer->PushFaceIndices(index + 0, index + 1, index + 2);
            renderer->PushFaceIndices(index + 0, index + 1, index + 3);
            renderer->PushFaceIndices(index + 0, index + 2, index + 3);
            renderer->PushFaceIndices(index + 1, index + 2, index + 3);
        }
    }
}
