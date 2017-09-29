#ifndef MAINOPENGLWIDGET_H
#define MAINOPENGLWIDGET_H

#include "emitter/mpemitter.h"
#include "emitter/emitterlist.h"
#include "emitter/b2emitter.h"
#include "tests.h"
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

class MainOpenGLWidget : protected QOpenGLWidget, protected QOpenGLFunctions
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
        return width();
    }

    int GetHeight()
    {
        return height();
    }



protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void timerEvent(QTimerEvent *e) override;

    void initializeMP();
    uint GetId();

    void keyPressEvent(QKeyEvent *e);

private:
    QBasicTimer timer;
    QMatrix4x4 m_projection;

    MP_Device *m_device;
    MP_Manager *m_manager;

    ShaderProgram m_program;
    QOpenGLFunctions m_functions;
    MainWidget *m_parent = Q_NULLPTR;

    QOpenGLPaintDevice *m_mainGLPaintDevice = nullptr;
    QPainter *m_painter = nullptr;

    TextureList m_textures;


//    QList<QOpenGLTexture *> m_textures;

    EmitterList *m_emitters;

    QList<uint> occupiedIdentificatos;

    Settings settings;

public:
    float *m_vertices;
    unsigned char *m_colors;
    float *m_textureCoords;
    unsigned short *m_indexes;

    DebugDraw g_debugDraw;
    Camera g_camera;

};

#endif // MAINOPENGLWIDGET_H
