#include "MeshResource.hpp"
#include <fstream>
#include "../../../System/Graphics/DataType/MeshFace.hpp"
#include "../../../System/Math/Algebra/Vector3.hpp"
#include "../../../System/Math/Algebra/Vector2.hpp"
#include "../../../System/Math/Primitive/ConvexHull2D/Triangle.hpp"

namespace Engine5
{
    MeshResource::MeshResource(const std::wstring& path)
        : Resource(path)
    {
        m_type = eResourceType::Mesh;
    }

    MeshResource::~MeshResource()
    {
    }

    void MeshResource::Initialize()
    {
        std::ifstream file_stream;
        // Open the model file.
        file_stream.open(m_file_path);
        // If it could not open the file then exit.
        if (file_stream.fail())
        {
            return;
        }
        m_mesh_data.b_resource = true;
        CheckMeshType();
        switch (m_mesh_type)
        {
        case eMeshType::Invalid:
            break;
        case eMeshType::CustomTXT:
            LoadCustomTXT();
            break;
        case eMeshType::WaveFrontOBJ:
            LoadWaveFrontOBJ(file_stream);
            break;
        default:
            break;
        }
        file_stream.close();
    }

    void MeshResource::Shutdown()
    {
    }

   
    void MeshResource::LoadWaveFrontOBJ(std::ifstream& file)
    {
        // Read in the vertices, texture coordinates, and normals into the data structures.
        // Important: Also convert to left hand coordinate system since Maya uses right hand coordinate system.
        size_t                vertex_index = 0;
        size_t                uv_index     = 0;
        size_t                normal_index = 0;
        size_t                face_index   = 0;
        char                  input, input2;
        std::vector<Vector3>  points;
        std::vector<Vector3>  normals;
        std::vector<Vector2>  uvs;
        std::vector<MeshFace> faces;
        file.get(input);
        while (!file.eof())
        {
            if (input == 'v')
            {
                file.get(input);
                // Read in the vertices.
                if (input == ' ')
                {
                    Vector3 vertex;
                    file >> vertex.x >> vertex.y >> vertex.z;
                    // Invert the Z vertex to change to left hand system.
                    vertex.z = vertex.z * -1.0f;
                    vertex_index++;
                    points.push_back(vertex);
                }
                // Read in the texture uv coordinates.
                if (input == 't')
                {
                    Vector2 uv;
                    file >> uv.x >> uv.y;
                    // Invert the V texture coordinates to left hand system.
                    uv.y = 1.0f - uv.y;
                    uv_index++;
                    uvs.push_back(uv);
                }
                // Read in the normals.
                if (input == 'n')
                {
                    Vector3 normal;
                    file >> normal.x >> normal.y >> normal.z;
                    // Invert the Z normal to change to left hand system.
                    normal.z = normal.z * -1.0f;
                    normal_index++;
                    normals.push_back(normal);
                }
            }
            // Read in the faces.
            if (input == 'f')
            {
                file.get(input);
                if (input == ' ')
                {
                    MeshFace face;
                    // Read the face data in backwards to convert it to a left hand system from right hand system.
                    file >> face.vertex_index_c;
                    file.get(input2);
                    if (input2 == ' ')
                    {
                        file >> face.vertex_index_b;
                        file >> face.vertex_index_a;
                    }
                    else if (input2 == '/')
                    {
                        file.get(input2);
                        if (input2 != '/')
                        {
                            file.unget();
                            file >> face.uv_index_c;
                            file.get(input2);
                            if (input2 == ' ')
                            {
                                file >> face.vertex_index_b;
                                file.get(input2);
                                file >> face.uv_index_b;
                                file >> face.vertex_index_a;
                                file.get(input2);
                                file >> face.uv_index_a;
                            }
                            else if (input2 == '/')
                            {
                                file >> face.normal_index_c;
                                file >> face.vertex_index_b;
                                file.get(input2);
                                file >> face.uv_index_b;
                                file.get(input2);
                                file >> face.normal_index_b;
                                file >> face.vertex_index_a;
                                file.get(input2);
                                file >> face.uv_index_a;
                                file.get(input2);
                                file >> face.normal_index_a;
                            }
                        }
                        else
                        {
                            file >> face.normal_index_c;
                            file >> face.vertex_index_b;
                            file.get(input2);
                            file.get(input2);
                            file >> face.normal_index_b;
                            file >> face.vertex_index_a;
                            file.get(input2);
                            file.get(input2);
                            file >> face.normal_index_a;
                        }
                    }
                    face_index++;
                    faces.push_back(face);
                }
            }
            // Read in the remainder of the line.
            while (input != '\n')
            {
                file.get(input);
            }
            // Start reading the beginning of the next line.
            file.get(input);
        }
        bool b_uv     = !uvs.empty();
        bool b_normal = !normals.empty();
        U32  index    = 0;
        for (auto& face : faces)
        {
            TextureVertex vertex_a, vertex_b, vertex_c;
            vertex_a.SetPosition(points[face.vertex_index_a - 1]);
            vertex_b.SetPosition(points[face.vertex_index_b - 1]);
            vertex_c.SetPosition(points[face.vertex_index_c - 1]);
            if (b_uv)
            {
                vertex_a.SetUV(uvs[face.uv_index_a - 1]);
                vertex_b.SetUV(uvs[face.uv_index_b - 1]);
                vertex_c.SetUV(uvs[face.uv_index_c - 1]);
            }
            if (b_normal)
            {
                vertex_a.SetNormal(normals[face.normal_index_a - 1]);
                vertex_b.SetNormal(normals[face.normal_index_b - 1]);
                vertex_c.SetNormal(normals[face.normal_index_c - 1]);
            }
            else
            {
                Vector3 normal = Triangle::Normal(vertex_a.GetPosition(), vertex_b.GetPosition(), vertex_c.GetPosition());
                vertex_a.SetNormal(normal);
                vertex_b.SetNormal(normal);
                vertex_c.SetNormal(normal);
            }
            vertex_a.CalculateTangentAndBinormal();
            vertex_b.CalculateTangentAndBinormal();
            vertex_c.CalculateTangentAndBinormal();
            m_mesh_data.vertices.push_back(vertex_a);
            m_mesh_data.vertices.push_back(vertex_b);
            m_mesh_data.vertices.push_back(vertex_c);
            m_mesh_data.indices.push_back(index);
            m_mesh_data.indices.push_back(index + 1);
            m_mesh_data.indices.push_back(index + 2);
            index += 3;
        }
    }

    void MeshResource::LoadCustomTXT()
    {
    }

    void MeshResource::CheckMeshType()
    {
        if (m_file_type == L".obj")
        {
            m_mesh_type = eMeshType::WaveFrontOBJ;
        }
    }

    MeshData* MeshResource::GetMeshData()
    {
        return &m_mesh_data;
    }
}
