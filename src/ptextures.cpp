#include "ptextures.h"

TextureList::TextureList()
{
}

TextureList::~TextureList()
{
    for (int i = 0; i < list.count(); i++) {
        list[i]->release();
    }
}

void TextureList::CreateTexture(QOpenGLTexture::Target target)
{
    list.push_back(new QOpenGLTexture(target));
}

void TextureList::Destroy()
{
    for (int i = 0; i < list.count(); i++) {
        list[i]->release();
    }
}

QOpenGLTexture &TextureList::at(int i)
{
    return *list.at(i);
}

void TextureList::RefreshTextures()
{
    for (int i = 0; i < list.count(); i++) {
        list.at(i)->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
        list.at(i)->bind();
    }
}

QOpenGLTexture *TextureList::GetTexture(int id)
{
    for (int i = 0; i < list.count(); i++) {
        if (list.at(i)->textureId() == id) {
            return list.at(i);
        }
    }
    return NULL;
}
