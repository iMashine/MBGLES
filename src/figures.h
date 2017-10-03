#ifndef FIGURES_H
#define FIGURES_H

#include <src/emitter/b2emitter.h>

#include <src/debugdraw.h>
#include <src/floatrange.h>

class Figures : public B2Emitter
{
public:
    Figures(uint id, QString name, DebugDraw *debugDraw);

    void Step(Settings *settings);
    void CreateObstacle(const b2PolygonShape shape);

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
};

#endif // FIGURES_H
