#ifndef RENDERTRIANGLES_H
#define RENDERTRIANGLES_H

#include "camera.h"

#include <Box2D/Box2D.h>
#include <GL/gl.h>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

class RenderTriangles
{
public:

    RenderTriangles(Camera *camera);

    void Create(QOpenGLFunctions *functions);

    void Destroy();

    void Vertex(const b2Vec2 &v, const b2Color &c);

    void Flush();

    enum { e_maxVertices = 3 * 64 };
    b2Vec2 m_vertices[e_maxVertices];
    b2Color m_colors[e_maxVertices];

    int32 m_count;

    QOpenGLShaderProgram m_program;
    QOpenGLFunctions m_functions;

    Camera *g_camera;

    GLint m_projectionUniform;
    GLint m_vertexAttribute;
    GLint m_colorAttribute;
};

#endif // RENDERTRIANGLES_H
