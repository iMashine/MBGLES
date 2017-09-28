#include "debugdraw.h"

DebugDraw::DebugDraw()
{
    m_points = NULL;
    m_lines = NULL;
    m_triangles = NULL;
}

//
DebugDraw::~DebugDraw()
{
//    b2Assert(m_points == NULL);
//    b2Assert(m_lines == NULL);
//    b2Assert(m_triangles == NULL);
}

void DebugDraw::Create(Camera *camera, QOpenGLFunctions *functions)
{
    m_points = new RenderPoints(camera);
    m_points->Create(functions);
    m_lines = new RenderLines(camera);
    m_lines->Create(functions);
    m_triangles = new RenderTriangles(camera);
    m_triangles->Create(functions);
    g_camera = camera;
}

//
void DebugDraw::Destroy()
{
    if (m_points) {
        m_points->Destroy();
        delete m_points;
        m_points = NULL;
    }

    if (m_lines) {
        m_lines->Destroy();
        delete m_lines;
        m_lines = NULL;
    }

    if (m_triangles) {
        m_triangles->Destroy();
        delete m_triangles;
        m_triangles = NULL;
    }
}

//
void DebugDraw::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    b2Vec2 p1 = vertices[vertexCount - 1];
    for (int32 i = 0; i < vertexCount; ++i) {
        b2Vec2 p2 = vertices[i];
        m_lines->Vertex(p1, color);
        m_lines->Vertex(p2, color);
        p1 = p2;
    }
}

//
void DebugDraw::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    b2Color fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);

    for (int32 i = 1; i < vertexCount - 1; ++i) {
        m_triangles->Vertex(vertices[0], fillColor);
        m_triangles->Vertex(vertices[i], fillColor);
        m_triangles->Vertex(vertices[i + 1], fillColor);
    }

    b2Vec2 p1 = vertices[vertexCount - 1];
    for (int32 i = 0; i < vertexCount; ++i) {
        b2Vec2 p2 = vertices[i];
        m_lines->Vertex(p1, color);
        m_lines->Vertex(p2, color);
        p1 = p2;
    }
}

//
void DebugDraw::DrawCircle(const b2Vec2 &center, float32 radius, const b2Color &color)
{
    const float32 k_segments = 16.0f;
    const float32 k_increment = 2.0f * b2_pi / k_segments;
    float32 sinInc = sinf(k_increment);
    float32 cosInc = cosf(k_increment);
    b2Vec2 r1(1.0f, 0.0f);
    b2Vec2 v1 = center + radius * r1;
    for (int32 i = 0; i < k_segments; ++i) {
        // Perform rotation to avoid additional trigonometry.
        b2Vec2 r2;
        r2.x = cosInc * r1.x - sinInc * r1.y;
        r2.y = sinInc * r1.x + cosInc * r1.y;
        b2Vec2 v2 = center + radius * r2;
        m_lines->Vertex(v1, color);
        m_lines->Vertex(v2, color);
        r1 = r2;
        v1 = v2;
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
    b2Color fillColor(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
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

    r1.Set(1.0f, 0.0f);
    v1 = center + radius * r1;
    for (int32 i = 0; i < k_segments; ++i) {
        b2Vec2 r2;
        r2.x = cosInc * r1.x - sinInc * r1.y;
        r2.y = sinInc * r1.x + cosInc * r1.y;
        b2Vec2 v2 = center + radius * r2;
        m_lines->Vertex(v1, color);
        m_lines->Vertex(v2, color);
        r1 = r2;
        v1 = v2;
    }

    // Draw a line fixed in the circle to animate rotation.
    b2Vec2 p = center + radius * axis;
    m_lines->Vertex(center, color);
    m_lines->Vertex(p, color);
}

//
void DebugDraw::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
{
    m_lines->Vertex(p1, color);
    m_lines->Vertex(p2, color);
}

//
void DebugDraw::DrawTransform(const b2Transform &xf)
{
    const float32 k_axisScale = 0.4f;
    b2Color red(1.0f, 0.0f, 0.0f);
    b2Color green(0.0f, 1.0f, 0.0f);
    b2Vec2 p1 = xf.p, p2;

    m_lines->Vertex(p1, red);
    p2 = p1 + k_axisScale * xf.q.GetXAxis();
    m_lines->Vertex(p2, red);

    m_lines->Vertex(p1, green);
    p2 = p1 + k_axisScale * xf.q.GetYAxis();
    m_lines->Vertex(p2, green);
}

//
void DebugDraw::DrawPoint(const b2Vec2 &p, float32 size, const b2Color &color)
{
    m_points->Vertex(p, color, size);
}

//
void DebugDraw::DrawString(QPainter *painter, int x, int y, const char *string)
{
    painter->save();
    painter->setPen(QColor(230, 153, 153, 255));
    painter->drawText(x, y, QString(string));
    painter->restore();
}

//
void DebugDraw::DrawString(QPainter *painter, const b2Vec2 &pw, const char *string)
{
    b2Vec2 ps = g_camera->ConvertWorldToScreen(pw);

    painter->save();
    painter->setPen(QColor(230, 153, 153, 255));
    painter->drawText(ps.x, ps.y, QString(string));
    painter->restore();
}

//
void DebugDraw::DrawAABB(b2AABB *aabb, const b2Color &c)
{
    b2Vec2 p1 = aabb->lowerBound;
    b2Vec2 p2 = b2Vec2(aabb->upperBound.x, aabb->lowerBound.y);
    b2Vec2 p3 = aabb->upperBound;
    b2Vec2 p4 = b2Vec2(aabb->lowerBound.x, aabb->upperBound.y);

    m_lines->Vertex(p1, c);
    m_lines->Vertex(p2, c);

    m_lines->Vertex(p2, c);
    m_lines->Vertex(p3, c);

    m_lines->Vertex(p3, c);
    m_lines->Vertex(p4, c);

    m_lines->Vertex(p4, c);
    m_lines->Vertex(p1, c);
}

//
void DebugDraw::Flush()
{
    m_triangles->Flush();
    m_lines->Flush();
    m_points->Flush();
}
