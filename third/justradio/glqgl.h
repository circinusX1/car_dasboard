#ifndef QGLWIDGET_H
#define QGLWIDGET_H

#include <vector>
#include <QObject>
#include <QWidget>
#include <QtOpenGL/QGLWidget>
#include <GL/glut.h>
#include <GL/glext.h>
#include <QtOpenGL>
#include "camera.h"

typedef	u_int8_t		RGBA[4];
typedef real_t           real_t;

struct AcordPt
{
    real_t x;
    real_t y;
    int    hertz;
    std::string f;
};

struct FRECT
{
    real_t left;
    real_t top;
    real_t right;
    real_t bottom;
};

class GlQGl : public QGLWidget
{
    Q_OBJECT
public:
    explicit GlQGl(QWidget* parent=0,
                   const QGLWidget* shareWidget = 0, Qt::WindowFlags f=0);
    void    SelectTextCtx(const QRect& wrect);
    void    DeSelectTextCtx();


    void mouseMoveEvent( QMouseEvent *e );
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    void initializeGL();
    void resizeGL(int32_t width, int32_t height);
    void paintGL();
    void Project3D(int32_t iWidth, int32_t iHeight);
    void GLPaint();
    void Reinit();
    void DrawInHrc(int update);
    void SetFaceMode();
    void Clear();
    void Draw3DScene();
    void Replay(int sens);
    void TextOut(double x, double y,  const char *str);
    void setWidth(int width){
        _gain.clear();
        _width=width;
        float tw =(float)this->width();
        _scalex = 2.6f;//tw/(float)-1.0;
        update();
    };
    void tune(int freq)
    {
        sprintf(_tune,"%d", freq);
    }
    void addGain(int gain, const char* txt, int hz)
    {
        AcordPt xy;
        xy.hertz = 0;
        xy.x = _gain.size();
        float scale = this->height()/(3.0f*12.0f);
        xy.y = this->height()-(this->height()/3.0f + gain * scale);
        if(*txt){
            xy.f=txt;
            xy.hertz = hz;
        }
        _gain.push_back(xy);
    }
private:
    void InitGL();
    void DrawGridLines();
    void DrawAxis( real_t ex);
    void Project2D();
    void Draw2D(bool);
signals:

public slots:
private:
    QRect   _rt;
    real_t   _ffar;
    real_t   _fov;
    real_t   _fnear;
    Camera  _cam;
    int     _gridSize=1000;
    int     _gridStep=10;
    int     _step = 0;
    real_t   _zoom=1.0f;
    real_t   _shiftx = 0.0f;
    real_t   _shifty = 0.0f;
    FRECT   _vport;
public:
    real_t       _scaley = 1.0f;
    real_t       _scalex = 1.0f;
    real_t       _scalesample=1.0;
    int         _show = (0x7 | 0x100);
    int         _width = 0;
    char        _tune[128];
    QPoint      _mdn;
    bool        _mbdn=false;
    std::vector<AcordPt> _gain;
};

extern GlQGl* PGL;

struct GCur
{
    GlQGl* _pv;
    GCur(GlQGl* pv):_pv(pv){
        if(_pv)
            _pv->makeCurrent();
    }

    ~GCur(){
        if(_pv)
            _pv->doneCurrent();
    }
};

#ifndef NDEBUG
#define GL(_f)                    \
  do {                            \
    gl##_f;                       \
    GLenum error = glGetError();  \
    assert(error == GL_NO_ERROR); \
                                  \
  } while (void(0), 0)
#else  // NDEBUG
#define GL(_f) gl##_f
#endif  // NDEBUG



#endif // QGLWIDGET_H
