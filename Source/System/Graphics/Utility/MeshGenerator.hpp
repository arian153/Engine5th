#pragma once
#include "../../Core/Utility/CoreDef.hpp"
#include "../../Math/Utility/MathDef.hpp"
#include "../Common/Vertex/NormalVertexCommon.hpp"

namespace Engine5
{
    class MeshData;
    class MeshGenerator
    {
    public:
        MeshGenerator();
        ~MeshGenerator();

        MeshData* CreateBox(Real width, Real height, Real depth, U32 num_subdivisions) const;
        MeshData* CreateSphere(Real radius, U32 slice_count, U32 stack_count) const;
        MeshData* CreateGeodesicSphere(Real radius, U32 subdivisions) const;
        MeshData* CreateCylinder(Real bottom_radius, Real top_radius, Real height, U32 slice_count, U32 stack_count) const;
        MeshData* CreateGrid(Real width, Real depth, U32 m, U32 n) const;
        MeshData* CreateQuad(Real x, Real y, Real w, Real h, Real depth) const;

    private:
        void   Subdivide(MeshData& mesh_data) const;
        NormalVertexCommon MidPoint(const NormalVertexCommon& v0, const NormalVertexCommon& v1) const;
        void   BuildCylinderTopCap( Real top_radius, Real height, U32 slice_count, MeshData& mesh_data) const;
        void   BuildCylinderBottomCap(Real bottom_radius, Real height, U32 slice_count, MeshData& mesh_data) const;
        void   CalculateBinormal(MeshData& mesh_data) const;
    };

}
