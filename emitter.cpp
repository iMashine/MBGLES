#include "emitter.h"
#include "mainopenglwidget.h"

bool Emitter::temporary_emitter_modes[32];

MainOpenGLWidget *Emitter::m_widget = Q_NULLPTR;

Emitter::Emitter()
{
}

Emitter::Emitter(unsigned int id, MP_Manager *manager)
{
    m_id = id;

    first_restart = true;

    restore_file = "";

    state = MAGIC_STATE_UPDATE;

    is_atlas = false;

    if (manager)
        m_manager = manager;
}

Emitter::~Emitter()
{
    Magic_UnloadEmitter(m_id);
}

Emitter &Emitter::operator=(const Emitter &from)
{
    state = from.state;

    is_atlas = from.is_atlas;

    restore_file = from.restore_file;

    m_id = Magic_DuplicateEmitter(from.m_id);

    return *this;
}

unsigned int Emitter::GetId()
{
    return m_id;
}

QString Emitter::GetEmitterName()
{
    return QString(Magic_GetEmitterName(m_id));
}

void Emitter::Restart()
{
    if (!Magic_IsInterval1(m_id)) {
        Magic_Restart(m_id);
    }

    first_restart = false;
}

QPointF Emitter::GetPosition()
{
    MAGIC_POSITION pos;
    Magic_GetEmitterPosition(m_id, &pos);
    return QPointF(pos.x, pos.y);
}

void Emitter::SetPosition(QPointF position)
{
    MAGIC_POSITION pos;
    pos.x = position.x();
    pos.y = position.y();
    Magic_SetEmitterPosition(m_id, &pos);
}

void Emitter::Move(QPointF position, bool restart)
{
    if (restart) {
        Restart();
        SetPosition(position);
    }
    else {
        // eng: temporary setting mode of movement together with particles
        // rus: временно устанавливаем режим перемещения вместе с частицами
        int i;
        int emitters = Magic_GetEmitterCount(m_id);
        for (i = 0; i < emitters; i++) {
            unsigned int child_id = Magic_GetEmitter(m_id, i);
            temporary_emitter_modes[i] = Magic_GetEmitterPositionMode(child_id);
        }
        Magic_SetEmitterPositionMode(m_id, MAGIC_CHANGE_EMITTER_AND_PARTICLES);

        SetPosition(position);

        // eng: restore previous mode of movement
        // rus: возвращаем на место старый режим перемещения
        for (i = 0; i < emitters; i++) {
            unsigned int child_id = Magic_GetEmitter(m_id, i);
            Magic_SetEmitterPositionMode(child_id, temporary_emitter_modes[i]);
        }
    }
}

void Emitter::Offset(QPointF offset)
{
    QPointF pos = GetPosition();
    pos += offset;
    SetPosition(pos);
}

void Emitter::SetDirection(qreal angle)
{
    MAGIC_DIRECTION direction;
    direction.angle = angle;
    Magic_SetEmitterDirection(m_id, &direction);
}

qreal Emitter::GetDirection()
{
    MAGIC_DIRECTION direction;
    Magic_GetEmitterDirection(m_id, &direction);
    return (qreal)direction.angle;
}

void Emitter::Direct(qreal angle, bool restart)
{
    if (restart) {
        Restart();
        SetDirection(angle);
    }
    else {
        // eng: temporary setting mode of rotation together with particles
        // rus: временно устанавливаем режим вращения вместе с частицами
        int i;
        int emitters = Magic_GetEmitterCount(m_id);
        for (i = 0; i < emitters; i++) {
            unsigned int child_id = Magic_GetEmitter(m_id, i);
            temporary_emitter_modes[i] = Magic_GetEmitterDirectionMode(child_id);
        }
        Magic_SetEmitterDirectionMode(m_id, MAGIC_CHANGE_EMITTER_AND_PARTICLES);

        SetDirection(angle);

        // eng: restore previous mode of movement
        // rus: возвращаем на место старый режим перемещения
        for (i = 0; i < emitters; i++) {
            unsigned int child_id = Magic_GetEmitter(m_id, i);
            Magic_SetEmitterDirectionMode(child_id, temporary_emitter_modes[i]);
        }
    }
}

void Emitter::Rotate(qreal angle)
{
    qreal q = GetDirection();
    q += angle;
    SetDirection(q);
}

// eng: Setting the scale of the emitter
// rus: Установка масштаба эмиттера
void Emitter::SetScale(qreal scale)
{
    Magic_SetScale(m_id, scale);
}

unsigned int Emitter::GetState()
{
    return state;
}

void Emitter::SetState(unsigned int state)
{
    if (this->state != state) {
        if (state == MAGIC_STATE_UPDATE && Magic_IsInterrupt(m_id)) {
            // eng: it is necessary to turn off interrupting of emitter work
            // rus: необходимо отключить прерывание работы эмиттера
            Magic_SetInterrupt(m_id, false);
        }

        if (state == MAGIC_STATE_STOP && this->state != MAGIC_STATE_INTERRUPT) {
            // eng: unload particles from memory
            // rus: выгружаем пространство частиц из памяти
            Magic_Stop(m_id);
        }
        else {
            if (state == MAGIC_STATE_UPDATE || state == MAGIC_STATE_INTERRUPT) {
                // eng: start emitter
                // rus: стартуем эмиттер
                if (!first_restart) {
                    if (this->state == MAGIC_STATE_STOP || (!Magic_InInterval(m_id))) {
//                        Magic_CorrectEmitterPosition(m_id, device->window_width, device->window_height);
                    }
                }

                if (state == MAGIC_STATE_INTERRUPT)
                    Magic_SetInterrupt(m_id, true);
            }
        }

        this->state = state;
    }
}

void Emitter::Update(double time)
{
    if (state == MAGIC_STATE_UPDATE || state == MAGIC_STATE_INTERRUPT) {
        if (first_restart)
            Restart();

        if (!Magic_IsInterpolationMode(m_id)) {
            // rus: without interpolation a fixing step is only possible
            // rus: без интерполяции возможен только фиксированный шаг
            time = Magic_GetUpdateTime(m_id);
        }

        if (!Magic_Update(m_id, time)) {
            // eng: working of emitter is over
            // rus: выполнение эмиттера завершено
//            SetState(MAGIC_STATE_STOP);
        }
    }
}

QOpenGLTexture::WrapMode QOpenGLTextureWrapMode[] = {
    QOpenGLTexture::Repeat,
    QOpenGLTexture::MirroredRepeat,
    QOpenGLTexture::ClampToEdge,
    QOpenGLTexture::ClampToBorder
};

unsigned int Emitter::Render()
{
    Q_ASSERT(m_manager);
    int count = 0;
    if (state != MAGIC_STATE_STOP) {
        unsigned int emitter_id = GetId();
        if (Magic_InInterval(emitter_id)) {
            MAGIC_RENDERING_START start;

            void *context = Magic_PrepareRenderArrays(emitter_id, &start, 4, MAGIC_ARGB, false);
            if (start.arrays) {

                count = start.particles;

                for (int i = 0; i < 16; i++) {
                    STAGE *s = &(m_stages[i]);
                    s->texture = -1;
                    s->address_u = -1;
                    s->address_v = -1;
                }

                MAGIC_ARRAY_INFO vertex_info, color_info, textureCoord_info, index_info;

                int status = 0;

                status = Magic_GetRenderArrayData(context, 0, &vertex_info);
                Q_ASSERT(status == MAGIC_SUCCESS);
                status = Magic_GetRenderArrayData(context, 1, &color_info);
                Q_ASSERT(status == MAGIC_SUCCESS);
                status = Magic_GetRenderArrayData(context, 2, &textureCoord_info);
                Q_ASSERT(status == MAGIC_SUCCESS);
                status = Magic_GetRenderArrayData(context, 3, &index_info);
                Q_ASSERT(status == MAGIC_SUCCESS);

                m_widget->Refresh();

                m_multitextures = 0;

                m_widget->ClearBuffers();

                m_widget->m_vertices = new float[vertex_info.length * 2];
                status = Magic_SetRenderArrayData(context, 0, m_widget->m_vertices, 0, vertex_info.bytes_per_one);
                Q_ASSERT(status == MAGIC_SUCCESS);

                m_widget->m_colors = new unsigned char[color_info.length * 4];
                status = Magic_SetRenderArrayData(context, 1, m_widget->m_colors, 0, color_info.bytes_per_one);
                Q_ASSERT(status == MAGIC_SUCCESS);

                m_widget->m_textureCoords = new float[textureCoord_info.length * 2];
                status = Magic_SetRenderArrayData(context, 2, m_widget->m_textureCoords, 0, textureCoord_info.bytes_per_one);
                Q_ASSERT(status == MAGIC_SUCCESS);

                m_widget->m_indexes = new unsigned short[index_info.length];
                status = Magic_SetRenderArrayData(context, 3, m_widget->m_indexes, 0, index_info.bytes_per_one);
                Q_ASSERT(status == MAGIC_SUCCESS);

                // заполнение буферов данными
                Magic_FillRenderArrays(context);

                MAGIC_RENDER_VERTICES vrts;
                MAGIC_RENDER_STATE state;

                while (Magic_GetVertices(context, &vrts) == MAGIC_SUCCESS) {
                    while (Magic_GetNextRenderState(context, &state) == MAGIC_SUCCESS) {
                        if (state.state == MAGIC_RENDER_STATE_BLENDING) {
                            if (state.value == RENDER_BLENDING_NORMAL) {
                                m_manager->m_functions->glEnable(GL_BLEND);
                                m_manager->m_functions->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                            }
                            else if (state.value == RENDER_BLENDING_ADD) {
                                m_manager->m_functions->glEnable(GL_BLEND);
                                m_manager->m_functions->glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                            }
                            else if (state.value == RENDER_BLENDING_OPACITY) {
                                m_manager->m_functions->glDisable(GL_BLEND);
                            }
                        }
                        else if (state.state == MAGIC_RENDER_STATE_TEXTURE_COUNT) {
                            if (m_multitextures != state.value) {
                                m_multitextures = state.value;
                            }
                        }
                        else if (state.state == MAGIC_RENDER_STATE_TEXTURE) {
                            MP_Atlas *atlas = (MP_Atlas *)MP_Manager::GetInstance().GetAtlas(state.value);
                            GLuint tex = atlas->GetTexture();
                            STAGE *stage = &(m_stages[state.index]);
                            stage->texture = state.value;
//                            m_manager->m_textures->GetTexture(tex)->bind();
//                            if (atlas->address_u != stage->address_u) {
//                                atlas->address_u = stage->address_u;
//                                m_manager->m_textures->GetTexture(tex)->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTextureWrapMode[stage->address_u]);
//                            }
//                            if (atlas->address_v != stage->address_v) {
//                                atlas->address_v = stage->address_v;
//                            m_manager->m_textures->GetTexture(tex)->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTextureWrapMode[stage->address_v]);
//                            }
                            m_manager->m_textures->GetTexture(tex)->bind();
                            QOpenGLTexture::WrapMode mode = QOpenGLTextureWrapMode[0];
                            m_manager->m_textures->GetTexture(tex)->setWrapMode(QOpenGLTexture::DirectionS, mode);
                            m_manager->m_textures->GetTexture(tex)->setWrapMode(QOpenGLTexture::DirectionT, mode);
                        }
//                        else if (state.state == MAGIC_RENDER_STATE_ADDRESS_U) {
//                            STAGE *stage = &(m_stages[state.index]);
//                            if (stage->texture != -1) {
//                                MP_Atlas *atlas = (MP_Atlas *)MP_Manager::GetInstance().GetAtlas(stage->texture);
//                                if (atlas->address_u != state.value) {
//                                    atlas->address_u = state.value;
//                                    m_manager->m_textures->at(stage->texture).bind();
//                                    m_manager->m_textures->at(stage->texture).setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTextureWrapMode[state.value]);
//                                }
//                            }
//                            stage->address_u = state.value;
//                        }
//                        else if (state.state == MAGIC_RENDER_STATE_ADDRESS_V) {
//                            STAGE *stage = &(m_stages[state.index]);
//                            if (stage->texture != -1) {
//                                MP_Atlas *atlas = (MP_Atlas *)MP_Manager::GetInstance().GetAtlas(stage->texture);
//                                if (atlas->address_v != state.value) {
//                                    atlas->address_v = state.value;
//                                    m_manager->m_textures->at(stage->texture).bind();
//                                    m_manager->m_textures->at(stage->texture).setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTextureWrapMode[state.value]);
//                                }
//                            }
//                            stage->address_v = state.value;
//                        }
                    }
                    m_widget->Draw(vrts.starting_index, vrts.indexes_count, vertex_info.length);
                }
            }
        }
    }
    return count;
}

bool Emitter::Contains(QPointF mousePos)
{
    bool isContains = false;

    // способ 1 - через родной бокс
    MAGIC_BBOX currentBox;
    if (MAGIC_SUCCESS == Magic_RecalcBBox(m_id) && MAGIC_SUCCESS == Magic_GetBBox(m_id, &currentBox)) {
        int x1 = currentBox.corner1.x,
            x2 = currentBox.corner2.x;
        int y1 = m_widget->GetWidth() + currentBox.corner1.y,
            y2 = m_widget->GetHeight()  + currentBox.corner2.y;

        if (mousePos.x() + 50 >= x1 && mousePos.x() - 50 <= x2 && mousePos.y() + 50 >= y1 && mousePos.y() - 50 <= y2)
            isContains = true;
    }

    // способ 2 - через крайние точки частиц
    else {
//        int ParticlesTypeCount = Magic_GetParticlesTypeCount(m_id);

//        if (MAGIC_SUCCESS == Magic_LockParticlesType(m_id, ParticlesTypeCount - 1)) {
//            MAGIC_PARTICLE currentParticle;
//            int leftX = 99999, rightX = -1, leftY = 99999, rightY = -1;

//            while (MAGIC_SUCCESS == Magic_GetNextParticle(&currentParticle)) {
//                leftX = currentParticle.position.x < leftX ? currentParticle.position.x : leftX;
//                rightX = currentParticle.position.x > rightX ? currentParticle.position.x : rightX;
//                leftY = currentParticle.position.y < leftY ? currentParticle.position.y : leftY;
//                rightY = currentParticle.position.y > rightY ? currentParticle.position.y : rightY;
//            }

//            Q_ASSERT(MAGIC_SUCCESS == Magic_UnlockParticlesType());

//            isContains = false;
//            if (mousePos.x() + 50 >= leftX && mousePos.x() - 50 <= rightX && mousePos.y() + 50 >= leftY && mousePos.y() - 50 <= rightY)
//                isContains = true;
//        }
    }

    return isContains;
}

void Emitter::SetParticlesSpeed(unsigned int value)
{
    this->SetParticlesParameter(MAGIC_DIAGRAM_VELOCITY, value);
}

unsigned int Emitter::GetParticlesSpeed()
{
    return this->GetParticleParameter(MAGIC_DIAGRAM_VELOCITY);
}

void Emitter::SetParticlesSize(unsigned int value)
{
    this->SetParticlesParameter(MAGIC_DIAGRAM_SIZE, value);
}

unsigned int Emitter::GetParticlesSize()
{
    return this->GetParticleParameter(MAGIC_DIAGRAM_SIZE);
}

void Emitter::SetParticlesSaturation(unsigned int value)
{
    this->SetParticlesParameter(MAGIC_DIAGRAM_NUMBER, value);
}

unsigned int Emitter::GetParticlesSaturation()
{
    return this->GetParticleParameter(MAGIC_DIAGRAM_NUMBER);
}

void Emitter::SetParticlesLife(unsigned int value)
{
    this->SetParticlesParameter(MAGIC_DIAGRAM_LIFE, value);
}

unsigned int Emitter::GetParticlesLife()
{
    return this->GetParticleParameter(MAGIC_DIAGRAM_LIFE);
}

void Emitter::SetParticlesWeight(unsigned int value)
{
    this->SetParticlesParameter(MAGIC_DIAGRAM_WEIGHT, value);
}

unsigned int Emitter::GetParticlesWeight()
{
    return this->GetParticleParameter(MAGIC_DIAGRAM_WEIGHT);
}

void Emitter::BindTexture()
{
    if (m_texture)
        m_texture->bind();
}

unsigned int Emitter::GetParticleParameter(MAGIC_DIAGRAM_ENUM parameter)
{
    float Factor = 0;
    if (Magic_GetParticlesTypeCount(m_id)) {
        if (Magic_LockParticlesType(m_id, 0)) {
            Factor += Magic_GetDiagramFactor(m_id, 0, parameter);
            Magic_UnlockParticlesType();
            return (unsigned int)(Factor * 10);
        }
    }
}

void Emitter::SetParticlesParameter(MAGIC_DIAGRAM_ENUM parameter, unsigned int value)
{
    if (Magic_GetParticlesTypeCount(m_id)) {
        if (Magic_LockParticlesType(m_id, 0)) {
            Magic_SetDiagramFactor(m_id, 0, parameter, ((float)(value) / 10));
            Magic_UnlockParticlesType();
        }
    }
}

qreal Emitter::GetScale()
{
    return Magic_GetScale(m_id);
}
