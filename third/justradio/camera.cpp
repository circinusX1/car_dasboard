//----------------------------------------------------------------------------------------
// Unauthorized use or duplication is strictly prohibited.
// Copyright Zalsoft Inc 1999-2004.
//// Author: Marius C of Zalsoft Inc 2000-2004
//----------------------------------------------------------------------------------------

#include "camera.h"


//--| pos, real_t yaw, real_t pich, real_t rol|-------------------------------------------------
Camera::Camera(v3d_t& pos, real_t yaw, real_t pich, real_t rol)
{
    SetAngles(yaw, pich, rol);
    _pos          = pos;
    _prevfov      = 0;
    _prevrs       = 0;
    _fov		  = 64;
}

//--|Camera::Camera|----------------------------------------------------------------------
Camera::Camera()
{
    _prevrs = 0;
    _fov    = 64;
}

//--| m_16t Camera::GetViewMtx|--------------------------------------------------------------
const m_16t Camera::GetViewMtx(real_t dTime)
{
    Q_UNUSED(dTime);
    return MLook(_pos, _fwd, _up, _right);
}

//--| void Camera::AddOccluder|--------------------------------------------------------------
void Camera::AddOccluder(const Poly& p)
{


}

