#ifndef PTEXTURES_H
#define PTEXTURES_H

#include <QOpenGLTexture>
#include <QOpenGLContext>
#include <QDebug>

class TextureList
{
public:
    TextureList();
    ~TextureList();

    void CreateTexture(QOpenGLTexture::Target target);

    void Destroy();

    QOpenGLTexture &at(int i);

    void RefreshTextures();

    QOpenGLTexture *GetTexture(int id);

    QList<QOpenGLTexture *> list;

};

#endif // PTEXTURES_H
