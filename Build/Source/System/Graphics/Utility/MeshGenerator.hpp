#pragma once
#include "Vertex.hpp"
#include <vector>
#include "../../Core/Utility/CoreDef.hpp"

namespace Engine5
{
    class MeshData
    {
    public:
        MeshData();
        ~MeshData();

    public:
        std::vector<Vertex> vertices;
        std::vector<I32>    indices;
        bool                is_resource = false;
    };

    class MeshGenerator
    {
    public:
        MeshGenerator();
        ~MeshGenerator();

        MeshData* CreateBox(Real width, Real height, Real depth, I32 num_subdivisions) const;
        MeshData* CreateSphere(Real radius, I32 slice_count, I32 stack_count) const;
        MeshData* CreateSphereWithSubdivision(Real radius, I32 subdivisions) const;
        MeshData* CreateCylinder(Real bottom_radius, Real top_radius, Real height, I32 slice_count, I32 stack_count) const;
        MeshData* CreateGrid(Real width, Real depth, I32 m, I32 n) const;
        MeshData* CreateQuad(Real x, Real y, Real w, Real h, Real depth) const;

    private:
    private:
        void   Subdivide(MeshData& mesh_data) const;
        Vertex MidPoint(const Vertex& v0, const Vertex& v1) const;
        void   BuildCylinderTopCap(Real bottom_radius, Real top_radius, Real height, I32 slice_count, I32 stack_count, MeshData* mesh_data) const;
        void   BuildCylinderBottomCap(Real bottom_radius, Real top_radius, Real height, I32 slice_count, I32 stack_count, MeshData* mesh_data) const;
        void   CalculateBinormal(MeshData& mesh_data) const;
    };
}
