#include "floatrange.h"

FloatRange::FloatRange() : m_leftBound(0), m_rightBound(0) { }


FloatRange::FloatRange(float LeftBound, float RightBound)   : m_leftBound(LeftBound), m_rightBound(RightBound) { }

bool FloatRange::operator == (const FloatRange &other) const
{
    if (this->m_leftBound == other.m_leftBound) {
        if (this->m_rightBound == other.m_rightBound) {
            return true;
        }
    }
    return false;
}

bool FloatRange::operator != (const FloatRange &other) const
{
    if (this->m_leftBound != other.m_leftBound) {
        return true;
    }
    if (this->m_rightBound != other.m_rightBound) {
        return true;
    }
    return false;
}

float FloatRange::GetLeftBound() const
{
    return m_leftBound;
}

float FloatRange::GetRightBound() const
{
    return m_rightBound;
}

void FloatRange::SetLeftBound(float LeftBound)
{
    m_leftBound = LeftBound;
}

void FloatRange::SetRightBound(float RightBound)
{
    m_rightBound = RightBound;
}
