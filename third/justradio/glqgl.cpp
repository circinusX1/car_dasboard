#include "glqgl.h"
#include "dialog.h"

RGBA	CLR_WHITE	= {255,255,255,255};
#define SCALE_Y     0.01
#define SCALE_X     1.0

GlQGl* PGL;

GlQGl::GlQGl(QWidget* parent,
                     const QGLWidget* shareWidget, Qt::WindowFlags f) : QGLWidget(parent)
{
    _ffar = 35535;
    _fov = 64;
    _fnear = 1.0f;

    _tune[0]=0;

    _cam.SetFov(2*PIPE6);
    _cam.SetFar(24800.0);//250m
    _cam.SetAngles(-PIPE4,-PIPE6,0);
    _cam.Euler2Pos();
    _cam._pos = v3d_t(1600.0f,1300.0f,1800.0f);
    _cam.Rotate(0,G2R(5),0);
    installEventFilter(this);
    PGL = this;
}

void GlQGl::mouseMoveEvent( QMouseEvent *e )
{
    if(_gain.size())
    {
        float xscale = (float)_gain.size() / (float)this->width();
        int   xindex = (float)e->x() * xscale;
        const AcordPt& fp  = _gain[xindex];
        if(fp.hertz)
        {
            PDLG->apply_freq(fp.hertz);
        }
    }
    _mdn = e->pos();
    update();
    QGLWidget::mouseMoveEvent(e);
}

void GlQGl::mousePressEvent(QMouseEvent *e)
{
    _mdn = e->pos();
    _mbdn = true;
    update();
    QGLWidget::mousePressEvent(e);
}

void GlQGl::mouseReleaseEvent(QMouseEvent *e)
{
    _mbdn = false;

   // if(abs(e->x()-_mdn.x())>5)
    {
        float xscale = (float)_gain.size() / (float)this->width();
        int   xindex = (float)e->x() * xscale;
        const AcordPt& fp  = _gain[xindex];
        _tune[0] = 0;
        if(fp.f.empty())
        {
            int j=1;
            while(xindex-j > 0 && xindex+j<_gain.size())
            {
                const AcordPt& fp  = _gain[xindex-j];
                if(!fp.f.empty())
                {
                    ::strcpy(_tune,fp.f.c_str());
                    PDLG->apply_freq(fp.hertz);
                    break;
                }
                const AcordPt& fp2  = _gain[xindex+j];
                if(!fp2.f.empty())
                {
                    ::strcpy(_tune,fp2.f.c_str());
                    PDLG->apply_freq(fp2.hertz);
                    break;
                }
                ++j;
            }
        }
        else {
            ::strcpy(_tune,fp.f.c_str());
            PDLG->apply_freq(fp.hertz);
        }
        update();

    }
    QGLWidget::mouseReleaseEvent(e);
}

void GlQGl::mouseDoubleClickEvent(QMouseEvent *e)
{
    _mdn = e->pos();
    QGLWidget::mouseDoubleClickEvent(e);
}

void    GlQGl::SelectTextCtx(const QRect& wrect)
{
    glPushMatrix();
        glLoadIdentity();
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(wrect.left(), wrect.right(), wrect.top() , wrect.bottom(), -32768, 32768);
        glColor3ub( 255,255,0);
    glMatrixMode(GL_MODELVIEW);

}

void    GlQGl::DeSelectTextCtx()
{
    glPopMatrix();
}


void GlQGl::initializeGL()
{
    GCur g(this);
    InitGL();

    /*
    qglClearColor(QColor(0,55,0));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
*/

    int iw = this->width();
    float sy = (float)this->height()/(float)this->width();
    float yc = 0;
    _width = iw;
    for(int i =0; i<this->width();i++)
    {
        AcordPt p;
        p.x=i;
        p.y=yc;
        yc+=sy;
        _gain.push_back(p);
    }
}

void GlQGl::resizeGL(int32_t cx, int32_t cy)
 {
    GCur g(this);
    Project3D(cx, cy);
    _rt = this->rect();
    if(_rt.bottom()==0)
        _rt.setBottom(1);

 }

void GlQGl::paintGL()
{
    GCur g(this);
    DrawInHrc(true);//GLPaint();

}



void GlQGl::DrawInHrc(int update)
{
    Q_UNUSED(update);
    //Ta.Enable();
    //glDisable(GL_COLOR_MATERIAL);

    _rt = rect();
    if(_rt.right() && _rt.bottom())
    {
        Project3D(_rt.right(), _rt.bottom());
        glClear (GL_COLOR_BUFFER_BIT|
                 GL_DEPTH_BUFFER_BIT|
                 GL_STENCIL_BUFFER_BIT);
        GLPaint();

        Project2D();
        Draw2D(false);
        swapBuffers();
        if(update)
            this->update();
    }
    //Ta.Disable();
}

void GlQGl::InitGL()
{
/*?*
    PIXELFORMATDESCRIPTOR  pfd = {0};
    for(int32_t i=0;i<8;i++)
    {
        DescribePixelFormat(hDC, i,  sizeof(PIXELFORMATDESCRIPTOR), &pfd);
        if((pfd.dwFlags & (PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER)) &&
           pfd.iLayerType  == PFD_MAIN_PLANE)
        {
            break;
        }
    }
*/
#ifdef WINDOWS

    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize       = sizeof(pfd);
    pfd.nVersion    = 1;
    pfd.dwFlags     = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType  = PFD_TYPE_RGBA;
    pfd.cColorBits  = 24;
    pfd.cDepthBits  = 8;
    pfd.cStencilBits= 16;
    pfd.iLayerType  = PFD_MAIN_PLANE;


    int32_t iFormat     = ChoosePixelFormat(hDC, &pfd);

    PIXELFORMATDESCRIPTOR pfd_new;
    DescribePixelFormat (hDC, iFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd_new);

    int32_t generic_format = pfd_new.dwFlags & PFD_GENERIC_FORMAT;
    int32_t generic_accelerated = pfd_new.dwFlags & PFD_GENERIC_ACCELERATED;

    if (generic_format && ! generic_accelerated)
    {
        TAfxMessageBox("Cannot run without hardware acceleration Video Driver", MB_OK|MB_ICONHAND);
//        return 0;
    }
    SetPixelFormat(hDC, iFormat, &pfd);

    hrc = wglCreateContext(hDC);

    MakeCurrent(hDC, hrc);
#else

#endif

      glShadeModel(GL_SMOOTH);
      glEnable(GL_DEPTH_TEST);
      glDepthFunc(GL_LESS);
      glDepthMask(GL_TRUE);
      glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
      glEnable(GL_COLOR_MATERIAL);
      glClearDepth( 1 );
      glDisable(GL_FOG);//
      glDisable(GL_DITHER);
      glEnable(GL_ALPHA_TEST);
      glClearColor(0.0f,0.0f,0.0f,0.f);
      SetFaceMode();

/*

     //SetLightMode(0);
     glPolygonMode(GL_BACK,GL_FILL);
     glFrontFace(GL_CW);
     glEnable(GL_CULL_FACE);
     glCullFace(GL_FRONT);
*/

}

void GlQGl::SetFaceMode()
{
    {
        glColor4ubv(CLR_WHITE);
        glDisable(GL_FOG);
        glDisable(GL_LIGHTING);
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);
    }


    glDisable(GL_CULL_FACE);
    //glFrontFace(GL_CCW);
    //glCullFace(GL_FRONT_AND_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void  GlQGl::Project3D(int32_t iWidth, int32_t iHeight)
{
    //GCur g(this);

    if(iHeight == 0)
        iHeight = 1;


    real_t farCam = _ffar;
/*
    if(SCENE()._si.fogFar)
        farCam = SCENE()._si.fogFar*_ffar;
*/

    glViewport(0, 0, iWidth, iHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // sudo apt-get install freeglut3 freeglut3-dev
    gluPerspective(_fov,(GLfloat)iWidth/(GLfloat)iHeight, _fnear, farCam);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


//	MakeCurrent(_hdc, 0);
}


void GlQGl::GLPaint()
{

    glPushMatrix();
        Camera& cam = _cam;
        const m_16t&		mt = cam.GetViewMtx(0);
        cam.UpdateFrustrumPlanes();
        glColor3f(1.0, 1.0f, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();


        glMultMatrixf((real_t*)&mt);
        //glScalef(1,-1,1); // SWAP2
        //glRotatef(90,1,0,0); // SWAP-1
        real_t axis_map[][3] = {{0, -1, 0}, {0, 0, 1}, {1, 0, 0}};

        glRotatef(90, axis_map[2][0], axis_map[2][1], axis_map[2][2]);


        Draw3DScene();

    glPopMatrix();
}


void GlQGl::Draw3DScene()
{

    SetFaceMode();
    glDisable(GL_BLEND);
    //DrawAxis(1500);
    glPushMatrix();

    glPopMatrix();
//
}


void GlQGl::DrawAxis( real_t ex)
{
    glPushMatrix();

    glTranslatef(0,0,0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    glBegin(GL_LINES);
        glColor4ub(255,96,96,128);
            glVertex3f(0,0,0);
            glVertex3f(ex,.6f,0.f);

        glColor4ub(96,255,96,128);
            glVertex3f(0,0,0);
            glVertex3f(0.f,ex,0.f);

        glColor4ub(96,96,255,128);
            glVertex3f(0,0,0);
            glVertex3f(0.f,.1f,ex);

        glColor4ub(128,0,0,128);
            glVertex3f(0,0,0);
            glVertex3f(-ex,.6f,0.f);

        glColor4ub(0,128,0,128);
            glVertex3f(0,0,0);
            glVertex3f(0.f,-ex,0.f);

        glColor4ub(0,0,128,128);
            glVertex3f(0,0,0);
            glVertex3f(0.f,0.f,-ex);
    glEnd();

    glPopMatrix();
}

//---------------------------------------------------------------------------------------
void GlQGl::DrawGridLines()
{
    real_t r;

    glColor4ub(53,53,53,64);

    real_t gs = _gridStep*10;

    glBegin(GL_LINES);
    for( r = -_gridSize/2; r< _gridSize/2; r+=gs)
    {
        glVertex3f(r,0,-_gridSize/2);
        glVertex3f(r,0,_gridSize/2);
    }

    for(r = -_gridSize/2; r< _gridSize/2; r+=gs)
    {
        glVertex3f(-_gridSize/2,0,r);
        glVertex3f(_gridSize/2,0,r);
    }

    glEnd();
    glEnable(GL_DEPTH_TEST);
    glDepthMask(1);
    glDisable(GL_BLEND);
}

void GlQGl::Clear()
{

}

void GlQGl::Project2D()
{
    int32_t iWidth = this->width();
    int32_t iHeight = this->height();
    if(iHeight == 0)
    {
        iHeight = 1;
    }

    glViewport(0, 0, iWidth, iHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    _vport.left   = -(((real_t)iWidth * 0.5)   / _zoom)   + _shiftx;
    _vport.right  =  (((real_t)iWidth * 0.5)   / _zoom)   + _shiftx;
    _vport.top    = -(((real_t)iHeight * 0.5)  / _zoom)   + _shifty;
    _vport.bottom  = (((real_t)iHeight * 0.5)  / _zoom)   + _shifty;
    glOrtho(_vport.left, _vport.right, _vport.top , _vport.bottom, -131072, 131072);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void GlQGl::Draw2D(bool tracing)
{
    glPushMatrix();
    glTranslatef(-this->width()/2,-this->height()/2,0);



    glScalef(_scalex,_scaley,-1.0f);

    int tune = 0;
    glBegin(GL_LINE_STRIP);
    for(const auto& s : _gain)
    {
        glColor4ub(255,s.y>255.0f?255:s.y,0,0);
        glVertex2f(s.x, s.y);
        if(_tune[0] && s.f.find(_tune)!=std::string::npos)
        {
            tune = s.x;
        }
    }
    glEnd();

    for(const auto& s : _gain)
    {
        if(s.x==tune)
        {
            glColor4ub(255,255,255,128);
            glBegin(GL_LINES);
                glVertex2f(tune, 0);
                glVertex2f(tune, this->height());
            glEnd();
        }
        if(!s.f.empty())
        {
            glColor4ub(0,255,255,128);
            glLineWidth(4);
            glBegin(GL_LINES);
                glVertex2f(s.x, 0);
                glVertex2f(s.x, this->height()/4);
            glEnd();
            glLineWidth(1);
        }
    }

    glPopMatrix();

    if(_mbdn)
    {
        glPushMatrix();
        glTranslatef(-this->width()/2,-this->height()/2,0);

        glLineWidth(3);
        glColor4ub(0,255,0,0);
        glBegin(GL_LINES);
            glVertex2f(_mdn.x(), 10);
            glVertex2f(_mdn.x(), this->height()-10);
        glEnd();
        glLineWidth(1);


        glColor4ub(128,128,128,0);
       glBegin(GL_LINES);
           glVertex2f(0, this->height()/3-1);
           glVertex2f(this->width(), this->height()/3-1);
       glEnd();


        glPopMatrix();
    }



    int off[] = {0,12,24,36};
    int idx=0;
    glColor4ub(0,0,255,255);
    for(const auto& s : _gain)
    {
        if(!s.f.empty()){
            TextOut(s.x+1,20+off[idx++%4],s.f.c_str());
        }
    }
}

void GlQGl::TextOut(double x, double y,  const char *str)
{
    glPushMatrix();

    glRasterPos3f(x*_scalex-this->width()/2, y-this->height()/2, 0);

    void* font = GLUT_BITMAP_HELVETICA_12;
    int i;
    int n = strlen(str);
    for (i = 0; i < n; i++){
        glutBitmapCharacter(font, str[i]);
    }
    glPopMatrix();
}
