#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include "../../Math/Utility/MathDef.hpp"
#include "../DataType/Vertex/TextureVertex.hpp"

namespace Engine5
{
    class MeshData;
    class MeshGenerator
    {
    public:
        MeshGenerator();
        ~MeshGenerator();

        MeshData* CreateBox(Real width, Real height, Real depth, I32 num_subdivisions) const;
        MeshData* CreateSphere(Real radius, I32 slice_count, I32 stack_count) const;
        MeshData* CreateGeodesicSphere(Real radius, I32 subdivisions) const;
        MeshData* CreateCylinder(Real bottom_radius, Real top_radius, Real height, I32 slice_count, I32 stack_count) const;
        MeshData* CreateGrid(Real width, Real depth, I32 m, I32 n) const;
        MeshData* CreateQuad(Real x, Real y, Real w, Real h, Real depth) const;

    private:
        void   Subdivide(MeshData& mesh_data) const;
        TextureVertex MidPoint(const TextureVertex& v0, const TextureVertex& v1) const;
        void   BuildCylinderTopCap(Real bottom_radius, Real top_radius, Real height, I32 slice_count, I32 stack_count, MeshData* mesh_data) const;
        void   BuildCylinderBottomCap(Real bottom_radius, Real top_radius, Real height, I32 slice_count, I32 stack_count, MeshData* mesh_data) const;
        void   CalculateBinormal(MeshData& mesh_data) const;
    };

}
