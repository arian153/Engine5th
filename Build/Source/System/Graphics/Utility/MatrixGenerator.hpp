#pragma once
#include "../../Math/Algebra/Matrix44.hpp"

namespace Engine5
{
    class MatrixGenerator
    {
    public:
        MatrixGenerator();
        ~MatrixGenerator();

        //Direct Create
        Matrix44 ProjectionMatrix(Real screen_aspect, Real field_of_view, Real far_plane, Real near_plane);
        Matrix44 OrthoGraphicMatrix(size_t client_width, size_t client_height, Real far_plane, Real near_plane);
        Matrix44 OrthoGraphicCenterMatrix(Real right, Real left, Real top, Real bottom, Real far_plane, Real near_plane);
        Matrix44 LookAt(const Vector3& eye_position, const Vector3& focus_position, const Vector3& up_direction) const;
        Matrix44 LookTo(const Vector3&  eye_position, const Vector3& eye_direction, const Vector3& up_direction) const;

        //Setter
        void SetScreenAspect(Real screen_aspect);
        void SetScreenAspect(size_t client_width, size_t client_height);
        void SetFieldOfView(Real field_of_view);
        void SetFarPlane(Real far_plane);
        void SetNearPlane(Real near_plane);
        void SetClientWidth(size_t client_width);
        void SetClientHeight(size_t client_height);
        void SetClientRect(size_t client_width, size_t client_height);
        void SetClientRect(Real right, Real left, Real top, Real bottom);

        //Getter
        Matrix44 GetProjectionMatrix() const;
        Matrix44 GetOrthoGraphicMatrix() const;

    private:
        Real m_right  = 0.0f;
        Real m_left   = 0.0f;
        Real m_top    = 0.0f;
        Real m_bottom = 0.0f;

        Real m_screen_aspect = 1.0f;
        Real m_field_of_view = Math::PI_DIV_4;

        Real m_far_plane     = 1000.f;
        Real m_near_plane    = 0.001f;
    };
}
