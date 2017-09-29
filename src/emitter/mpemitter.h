#ifndef MPEMITTER_H
#define MPEMITTER_H

#include "emitter.h"

#include "mp.h"
#include "platform_win_posix.h"

#include <QColor>
#include <QImage>
#include <QOpenGLBuffer>
#include <QVector2D>

class MainOpenGLWidget;

// rus: Класс, который хранит загруженные эмиттеры

#define MAGIC_STATE_STOP        0   // эмиттер не работает

#define MAGIC_STATE_UPDATE      1   // эмиттер обновляется и рисуется

#define MAGIC_STATE_INTERRUPT   2   // эмиттер обновляется и рисуется до момента уничтожения всех созданных частиц, новые частицы больше не создаются

#define MAGIC_STATE_VISIBLE     3   // эмиттер только рисуется

enum RENDER_BLENDING_MODE { RENDER_BLENDING_NORMAL, RENDER_BLENDING_ADD, RENDER_BLENDING_OPACITY };

struct STAGE {
    int texture;
    int address_u, address_v;
};

class MPEmitter : public Emitter
{
public:
    MPEmitter();

    MPEmitter(unsigned int id, MP_Manager *manager = Q_NULLPTR);

    ~MPEmitter();

    MPEmitter &operator=(const MPEmitter &from);

    QString GetEmitterName();

    void Restart();

    QPointF GetPosition();

    void SetPosition(QPointF position);

    void Move(QPointF position, bool restart = false);

    void Offset(QPointF offset);

    void SetDirection(qreal angle);

    qreal GetDirection();

    void Direct(qreal angle, bool restart = false);

    void Rotate(qreal angle);

    qreal GetScale();

    void SetScale(qreal scale);

    unsigned int GetState();

    void SetState(unsigned int state);

    void Update(double time);

    unsigned int Render();

    void SetManager(MP_Manager *manager);

    bool Contains(QPointF mousePos);

    EmitterType GetType()
    {
        return EmitterType::MP;
    }

    void SetParticlesSpeed(uint value);

    uint GetParticlesSpeed();

    void SetParticlesSize(uint value);

    uint GetParticlesSize();

    void SetParticlesSaturation(uint value);

    uint GetParticlesSaturation();

    void SetParticlesLife(unsigned int value);

    unsigned int GetParticlesLife();

    void SetParticlesWeight(unsigned int value);

    unsigned int GetParticlesWeight();

    unsigned int GetParticleParameter(MAGIC_DIAGRAM_ENUM parameter);

    void SetParticlesParameter(MAGIC_DIAGRAM_ENUM parameter, unsigned int value);

protected:

    unsigned int state;
    MP_Manager *m_manager;
    int m_multitextures = 0;
    STAGE m_stages[16];
    bool first_restart; // признак того, что эмиттер еще не выставлялся на стартовую позицию анимации


public:
    bool is_atlas;              // признак того, что атлас для данного эмиттера уже был построен
    std::string restore_file;   // файл для восстановления текстур эмиттера
    static bool temporary_emitter_modes[32];
    static MainOpenGLWidget *m_widget;
};

#endif // MPEMITTER_H
