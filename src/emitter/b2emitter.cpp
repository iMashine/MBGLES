#include "b2emitter.h"

B2Emitter::B2Emitter()
{
    m_id = 0;
}

B2Emitter::B2Emitter(uint id, QString name, DebugDraw &debugDraw) :
    m_isEqualSize(true),
    m_isCircles(true),
    m_isTriangles(false),
    m_isRectangles(false)
{
    g_debugDraw = debugDraw;

    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    m_world = new b2World(gravity);
    m_world->SetDebugDraw(&g_debugDraw);
    m_id = id;
    m_name = name;

    float32 y = g_debugDraw.g_camera->ConvertScreenToWorld(b2Vec2(0, g_debugDraw.g_camera->m_height)).y,
            x = g_debugDraw.g_camera->ConvertScreenToWorld(b2Vec2(g_debugDraw.g_camera->m_width, 0)).x;

//    b2PolygonShape obstacle;
//    obstacle.SetAsBox(25.0f, 2.0f, b2Vec2((x / 2),  10.0f), 0);

//    CreateObstacle(obstacle);

    m_size = FloatRange(0.5f, 0.8f);

    g_debugDraw.SetColor(Qt::darkMagenta);
}

B2Emitter::~B2Emitter()
{
    // By deleting the world, we delete the bomb, mouse joint, etc.
    delete m_world;
    m_world = NULL;
//    g_debugDraw.Destroy();
}

void B2Emitter::Step(Settings *settings)
{
    float32 timeStep = settings->hz > 0.0f ? 1.0f / settings->hz : float32(0.0f);

    if (settings->pause) {
        if (settings->singleStep) {
            settings->singleStep = 0;
        }
        else {
            timeStep = 0.0f;
        }
    }

    uint32 flags = 0;
    flags += settings->drawShapes           * b2Draw::e_shapeBit;
    flags += settings->drawJoints           * b2Draw::e_jointBit;
    flags += settings->drawAABBs            * b2Draw::e_aabbBit;
    flags += settings->drawCOMs             * b2Draw::e_centerOfMassBit;
    g_debugDraw.SetFlags(flags);

    m_world->SetAllowSleeping(settings->enableSleep);
    m_world->SetWarmStarting(settings->enableWarmStarting);
    m_world->SetContinuousPhysics(settings->enableContinuous);
    m_world->SetSubStepping(settings->enableSubStepping);

    m_world->Step(timeStep, settings->velocityIterations, settings->positionIterations);

    m_world->DrawDebugData();
    g_debugDraw.Flush();

    if (m_count < m_maxFigures) {

        b2Shape *shape;

        if (m_isCircles && !m_isTriangles && !m_isRectangles) {
            shape = CreateCircle();
        }
        else if (!m_isCircles && m_isTriangles && !m_isRectangles) {
            shape = CreateTriangle();
        }
        else if (!m_isCircles && !m_isTriangles && m_isRectangles) {
            shape = CreateRectangle();
        }
        else if (m_isCircles && m_isTriangles && !m_isRectangles) {
            int what = rand() % 2 + 1;
            shape = what == 1 ? CreateTriangle() : CreateCircle();
        }
        else if (m_isCircles && !m_isTriangles && m_isRectangles) {
            int what = rand() % 2 + 1;
            shape = what == 1 ? CreateRectangle() : CreateCircle();
        }
        else if (!m_isCircles && m_isTriangles && m_isRectangles) {
            int what = rand() % 2 + 1;
            shape = what == 1 ? CreateTriangle() : CreateRectangle();
        }
        else if (m_isCircles && m_isTriangles && m_isRectangles) {
            int what = rand() % 3 + 1;
            switch (what) {
            case 1: {
                shape = CreateTriangle();
                break;
            }
            case 2: {
                shape = CreateRectangle();
                break;
            }
            case 3: {
                shape = CreateCircle();
                break;
            }
            }
        }

        b2FixtureDef fd;
        fd.shape = shape;
        fd.density = 1.0f;
        fd.restitution = 0.4f;

        float32 x1 = g_debugDraw.g_camera->ConvertScreenToWorld(b2Vec2(0, 0)).x,
                x2 = g_debugDraw.g_camera->ConvertScreenToWorld(b2Vec2(g_debugDraw.g_camera->m_width, 0)).x;

        float32 y = g_debugDraw.g_camera->ConvertScreenToWorld(b2Vec2(0, 0)).y;

        float32 x = RandomFloat(x1, x2);
        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.position.Set(x + 5.0f, y);
//        bd.angle = RandomFloat(-b2_pi, b2_pi);
        bd.linearDamping = 0.0f;
        bd.angularDamping = 0.01f;
        b2Body *body = m_world->CreateBody(&bd);
        body->CreateFixture(&fd);
        m_count++;
    }
}

void B2Emitter::SetParticlesSpeed(uint value)
{

}

uint B2Emitter::GetParticlesSpeed()
{

}

void B2Emitter::SetParticlesSize(uint value)
{

}

uint B2Emitter::GetParticlesSize()
{

}

void B2Emitter::SetParticlesSaturation(uint value)
{

}

uint B2Emitter::GetParticlesSaturation()
{

}

void B2Emitter::SetParticlesColor(uint value)
{
    g_debugDraw.SetColor(value);
}

uint B2Emitter::GetParticlesColor()
{
    QColor q_color = g_debugDraw.GetColor();

    int color;
    color = q_color.alpha();
    color = (color << 8) + q_color.blue();
    color = (color << 8) + q_color.green();
    color = (color << 8) + q_color.red();

    return color;
}

void B2Emitter::SetParticlesTransparency(uint value)
{

}

uint B2Emitter::GetParticlesTransparency()
{

}

b2Shape *B2Emitter::CreateTriangle()
{
    b2Transform xf1;
    xf1.q.Set(0.3524f * b2_pi);
    xf1.p = xf1.q.GetXAxis();

    b2Vec2 vertices[3];

    b2PolygonShape *triangle = new b2PolygonShape();
    float32 size;

    if (m_isEqualSize) size = m_size.GetLeftBound();
    else size = RandomFloat(m_size.GetLeftBound(), m_size.GetRightBound());

    vertices[0] = b2Mul(xf1, b2Vec2(size, -size));
    vertices[1] = b2Mul(xf1, b2Vec2(-size, -size));
    vertices[2] = b2Mul(xf1, b2Vec2(-size, size));
    triangle->Set(vertices, 3);

    return triangle;
}

b2Shape *B2Emitter::CreateCircle()
{
    b2CircleShape *circle = new b2CircleShape();
    float32 radius;

    if (m_isEqualSize) radius = m_size.GetLeftBound();
    else radius = RandomFloat(m_size.GetLeftBound(), m_size.GetRightBound());

    circle->m_radius = radius;
    circle->m_p.Set(-1 * radius, radius);

    return circle;
}

b2Shape *B2Emitter::CreateRectangle()
{
    b2PolygonShape *rectangle = new b2PolygonShape();
    float32 size;

    if (m_isEqualSize) size = m_size.GetLeftBound();
    else size = RandomFloat(m_size.GetLeftBound(), m_size.GetRightBound());

    rectangle->SetAsBox(size, size, b2Vec2(-1 * size, 0.0f), 0.0f);

    return rectangle;
}

void B2Emitter::CreateObstacle(const b2PolygonShape shape)
{
    b2BodyDef bd;
    bd.position.Set(0.0f, 0.0f);
    b2Body *body = m_world->CreateBody(&bd);

    int vertexCount = shape.GetVertexCount();
    b2EdgeShape shapes[vertexCount];

    for (int i = 0; i < vertexCount - 1; i++) {
        shapes[i].Set(shape.GetVertex(i), shape.GetVertex(i + 1));
    }

    shapes[vertexCount - 1].Set(shape.GetVertex(vertexCount - 1), shape.GetVertex(0));

    for (int i = 0; i < vertexCount; i++) {
        body->CreateFixture(&shapes[i], 0.0f);
    }
}

void B2Emitter::EnableTriangles()
{
    m_isTriangles = true;
}

void B2Emitter::DisableTriangles()
{
    m_isTriangles = false;
}

void B2Emitter::EnableCircles()
{
    m_isCircles = true;
}

void B2Emitter::DisableCircles()
{
    m_isCircles = false;
}

void B2Emitter::EnableRectangles()
{
    m_isRectangles = true;
}

void B2Emitter::DisableRectangles()
{
    m_isRectangles = false;
}

void B2Emitter::B2Emitter::EnableEqualSize()
{
    m_isEqualSize = true;
}

void B2Emitter::DisableEqualSize()
{
    m_isEqualSize = false;
}

FloatRange B2Emitter::GetFiguresSize()
{
    return m_size;
}

void B2Emitter::SetFiguresSize(float leftBound, float rightBound)
{
    m_size = FloatRange(leftBound, rightBound);
}

uint B2Emitter::GetMaxFiguresCount()
{
    return m_maxFigures;
}

void B2Emitter::SetMaxFiguresCount(uint count)
{
    m_maxFigures = count;
}
