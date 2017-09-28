#ifndef CAMERA_H
#define CAMERA_H

#include <Box2D/Box2D.h>

#include <QMatrix4x4>

class Camera
{
public:
    Camera();

    b2Vec2 ConvertScreenToWorld(const b2Vec2 &screenPoint);
    b2Vec2 ConvertWorldToScreen(const b2Vec2 &worldPoint);
    void BuildProjectionMatrix(float32 *m, float32 zBias);
    QMatrix4x4 BuildProjectionMatrix(float32 zBias);

    b2Vec2 m_center;
    float32 m_zoom;
    int32 m_width;
    int32 m_height;
};

#endif // CAMERA_H
