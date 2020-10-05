// Frustrum.cpp: implementation of the Frustrum class.
//
//////////////////////////////////////////////////////////////////////

#include "camera.h"
#include "frustum.h"

void Frustrum::UpdateFrustrumPlanes(v3d_t& pos, v3d_t& ldir, v3d_t& lup, v3d_t& lside)
{
    _ihulls=0;
    _viewfdots[0] = pos;
#ifdef WINDOWS
    #pragma message ("frustrum may be build by trmatrix from modelview")
#endif
    // this goes with 640/480 and 60 degrees
    real_t heightFar = _farZ * 0.6713f;
    real_t widthFar  = _farZ * 0.8820f;

    // far translation matrix. where to translate far dots
    real_t tfx = _farZ * ldir.x;
    real_t tfy = _farZ * ldir.y;
    real_t tfz = _farZ * ldir.z;
    real_t wfx = widthFar*lside.x;
    real_t wfy = widthFar*lside.y;
    real_t wfz = widthFar*lside.z;
    real_t hfx = heightFar*lup.x;
    real_t hfy = heightFar*lup.y;
    real_t hfz = heightFar*lup.z;

    _viewfdots[4].x = pos.x + tfx + wfx + hfx;
    _viewfdots[4].y = pos.y + tfy + wfy + hfy;
    _viewfdots[4].z = pos.z + tfz + wfz + hfz;
    _viewfdots[3].x = pos.x + tfx + wfx - hfx;
    _viewfdots[3].y = pos.y + tfy + wfy - hfy;
    _viewfdots[3].z = pos.z + tfz + wfz - hfz;
    _viewfdots[2].x = pos.x + tfx - wfx - hfx;
    _viewfdots[2].y = pos.y + tfy - wfy - hfy;
    _viewfdots[2].z = pos.z + tfz - wfz - hfz;
    _viewfdots[1].x = pos.x + tfx - wfx + hfx;
    _viewfdots[1].y = pos.y + tfy - wfy + hfy;
    _viewfdots[1].z = pos.z + tfz - wfz + hfz;

    AddFrustrumPlan(ldir, pos);				                // near
    AddFrustrumPlan(_viewfdots[1],_viewfdots[2],_viewfdots[3]); // far
    AddFrustrumPlan(_viewfdots[1],_viewfdots[0],_viewfdots[2]); // left
    AddFrustrumPlan(_viewfdots[4],_viewfdots[3],_viewfdots[0]); // right
    AddFrustrumPlan(_viewfdots[3],_viewfdots[2],_viewfdots[0]); // down
    AddFrustrumPlan(_viewfdots[1],_viewfdots[4],_viewfdots[0]); // up
}

u_bool32    Frustrum::IsCulled(const v3d_t* am)
{
    int32_t i;
    for(i=0; i < 6; i++)
    {
        if(_hulls[i].DistTo(am->x,am->y,am->z) > 0) continue;
            return true;
    }

    for(;i<_ihulls; i++)
    {
        if(_hulls[i].DistTo(am->x,am->y,am->z) > 0) continue;
            return true;
    }

    return false;
}


u_bool32	Frustrum::IsCulled(const v3d_t* am, const v3d_t* aM)
{
    int32_t i;
    for( i=0; i<6; i++)
    {
        if(_hulls[i].DistTo(am->x,am->y,am->z) > 0) continue;
        if(_hulls[i].DistTo(aM->x,am->y,am->z) > 0) continue;
        if(_hulls[i].DistTo(am->x,aM->y,am->z) > 0) continue;
        if(_hulls[i].DistTo(aM->x,aM->y,am->z) > 0) continue;
        if(_hulls[i].DistTo(am->x,am->y,aM->z) > 0) continue;
        if(_hulls[i].DistTo(aM->x,am->y,aM->z) > 0) continue;
        if(_hulls[i].DistTo(am->x,aM->y,aM->z) > 0) continue;
        if(_hulls[i].DistTo(aM->x,aM->y,aM->z) > 0) continue;
        return true;
    }

    for(;i<_ihulls; i++)
    {
        if(_hulls[i].DistTo(am->x,am->y,am->z) > 0) continue;
        if(_hulls[i].DistTo(aM->x,am->y,am->z) > 0) continue;
        if(_hulls[i].DistTo(am->x,aM->y,am->z) > 0) continue;
        if(_hulls[i].DistTo(aM->x,aM->y,am->z) > 0) continue;
        if(_hulls[i].DistTo(am->x,am->y,aM->z) > 0) continue;
        if(_hulls[i].DistTo(aM->x,am->y,aM->z) > 0) continue;
        if(_hulls[i].DistTo(am->x,aM->y,aM->z) > 0) continue;
        if(_hulls[i].DistTo(aM->x,aM->y,aM->z) > 0) continue;
        return true;

    }


    return false;
}
