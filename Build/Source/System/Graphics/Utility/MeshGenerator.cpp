#pragma warning( disable : 26451)

#include "MeshGenerator.hpp"
#include <algorithm>
#include "../../Core/Utility/CoreUtility.hpp"
#include "../../Math/Utility/Converter.hpp"
#include "../../Math/Algebra/Vector3.hpp"
#include "MeshData.hpp"

namespace Engine5
{
    MeshGenerator::MeshGenerator()
    {
    }

    MeshGenerator::~MeshGenerator()
    {
    }

    MeshData* MeshGenerator::CreateBox(Real width, Real height, Real depth, I32 num_subdivisions) const
    {
        MeshData* mesh_data = new MeshData();
        TextureVertex    v[24];
        Real      w2 = 0.5f * width;
        Real      h2 = 0.5f * height;
        Real      d2 = 0.5f * depth;
        // Fill in the front face vertex data.
        v[0] = TextureVertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        v[1] = TextureVertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        v[2] = TextureVertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        v[3] = TextureVertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
        // Fill in the back face vertex data.
        v[4] = TextureVertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
        v[5] = TextureVertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        v[6] = TextureVertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        v[7] = TextureVertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        // Fill in the top face vertex data.
        v[8]  = TextureVertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        v[9]  = TextureVertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        v[10] = TextureVertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        v[11] = TextureVertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
        // Fill in the bottom face vertex data.
        v[12] = TextureVertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
        v[13] = TextureVertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        v[14] = TextureVertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        v[15] = TextureVertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
        // Fill in the left face vertex data.
        v[16] = TextureVertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
        v[17] = TextureVertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
        v[18] = TextureVertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
        v[19] = TextureVertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
        // Fill in the right face vertex data.
        v[20] = TextureVertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
        v[21] = TextureVertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        v[22] = TextureVertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
        v[23] = TextureVertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
        mesh_data->vertices.assign(&v[0], &v[24]);
        // Create the indices.
        I32 i[36];
        // Fill in the front face index data
        i[0] = 0;
        i[1] = 1;
        i[2] = 2;
        i[3] = 0;
        i[4] = 2;
        i[5] = 3;
        // Fill in the back face index data
        i[6]  = 4;
        i[7]  = 5;
        i[8]  = 6;
        i[9]  = 4;
        i[10] = 6;
        i[11] = 7;
        // Fill in the top face index data
        i[12] = 8;
        i[13] = 9;
        i[14] = 10;
        i[15] = 8;
        i[16] = 10;
        i[17] = 11;
        // Fill in the bottom face index data
        i[18] = 12;
        i[19] = 13;
        i[20] = 14;
        i[21] = 12;
        i[22] = 14;
        i[23] = 15;
        // Fill in the left face index data
        i[24] = 16;
        i[25] = 17;
        i[26] = 18;
        i[27] = 16;
        i[28] = 18;
        i[29] = 19;
        // Fill in the right face index data
        i[30] = 20;
        i[31] = 21;
        i[32] = 22;
        i[33] = 20;
        i[34] = 22;
        i[35] = 23;
        mesh_data->indices.assign(&i[0], &i[36]);
        I32 subdivisions = std::min<I32>(num_subdivisions, 6u);
        for (I32 j = 0; j < subdivisions; ++j)
            Subdivide(*mesh_data);
        CalculateBinormal(*mesh_data);
        return mesh_data;
    }

    MeshData* MeshGenerator::CreateSphere(Real radius, I32 slice_count, I32 stack_count) const
    {
        MeshData* mesh_data = new MeshData();
        TextureVertex    top_vertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        TextureVertex    bottom_vertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        mesh_data->vertices.push_back(top_vertex);
        Real phi_step   = Math::PI / stack_count;
        Real theta_step = Math::TWO_PI / slice_count;
        // Compute vertices for each stack ring (do not count the poles as rings).
        for (I32 i = 1; i <= stack_count - 1; ++i)
        {
            Real phi = i * phi_step;
            // Vertices of ring.
            for (I32 j = 0; j <= slice_count; ++j)
            {
                Real   theta = j * theta_step;
                TextureVertex v;
                // spherical to cartesian
                v.position.x = radius * sinf(phi) * cosf(theta);
                v.position.y = radius * cosf(phi);
                v.position.z = radius * sinf(phi) * sinf(theta);
                // Partial derivative of P with respect to the
                v.tangent.x         = -radius * sinf(phi) * sinf(theta);
                v.tangent.y         = 0.0f;
                v.tangent.z         = +radius * sinf(phi) * cosf(theta);
                DirectX::XMVECTOR T = DirectX::XMLoadFloat3(&v.tangent);
                DirectX::XMStoreFloat3(&v.tangent, DirectX::XMVector3Normalize(T));
                DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&v.position);
                XMStoreFloat3(&v.normal, DirectX::XMVector3Normalize(p));
                v.uv.x = theta / Math::TWO_PI;
                v.uv.y = phi / Math::PI;
                mesh_data->vertices.push_back(v);
            }
        }
        mesh_data->vertices.push_back(bottom_vertex);
        // Compute indices for top stack.  The top stack was written first to the vertex buffer
        // and connects the top pole to the first ring.
        for (I32 i = 1; i <= slice_count; ++i)
        {
            mesh_data->indices.push_back(0);
            mesh_data->indices.push_back(i + 1);
            mesh_data->indices.push_back(i);
        }
        // Compute indices for inner stacks (not connected to poles).
        // Offset the indices to the index of the first vertex in the first ring.
        // This is just skipping the top pole vertex.
        I32 base_index        = 1;
        I32 ring_vertex_count = slice_count + 1;
        for (I32 i = 0; i < stack_count - 2; ++i)
        {
            for (I32 j = 0; j < slice_count; ++j)
            {
                mesh_data->indices.push_back(base_index + i * ring_vertex_count + j);
                mesh_data->indices.push_back(base_index + i * ring_vertex_count + j + 1);
                mesh_data->indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
                mesh_data->indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
                mesh_data->indices.push_back(base_index + i * ring_vertex_count + j + 1);
                mesh_data->indices.push_back(base_index + (i + 1) * ring_vertex_count + j + 1);
            }
        }
        // Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
        // and connects the bottom pole to the bottom ring.
        // South pole vertex was added last.
        I32 south_pole_index = (I32)mesh_data->vertices.size() - 1;
        // Offset the indices to the index of the first vertex in the last ring.
        base_index = south_pole_index - ring_vertex_count;
        for (I32 i = 0; i < slice_count; ++i)
        {
            mesh_data->indices.push_back(south_pole_index);
            mesh_data->indices.push_back(base_index + i);
            mesh_data->indices.push_back(base_index + i + 1);
        }
        CalculateBinormal(*mesh_data);
        return mesh_data;
    }

    MeshData* MeshGenerator::CreateGeodesicSphere(Real radius, I32 subdivisions) const
    {
        MeshData* mesh_data = new MeshData();
        // Put a cap on the number of subdivisions.
        I32 _subdivisions = std::min<I32>(subdivisions, 5);
        // Approximate a sphere by tessellating an icosahedron.
        const float       X       = 0.525731f;
        const float       Z       = 0.850651f;
        DirectX::XMFLOAT3 pos[12] =
        {
            DirectX::XMFLOAT3(-X, 0.0f, Z), DirectX::XMFLOAT3(X, 0.0f, Z), DirectX::XMFLOAT3(-X, 0.0f, -Z), DirectX::XMFLOAT3(X, 0.0f, -Z), DirectX::XMFLOAT3(0.0f, Z, X), DirectX::XMFLOAT3(0.0f, Z, -X), DirectX::XMFLOAT3(0.0f, -Z, X), DirectX::XMFLOAT3(0.0f, -Z, -X), DirectX::XMFLOAT3(Z, X, 0.0f), DirectX::XMFLOAT3(-Z, X, 0.0f), DirectX::XMFLOAT3(Z, -X, 0.0f), DirectX::XMFLOAT3(-Z, -X, 0.0f)
        };
        I32 k[60] =
        {
            1, 4, 0, 4, 9, 0, 4, 5, 9, 8, 5, 4, 1, 8, 4, 1, 10, 8, 10, 3, 8, 8, 3, 5, 3, 2, 5, 3, 7, 2, 3, 10, 7, 10, 6, 7, 6, 11, 7, 6, 0, 11, 6, 1, 0, 10, 1, 6, 11, 0, 9, 2, 11, 9, 5, 2, 9, 11, 2, 7
        };
        mesh_data->vertices.resize(12);
        mesh_data->indices.assign(&k[0], &k[60]);
        for (I32 i                          = 0; i < 12; ++i)
            mesh_data->vertices[i].position = pos[i];
        for (I32 i = 0; i < _subdivisions; ++i)
            Subdivide(*mesh_data);
        // Project vertices onto sphere and scale.
        for (I32 i = 0; i < mesh_data->vertices.size(); ++i)
        {
            // Project onto unit sphere.
            DirectX::XMVECTOR n = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&mesh_data->vertices[i].position));
            // Project onto sphere.
            DirectX::XMVECTOR p = DirectX::XMVectorScale(n, radius);
            DirectX::XMStoreFloat3(&mesh_data->vertices[i].position, p);
            DirectX::XMStoreFloat3(&mesh_data->vertices[i].normal, n);
            // Derive texture coordinates from spherical coordinates.
            float theta = atan2f(mesh_data->vertices[i].position.z, mesh_data->vertices[i].position.x);
            // Put in [0, 2pi].
            if (theta < 0.0f)
                theta += DirectX::XM_2PI;
            float phi                   = acosf(mesh_data->vertices[i].position.y / radius);
            mesh_data->vertices[i].uv.x = theta / DirectX::XM_2PI;
            mesh_data->vertices[i].uv.y = phi / DirectX::XM_PI;
            // Partial derivative of P with respect to theta
            mesh_data->vertices[i].tangent.x = -radius * sinf(phi) * sinf(theta);
            mesh_data->vertices[i].tangent.y = 0.0f;
            mesh_data->vertices[i].tangent.z = +radius * sinf(phi) * cosf(theta);
            DirectX::XMVECTOR T              = DirectX::XMLoadFloat3(&mesh_data->vertices[i].tangent);
            DirectX::XMStoreFloat3(&mesh_data->vertices[i].tangent, DirectX::XMVector3Normalize(T));
        }
        CalculateBinormal(*mesh_data);
        return mesh_data;
    }

    MeshData* MeshGenerator::CreateCylinder(Real bottom_radius, Real top_radius, Real height, I32 slice_count, I32 stack_count) const
    {
        MeshData* mesh_data    = new MeshData();
        float     stack_height = height / stack_count;
        // Amount to increment radius as we move up each stack level from bottom to top.
        float radius_step = (top_radius - bottom_radius) / stack_count;
        I32   ring_count  = stack_count + 1;
        // Compute vertices for each stack ring starting at the bottom and moving up.
        for (I32 i = 0; i < ring_count; ++i)
        {
            Real y = -0.5f * height + i * stack_height;
            Real r = bottom_radius + i * radius_step;
            // vertices of ring
            Real d_theta = Math::TWO_PI / slice_count;
            for (I32 j = 0; j <= slice_count; ++j)
            {
                TextureVertex vertex;
                Real   c        = cosf(j * d_theta);
                Real   s        = sinf(j * d_theta);
                vertex.position = DirectX::XMFLOAT3(r * c, y, r * s);
                vertex.uv.x     = (float)j / slice_count;
                vertex.uv.y     = 1.0f - (float)i / stack_count;
                // This is unit length.
                vertex.tangent       = DirectX::XMFLOAT3(-s, 0.0f, c);
                Real              dr = bottom_radius - top_radius;
                DirectX::XMFLOAT3 bitangent(dr * c, -height, dr * s);
                DirectX::XMVECTOR T = DirectX::XMLoadFloat3(&vertex.tangent);
                DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&bitangent);
                DirectX::XMVECTOR N = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(T, B));
                DirectX::XMStoreFloat3(&vertex.normal, N);
                mesh_data->vertices.push_back(vertex);
            }
        }
        // Add one because we duplicate the first and last vertex per ring
        // since the texture coordinates are different.
        I32 ring_vertex_count = slice_count + 1;
        // Compute indices for each stack.
        for (I32 i = 0; i < stack_count; ++i)
        {
            for (I32 j = 0; j < slice_count; ++j)
            {
                mesh_data->indices.push_back(i * ring_vertex_count + j);
                mesh_data->indices.push_back((i + 1) * ring_vertex_count + j);
                mesh_data->indices.push_back((i + 1) * ring_vertex_count + j + 1);
                mesh_data->indices.push_back(i * ring_vertex_count + j);
                mesh_data->indices.push_back((i + 1) * ring_vertex_count + j + 1);
                mesh_data->indices.push_back(i * ring_vertex_count + j + 1);
            }
        }
        BuildCylinderTopCap(bottom_radius, top_radius, height, slice_count, stack_count, mesh_data);
        BuildCylinderBottomCap(bottom_radius, top_radius, height, slice_count, stack_count, mesh_data);
        CalculateBinormal(*mesh_data);
        return mesh_data;
    }

    MeshData* MeshGenerator::CreateGrid(Real width, Real depth, I32 m, I32 n) const
    {
        MeshData* mesh_data    = new MeshData();
        I32       vertex_count = m * n;
        I32       face_count   = (m - 1) * (n - 1) * 2;
        Real      half_width   = 0.5f * width;
        Real      half_depth   = 0.5f * depth;
        Real      dx           = width / (n - 1);
        Real      dz           = depth / (m - 1);
        Real      du           = 1.0f / (n - 1);
        Real      dv           = 1.0f / (m - 1);
        mesh_data->vertices.resize(vertex_count);
        for (I32 i = 0; i < m; ++i)
        {
            float z = half_depth - i * dz;
            for (I32 j = 0; j < n; ++j)
            {
                float x                                 = -half_width + j * dx;
                mesh_data->vertices[i * n + j].position = DirectX::XMFLOAT3(x, 0.0f, z);
                mesh_data->vertices[i * n + j].normal   = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
                mesh_data->vertices[i * n + j].tangent  = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
                // Stretch texture over grid.
                mesh_data->vertices[i * n + j].uv.x = j * du;
                mesh_data->vertices[i * n + j].uv.y = i * dv;
            }
        }
        mesh_data->indices.resize(face_count * 3); // 3 indices per face
        I32 k = 0;
        for (I32 i = 0; i < m - 1; ++i)
        {
            for (I32 j = 0; j < n - 1; ++j)
            {
                mesh_data->indices[k]     = i * n + j;
                mesh_data->indices[k + 1] = i * n + j + 1;
                mesh_data->indices[k + 2] = (i + 1) * n + j;
                mesh_data->indices[k + 3] = (i + 1) * n + j;
                mesh_data->indices[k + 4] = i * n + j + 1;
                mesh_data->indices[k + 5] = (i + 1) * n + j + 1;
                k += 6; // next quad
            }
        }
        CalculateBinormal(*mesh_data);
        return mesh_data;
    }

    MeshData* MeshGenerator::CreateQuad(Real x, Real y, Real w, Real h, Real depth) const
    {
        MeshData* mesh_data = new MeshData();
        mesh_data->vertices.resize(4);
        mesh_data->indices.resize(6);
        // Position coordinates specified in NDC space.
        mesh_data->vertices[0] = TextureVertex(
                                        x, y - h, depth,
                                        0.0f, 0.0f, -1.0f,
                                        1.0f, 0.0f, 0.0f,
                                        0.0f, 1.0f);
        mesh_data->vertices[1] = TextureVertex(
                                        x, y, depth,
                                        0.0f, 0.0f, -1.0f,
                                        1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f);
        mesh_data->vertices[2] = TextureVertex(
                                        x + w, y, depth,
                                        0.0f, 0.0f, -1.0f,
                                        1.0f, 0.0f, 0.0f,
                                        1.0f, 0.0f);
        mesh_data->vertices[3] = TextureVertex(
                                        x + w, y - h, depth,
                                        0.0f, 0.0f, -1.0f,
                                        1.0f, 0.0f, 0.0f,
                                        1.0f, 1.0f);
        mesh_data->indices[0] = 0;
        mesh_data->indices[1] = 1;
        mesh_data->indices[2] = 2;
        mesh_data->indices[3] = 0;
        mesh_data->indices[4] = 2;
        mesh_data->indices[5] = 3;
        CalculateBinormal(*mesh_data);
        return mesh_data;
    }

    void MeshGenerator::Subdivide(MeshData& mesh_data) const
    {
        // Save a copy of the input geometry.
        MeshData input_data = mesh_data;
        mesh_data.vertices.resize(0);
        mesh_data.indices.resize(0);
        I32 tri_angle_count = (I32)input_data.indices.size() / 3;
        for (I32 i = 0; i < tri_angle_count; ++i)
        {
            TextureVertex v0 = input_data.vertices[input_data.indices[i * 3 + 0]];
            TextureVertex v1 = input_data.vertices[input_data.indices[i * 3 + 1]];
            TextureVertex v2 = input_data.vertices[input_data.indices[i * 3 + 2]];
            TextureVertex m0 = MidPoint(v0, v1);
            TextureVertex m1 = MidPoint(v1, v2);
            TextureVertex m2 = MidPoint(v0, v2);
            mesh_data.vertices.push_back(v0); // 0
            mesh_data.vertices.push_back(v1); // 1
            mesh_data.vertices.push_back(v2); // 2
            mesh_data.vertices.push_back(m0); // 3
            mesh_data.vertices.push_back(m1); // 4
            mesh_data.vertices.push_back(m2); // 5
            mesh_data.indices.push_back(i * 6 + 0);
            mesh_data.indices.push_back(i * 6 + 3);
            mesh_data.indices.push_back(i * 6 + 5);
            mesh_data.indices.push_back(i * 6 + 3);
            mesh_data.indices.push_back(i * 6 + 4);
            mesh_data.indices.push_back(i * 6 + 5);
            mesh_data.indices.push_back(i * 6 + 5);
            mesh_data.indices.push_back(i * 6 + 4);
            mesh_data.indices.push_back(i * 6 + 2);
            mesh_data.indices.push_back(i * 6 + 3);
            mesh_data.indices.push_back(i * 6 + 1);
            mesh_data.indices.push_back(i * 6 + 4);
        }
    }

    TextureVertex MeshGenerator::MidPoint(const TextureVertex& v0, const TextureVertex& v1) const
    {
        DirectX::XMVECTOR p0   = DirectX::XMLoadFloat3(&v0.position);
        DirectX::XMVECTOR p1   = DirectX::XMLoadFloat3(&v1.position);
        DirectX::XMVECTOR n0   = DirectX::XMLoadFloat3(&v0.normal);
        DirectX::XMVECTOR n1   = DirectX::XMLoadFloat3(&v1.normal);
        DirectX::XMVECTOR tan0 = DirectX::XMLoadFloat3(&v0.tangent);
        DirectX::XMVECTOR tan1 = DirectX::XMLoadFloat3(&v1.tangent);
        DirectX::XMVECTOR uv0  = DirectX::XMLoadFloat2(&v0.uv);
        DirectX::XMVECTOR uv1  = DirectX::XMLoadFloat2(&v1.uv);
        // Compute the midpoints of all the attributes. 
        //Vectors need to be normalized since linear interpolating can make them not unit length.  
        DirectX::XMVECTOR pos      = DirectX::XMVectorScale(DirectX::XMVectorAdd(p0, p1), 0.5f);
        DirectX::XMVECTOR normal   = DirectX::XMVector3Normalize(DirectX::XMVectorScale(DirectX::XMVectorAdd(n0, n1), 0.5f));
        DirectX::XMVECTOR tangent  = DirectX::XMVector3Normalize(DirectX::XMVectorScale(DirectX::XMVectorAdd(tan0, tan1), 0.5f));
        DirectX::XMVECTOR uv       = DirectX::XMVectorScale(DirectX::XMVectorAdd(uv0, uv1), 0.5f);
        DirectX::XMVECTOR binormal = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(tangent, normal));
        TextureVertex            v;
        DirectX::XMStoreFloat3(&v.position, pos);
        DirectX::XMStoreFloat3(&v.normal, normal);
        DirectX::XMStoreFloat3(&v.tangent, tangent);
        DirectX::XMStoreFloat3(&v.binormal, binormal);
        DirectX::XMStoreFloat2(&v.uv, uv);
        return v;
    }

    void MeshGenerator::BuildCylinderTopCap(Real bottom_radius, Real top_radius, Real height, I32 slice_count, I32 stack_count, MeshData* mesh_data) const
    {
        E5_UNUSED_PARAM(stack_count);
        E5_UNUSED_PARAM(bottom_radius);
        I32  base_index = (I32)mesh_data->vertices.size();
        Real y          = 0.5f * height;
        Real dtheta     = 2.0f * DirectX::XM_PI / slice_count;
        // Duplicate cap ring vertices because the texture coordinates and normals differ.
        for (I32 i = 0; i <= slice_count; ++i)
        {
            Real x = top_radius * cosf(i * dtheta);
            Real z = top_radius * sinf(i * dtheta);
            // Scale down by the height to try and make top cap texture coordinate area
            // proportional to base.
            Real u = x / height + 0.5f;
            Real v = z / height + 0.5f;
            mesh_data->vertices.push_back(TextureVertex(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
        }
        // Cap center vertex.
        mesh_data->vertices.push_back(TextureVertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));
        // Index of center vertex.
        I32 center_index = (I32)mesh_data->vertices.size() - 1;
        for (I32 i = 0; i < slice_count; ++i)
        {
            mesh_data->indices.push_back(center_index);
            mesh_data->indices.push_back(base_index + i + 1);
            mesh_data->indices.push_back(base_index + i);
        }
    }

    void MeshGenerator::BuildCylinderBottomCap(Real bottom_radius, Real top_radius, Real height, I32 slice_count, I32 stack_count, MeshData* mesh_data) const
    {
        E5_UNUSED_PARAM(stack_count);
        E5_UNUSED_PARAM(top_radius);
        I32  base_index = (I32)mesh_data->vertices.size();
        Real y          = -0.5f * height;
        // vertices of ring
        Real dtheta = 2.0f * DirectX::XM_PI / slice_count;
        for (I32 i = 0; i <= slice_count; ++i)
        {
            Real x = bottom_radius * cosf(i * dtheta);
            Real z = bottom_radius * sinf(i * dtheta);
            // Scale down by the height to try and make top cap texture coordinate area
            // proportional to base.
            Real u = x / height + 0.5f;
            Real v = z / height + 0.5f;
            mesh_data->vertices.push_back(TextureVertex(x, y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
        }
        // Cap center vertex.
        mesh_data->vertices.push_back(TextureVertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));
        // Cache the index of center vertex.
        I32 center_index = (I32)mesh_data->vertices.size() - 1;
        for (I32 i = 0; i < slice_count; ++i)
        {
            mesh_data->indices.push_back(center_index);
            mesh_data->indices.push_back(base_index + i);
            mesh_data->indices.push_back(base_index + i + 1);
        }
    }

    void MeshGenerator::CalculateBinormal(MeshData& mesh_data) const
    {
        for (size_t i = 0; i < mesh_data.vertices.size(); ++i)
        {
            Vector3 tangent  = Converter::ToVector3(mesh_data.vertices[i].tangent);
            Vector3 normal   = Converter::ToVector3(mesh_data.vertices[i].normal);
            Vector3 binormal = tangent.CrossProduct(normal);
            binormal.SetNormalize();
            mesh_data.vertices[i].binormal.x = binormal.x;
            mesh_data.vertices[i].binormal.y = binormal.y;
            mesh_data.vertices[i].binormal.z = binormal.z;
        }
    }
}
