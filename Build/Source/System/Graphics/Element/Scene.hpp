#pragma once
#include "../../Math/Utility/MathDef.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"
#include "../Shader/ShaderType.hpp"
#include "../DataType/ProjectionType.hpp"

namespace Engine5
{
    class PrimitiveRenderer;
    class MatrixManager;
    class ShaderManager;
    class RendererCommon;
    class RendererDX11;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        void Initialize();
        void Update(Real dt) const;
        void Shutdown();

        void SetRenderer(RendererCommon* renderer);
        void SetShaderManager(ShaderManager* shader_manager);
        void SetMatrixManager(MatrixManager* matrix_manager);

        void DrawShader(eShaderType shader_type);
        void SetProjectionType(eProjectionType projection_type);
        void UpdateView() const;
        void UpdateProjection();

        PrimitiveRenderer* GetPrimitiveRenderer() const;

    private:
        RendererCommon*    m_renderer           = nullptr;
        ShaderManager*     m_shader_manager     = nullptr;
        MatrixManager*     m_matrix_manager     = nullptr;
        Camera*            m_camera             = nullptr;
        PrimitiveRenderer* m_primitive_renderer = nullptr;
        eProjectionType    m_projection_type    = eProjectionType::Perspective;
        Matrix44           m_projection_matrix;
    };
}
