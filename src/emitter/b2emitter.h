#ifndef TEST_H
#define TEST_H

#include "../debugdraw.h"
#include "emitter.h"
#include <Box2D/Box2D.h>
#include <src/utils/floatrange.h>

class B2Emitter;
struct Settings;

typedef B2Emitter *TestCreateFcn();

#define RAND_LIMIT  32767
#define DRAW_STRING_NEW_LINE 16

/// Random number in range [-1,1]
inline float32 RandomFloat()
{
    float32 r = (float32)(rand() & (RAND_LIMIT));
    r /= RAND_LIMIT;
    r = 2.0f * r - 1.0f;
    return r;
}

/// Random floating point number in range [lo, hi]
inline float32 RandomFloat(float32 lo, float32 hi)
{
    float32 r = (float32)(rand() & (RAND_LIMIT));
    r /= RAND_LIMIT;
    r = (hi - lo) * r + lo;
    return r;
}

/// Test settings. Some can be controlled in the GUI.
struct Settings {
    Settings()
    {
        hz = 30.0f;
        velocityIterations = 8;
        positionIterations = 3;
        drawShapes = true;
        drawJoints = false;
        drawAABBs = false;
        drawContactPoints = false;
        drawContactNormals = false;
        drawContactImpulse = false;
        drawFrictionImpulse = false;
        drawCOMs = false;
        drawStats = false;
        drawProfile = false;
        enableWarmStarting = true;
        enableContinuous = true;
        enableSubStepping = false;
        enableSleep = true;
        pause = false;
        singleStep = false;
    }

    float32 hz;
    int32 velocityIterations;
    int32 positionIterations;
    bool drawShapes;
    bool drawJoints;
    bool drawAABBs;
    bool drawContactPoints;
    bool drawContactNormals;
    bool drawContactImpulse;
    bool drawFrictionImpulse;
    bool drawCOMs;
    bool drawStats;
    bool drawProfile;
    bool enableWarmStarting;
    bool enableContinuous;
    bool enableSubStepping;
    bool enableSleep;
    bool pause;
    bool singleStep;
};

struct TestEntry {
    uint id;
    const char *name;
    TestCreateFcn *createFcn;
};

const int32 k_maxContactPoints = 2048;

class B2Emitter : public Emitter
{
public:

    B2Emitter();
    B2Emitter(uint id, QString name, DebugDraw &debugDraw);
    virtual ~B2Emitter();

//    void DrawTitle(const char *string);
    virtual void Step(Settings *settings);

    EmitterType GetType()
    {
        return EmitterType::B2;
    }

    QString GetEmitterName()
    {
        return m_name;
    }

    // todo need implement this methods
    void SetParticlesSpeed(uint value);

    uint GetParticlesSpeed();

    void SetParticlesSize(uint value);

    uint GetParticlesSize();

    void SetParticlesSaturation(uint value);

    uint GetParticlesSaturation();

    void SetParticlesColor(uint value);

    uint GetParticlesColor();

    void SetParticlesTransparency(uint value);

    uint GetParticlesTransparency();

    void SetId(uint id)
    {
        this->m_id = id;
    }

    void SetName(QString name)
    {
        m_name = name;
    }

    void CreateObstacle(const b2PolygonShape shape);

    void EnableTriangles();

    void DisableTriangles();

    void EnableCircles();

    void DisableCircles();

    void EnableRectangles();

    void DisableRectangles();

    void EnableEqualSize();

    void DisableEqualSize();

    FloatRange GetFiguresSize();

    void SetFiguresSize(float leftBound, float rightBound);

    uint GetMaxFiguresCount();

    void SetMaxFiguresCount(uint count);

    QColor GetColor();

    void SetColor(const QColor &color);

private:

    b2Shape *CreateTriangle();

    b2Shape *CreateCircle();

    b2Shape *CreateRectangle();

    uint m_maxFigures = 1000;
    uint m_count = 0;

    bool m_isEqualSize = true;

    FloatRange m_size;

    bool m_isTriangles = false;
    bool m_isCircles = true;
    bool m_isRectangles = false;

protected:
    b2World *m_world;
    QString m_name;

public:
    DebugDraw g_debugDraw;
    QPainter *m_painter;
};

#endif // TEST_H
