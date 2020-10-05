// Camera.h: interface for the Camera class.
//The code has strict tutorial purposes ans shall be used only at home
//Author: Marius Chincisan (Zalsoft Inc. 2000-2002)
//
// Started: Sept 2001
//---------------------------------------------------------------------------------------
#ifndef __CAMERA_H__
#define __CAMERA_H__


#include "frustum.h"

//---------------------#include "stdafx.h"------------------------------------------------------------------
class Poly;
class Camera : public Pos
{
public:
    Camera();
    Camera(v3d_t& eye, real_t a = 0.0f, real_t e = 0.0f, real_t r = 0.0f);
    virtual ~Camera(){};
//ICamera
    void	PosCamera(v3d_t& pos, v3d_t& dir, v3d_t& up){
        _pos = pos;
        _fwd = dir;
        _up  = up;         // get to these
    }
    void	GetCamera(v3d_t& pos, v3d_t& dir, v3d_t& up){
        pos = _pos;
        dir = _fwd;
        up  = _up;
    }
    const m_16t	GetViewMtx(real_t dTime);
    void		SetFar(real_t ffar){_frustrum._farZ = ffar; }
    void		At(v3d_t& v){_fwd = (v-_pos).norm();}
    u_bool32		IsCulled(const v3d_t* aabbMin, const v3d_t* aabbMax){return _frustrum.IsCulled(aabbMin, aabbMax);}
    u_bool32		IsCulled(const v3d_t* point){return _frustrum.IsCulled(point);}
    real_t		GetFov(){return _fov ;}
    void		SetFov(real_t fov)   {
        _fov = fov;
    }
    void        AddOccluder(const Poly& p);
    void        UpdateFrustrumPlanes(){_frustrum.UpdateFrustrumPlanes(_pos, _fwd, _up, _right); }
private:
    Frustrum            _frustrum;
    real_t		        _fov;           // save here prev frustrum props
    real_t		        _prevfov;
    real_t		        _fowSaved;
    real_t		        _prevrs;
};


#endif // !__CAMERA_H__
