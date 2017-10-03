#include "debugdraw.h"

DebugDraw::DebugDraw()
{
    m_triangles = NULL;
}

//
DebugDraw::~DebugDraw()
{
}

void DebugDraw::Create(Camera *camera, QOpenGLFunctions *functions)
{
    m_triangles = new RenderTriangles(camera);
    m_triangles->Create(functions);
    g_camera = camera;
}

//
void DebugDraw::Destroy()
{
    if (m_triangles) {
        m_triangles->Destroy();
        delete m_triangles;
        m_triangles = NULL;
    }
}

//
void DebugDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    b2Color fillColor;

    fillColor = b2Color(mainColor.redF(), mainColor.greenF(), mainColor.blueF(), mainColor.alphaF());

    for (int32 i = 1; i < vertexCount - 1; ++i) {
        m_triangles->Vertex(vertices[0], fillColor);
        m_triangles->Vertex(vertices[i], fillColor);
        m_triangles->Vertex(vertices[i + 1], fillColor);
    }
}


//
void DebugDraw::DrawSolidCircle(const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color)
{
    const float32 k_segments = 16.0f;
    const float32 k_increment = 2.0f * b2_pi / k_segments;
    float32 sinInc = sinf(k_increment);
    float32 cosInc = cosf(k_increment);
    b2Vec2 v0 = center;
    b2Vec2 r1(cosInc, sinInc);
    b2Vec2 v1 = center + radius * r1;

    b2Color fillColor;

    fillColor = b2Color(mainColor.redF(), mainColor.greenF(), mainColor.blueF(), mainColor.alphaF());

    for (int32 i = 0; i < k_segments; ++i) {
        // Perform rotation to avoid additional trigonometry.
        b2Vec2 r2;
        r2.x = cosInc * r1.x - sinInc * r1.y;
        r2.y = sinInc * r1.x + cosInc * r1.y;
        b2Vec2 v2 = center + radius * r2;
        m_triangles->Vertex(v0, fillColor);
        m_triangles->Vertex(v1, fillColor);
        m_triangles->Vertex(v2, fillColor);
        r1 = r2;
        v1 = v2;
    }
}

void DebugDraw::Flush()
{
    m_triangles->Flush();
}
