#include "MatrixManager.hpp"
#include "../../Math/Algebra/Vector3.hpp"
#include "../../Math/Utility/MatrixUtility.hpp"

namespace Engine5
{
    MatrixManager::MatrixManager()
    {
    }

    MatrixManager::~MatrixManager()
    {
    }

    void MatrixManager::SetScreenAspect(Real screen_aspect)
    {
        m_screen_aspect      = screen_aspect;
        Real   client_height = m_top - m_bottom;
        Real   width         = screen_aspect * client_height;
        size_t client_width  = static_cast<size_t>(width);
        Real   half_x        = static_cast<Real>(client_width >> 1);
        m_right              = half_x;
        m_left               = -half_x;
    }

    void MatrixManager::SetScreenAspect(size_t client_width, size_t client_height)
    {
        m_screen_aspect = (Real)client_width / (Real)client_height;
        Real half_x     = static_cast<Real>(client_width >> 1);
        Real half_y     = static_cast<Real>(client_height >> 1);
        m_right         = half_x;
        m_left          = -half_x;
        m_top           = half_y;
        m_bottom        = -half_y;
    }

    void MatrixManager::SetFieldOfView(Real field_of_view)
    {
        m_field_of_view = field_of_view;
    }

    void MatrixManager::SetFarPlane(Real far_plane)
    {
        m_far_plane = far_plane;
    }

    void MatrixManager::SetNearPlane(Real near_plane)
    {
        m_near_plane = near_plane;
    }

    void MatrixManager::SetClientWidth(size_t client_width)
    {
        Real half_x     = static_cast<Real>(client_width >> 1);
        m_right         = half_x;
        m_left          = -half_x;
        m_screen_aspect = (Real)client_width / (m_top - m_bottom);
    }

    void MatrixManager::SetClientHeight(size_t client_height)
    {
        Real half_y     = static_cast<Real>(client_height >> 1);
        m_top           = half_y;
        m_bottom        = -half_y;
        m_screen_aspect = (m_right - m_left) / (Real)client_height;
    }

    void MatrixManager::SetClientRect(size_t client_width, size_t client_height)
    {
        Real half_x     = static_cast<Real>(client_width >> 1);
        Real half_y     = static_cast<Real>(client_height >> 1);
        m_right         = half_x;
        m_left          = -half_x;
        m_top           = half_y;
        m_bottom        = -half_y;
        m_screen_aspect = (Real)client_width / (Real)client_height;
    }

    void MatrixManager::SetClientRect(Real right, Real left, Real top, Real bottom)
    {
        m_right         = right;
        m_left          = left;
        m_top           = top;
        m_bottom        = bottom;
        m_screen_aspect = (m_right - m_left) / (m_top - m_bottom);
    }

    Matrix44 MatrixManager::GetProjectionMatrix() const
    {
        return Math::Matrix44::PerspectiveMatrix(m_screen_aspect, m_field_of_view, m_far_plane, m_near_plane);
    }

    Matrix44 MatrixManager::GetOrthoGraphicMatrix() const
    {
        return Math::Matrix44::OrthoGraphicCenterMatrix(m_right, m_left, m_top, m_bottom, m_far_plane, m_near_plane);
    }
}
