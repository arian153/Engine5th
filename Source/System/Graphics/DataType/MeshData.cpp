#include "MeshData.hpp"

#include "Color.hpp"
#include "../../Math/Algebra/Vector2.hpp"
#include "../../Math/Algebra/Vector3.hpp"

namespace Engine5
{
    MeshData::MeshData()
    {
    }

    MeshData::~MeshData()
    {
    }

    void MeshData::Convert(std::vector<ColorVertexCommon>& new_vertices, const Color& color)
    {
        new_vertices.reserve(vertices.size());
        for (auto& vertex : vertices)
        {
            new_vertices.emplace_back(vertex.GetPosition(), color);
        }
    }

    void MeshData::Convert(std::vector<TextureVertexCommon>& new_vertices)
    {
        new_vertices.reserve(vertices.size());
        for (auto& vertex : vertices)
        {
            new_vertices.emplace_back(vertex.GetPosition(), vertex.GetUV());
        }
    }

    void MeshData::Convert(std::vector<GeneralVertexCommon>& new_vertices)
    {
        new_vertices.reserve(vertices.size());
        for (auto& vertex : vertices)
        {
            new_vertices.emplace_back(vertex.GetPosition(), vertex.GetUV(), vertex.GetNormal(), vertex.GetTangent());
        }
    }
}
