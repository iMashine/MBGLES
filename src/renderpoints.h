#ifndef RENDERPOINTS_H
#define RENDERPOINTS_H

#include "camera.h"

#include <Box2D/Box2D.h>
#include <GL/gl.h>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class RenderPoints
{
public:

    RenderPoints(Camera *camera);

    void Create(QOpenGLFunctions *functions);

    void Destroy();

    void Vertex(const b2Vec2 &v, const b2Color &c, float32 size);

    void Flush();

    enum { e_maxVertices = 64 };
    b2Vec2 m_vertices[e_maxVertices];
    b2Color m_colors[e_maxVertices];
    float32 m_sizes[e_maxVertices];

    int32 m_count;


    QOpenGLShaderProgram m_program;
    QOpenGLFunctions m_functions;

    Camera *g_camera;

    GLint m_projectionUniform;

    GLint m_vertexAttribute;
    GLint m_colorAttribute;
    GLint m_sizeAttribute;
};

#endif // RENDERPOINTS_H
