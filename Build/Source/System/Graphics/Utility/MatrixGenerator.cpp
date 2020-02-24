#include "MatrixGenerator.hpp"

namespace Engine5
{
    MatrixGenerator::MatrixGenerator()
    {
    }

    MatrixGenerator::~MatrixGenerator()
    {
    }

    Matrix44 MatrixGenerator::ProjectionMatrix(Real screen_aspect, Real field_of_view, Real far_plane, Real near_plane)
    {
        SetScreenAspect(screen_aspect);
        SetFieldOfView(field_of_view);
        SetFarPlane(far_plane);
        SetNearPlane(near_plane);
        Real     alpha = field_of_view * 0.5f;
        Real     cot   = cosf(alpha) / sinf(alpha);
        Matrix44 result;
        result.data[0]  = cot / screen_aspect;
        result.data[5]  = cot;
        result.data[10] = far_plane / (far_plane - near_plane);
        result.data[11] = 1.0f;
        result.data[14] = -near_plane * far_plane / (far_plane - near_plane);
        result.data[15] = 0.0f;
        return result;
    }

    Matrix44 MatrixGenerator::OrthoGraphicMatrix(size_t client_width, size_t client_height, Real far_plane, Real near_plane)
    {
        SetClientRect(client_width, client_height);
        SetFarPlane(far_plane);
        SetNearPlane(near_plane);

        Real plane = 1.0f / (far_plane - near_plane);

        Matrix44 result;
        result.data[0]  = 2.0f / client_width;
        result.data[5]  = 2.0f / client_height;
        result.data[10] = plane;
        result.data[14] = plane * -near_plane;
        result.data[15] = 1.0f;
        return result;
    }

    Matrix44 MatrixGenerator::OrthoGraphicCenterMatrix(Real right, Real left, Real top, Real bottom, Real far_plane, Real near_plane)
    {
        SetClientRect(right, left, top, bottom);
        SetFarPlane(far_plane);
        SetNearPlane(near_plane);
        Matrix44 result;
        result.data[0]  = 2.0f / (right - left);
        result.data[5]  = 2.0f / (top - bottom);
        result.data[10] = 1.0f / (far_plane - near_plane);
        result.data[12] = (left + right) / (left - right);
        result.data[13] = (top + bottom) / (bottom - top);
        result.data[14] = near_plane / (near_plane - far_plane);
        result.data[15] = 1.0f;
        return result;
    }

    void MatrixGenerator::SetScreenAspect(Real screen_aspect)
    {
        m_screen_aspect      = screen_aspect;
        Real   client_height = m_top - m_bottom;
        Real   width         = screen_aspect * client_height;
        size_t client_width  = static_cast<size_t>(width);
        Real   half_x        = static_cast<Real>(client_width >> 1);
        m_right              = half_x;
        m_left               = -half_x;
    }

    void MatrixGenerator::SetScreenAspect(size_t client_width, size_t client_height)
    {
        m_screen_aspect = (Real)client_width / (Real)client_height;
        Real half_x     = static_cast<Real>(client_width >> 1);
        Real half_y     = static_cast<Real>(client_height >> 1);
        m_right         = half_x;
        m_left          = -half_x;
        m_top           = half_y;
        m_bottom        = -half_y;
    }

    void MatrixGenerator::SetFieldOfView(Real field_of_view)
    {
        m_field_of_view = field_of_view;
    }

    void MatrixGenerator::SetFarPlane(Real far_plane)
    {
        m_far_plane = far_plane;
    }

    void MatrixGenerator::SetNearPlane(Real near_plane)
    {
        m_near_plane = near_plane;
    }

    void MatrixGenerator::SetClientWidth(size_t client_width)
    {
        Real half_x     = static_cast<Real>(client_width >> 1);
        m_right         = half_x;
        m_left          = -half_x;
        m_screen_aspect = (Real)client_width / (m_top - m_bottom);
    }

    void MatrixGenerator::SetClientHeight(size_t client_height)
    {
        Real half_y     = static_cast<Real>(client_height >> 1);
        m_top           = half_y;
        m_bottom        = -half_y;
        m_screen_aspect = (m_right - m_left) / (Real)client_height;
    }

    void MatrixGenerator::SetClientRect(size_t client_width, size_t client_height)
    {
        Real half_x     = static_cast<Real>(client_width >> 1);
        Real half_y     = static_cast<Real>(client_height >> 1);
        m_right         = half_x;
        m_left          = -half_x;
        m_top           = half_y;
        m_bottom        = -half_y;
        m_screen_aspect = (Real)client_width / (Real)client_height;
    }

    void MatrixGenerator::SetClientRect(Real right, Real left, Real top, Real bottom)
    {
        m_right         = right;
        m_left          = left;
        m_top           = top;
        m_bottom        = bottom;
        m_screen_aspect = (m_right - m_left) / (m_top - m_bottom);
    }

    Matrix44 MatrixGenerator::GetProjectionMatrix() const
    {
        Real     alpha = m_field_of_view * 0.5f;
        Real     cot   = cosf(alpha) / sinf(alpha);
        Matrix44 result;
        result.data[0]  = cot / m_screen_aspect;
        result.data[5]  = cot;
        result.data[10] = m_far_plane / (m_far_plane - m_near_plane);
        result.data[11] = 1.0f;
        result.data[14] = -m_near_plane * m_far_plane / (m_far_plane - m_near_plane);
        result.data[15] = 0.0f;
        return result;
    }

    Matrix44 MatrixGenerator::GetOrthoGraphicMatrix() const
    {
        Real     r = static_cast<Real>(m_right);
        Real     l = static_cast<Real>(m_left);
        Real     t = static_cast<Real>(m_top);
        Real     b = static_cast<Real>(m_bottom);
        Matrix44 result;
        result.data[0]  = 2.0f / (r - l);
        result.data[5]  = 2.0f / (t - b);
        result.data[10] = 2.0f / (m_far_plane - m_near_plane);
        result.data[12] = (l + r) / (l - r);
        result.data[13] = (t + b) / (b - t);
        result.data[14] = -m_near_plane / (m_far_plane - m_near_plane);
        result.data[15] = 1.0f;
        return result;
    }
}
