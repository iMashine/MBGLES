#ifndef EMITTER_H
#define EMITTER_H

#include <QString>

typedef unsigned int uint;

//
class Emitter
{
public:

    Emitter() { }

    ~Emitter() { }

    virtual uint GetId() = 0;

    virtual QString GetEmitterName() = 0;

    virtual void SetParticlesSpeed(uint value) = 0;

    virtual uint GetParticlesSpeed() = 0;

    virtual void SetParticlesSize(uint value) = 0;

    virtual uint GetParticlesSize() = 0;

    virtual void SetParticlesSaturation(uint value) = 0;

    virtual uint GetParticlesSaturation() = 0;



//protected:

    QString m_name;
    uint m_id;
};

#endif // EMITTER_H
