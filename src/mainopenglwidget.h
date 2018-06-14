#ifndef MAINOPENGLWIDGET_H
#define MAINOPENGLWIDGET_H

#include "emitter/mpemitter.h"
#include "emitter/emitterlist.h"
#include "emitter/b2emitter.h"
#include "mainopengloffscreensurface.h"
#include "mainwidget.h"
#include "shaderprogram.h"

#include <QBasicTimer>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QPainter>
#include <QOpenGLPaintDevice>

#include <QDebug>

typedef unsigned int uint;

class MainOpenGLWidget : public MainOpenGLOffscreenSurface
{
public:
    MainOpenGLWidget(MainWidget *parent = Q_NULLPTR);
    ~MainOpenGLWidget();

    EmitterList *GetEmitters();

    void Refresh();

    void ClearBuffers();

    void Draw(int starting_index, int indexes_count, int max_vertices);

    int GetWidth()
    {
        return bufferSize().width();
    }

    int GetHeight()
    {
        return bufferSize().height();
    }

    void Update();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void timerEvent(QTimerEvent *e);

    void initializeMP();
    uint GetId();

    void keyPressEvent(QKeyEvent *e);

private:
    QMatrix4x4 m_projection;

    MP_Device *m_device;
    MP_Manager *m_manager;

    ShaderProgram m_program;
    MainWidget *m_parent = Q_NULLPTR;

    QOpenGLPaintDevice *m_mainGLPaintDevice = nullptr;
    QPainter *m_painter = nullptr;

    TextureList m_textures;

    EmitterList *m_emitters;

    QList<uint> occupiedIdentificatos;

    Settings settings;

    bool m_isInitialized = false;

public:
    float *m_vertices;
    unsigned char *m_colors;
    float *m_textureCoords;
    unsigned short *m_indexes;

    DebugDraw g_debugDraw;
    Camera *g_camera;
};

#endif // MAINOPENGLWIDGET_H
