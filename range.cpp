#include "range.h"

Range::Range() : m_leftBound(0), m_rightBound(0) { }

Range::Range(int LeftBound, int RightBound) : m_leftBound(LeftBound), m_rightBound(RightBound) { }

bool Range::operator == (const Range &other) const
{
    if (this->m_leftBound == other.m_leftBound) {
        if (this->m_rightBound == other.m_rightBound) {
            return true;
        }
    }
    return false;
}

bool Range::operator != (const Range &other) const
{
    if (this->m_leftBound != other.m_leftBound) {
        return true;
    }
    if (this->m_rightBound != other.m_rightBound) {
        return true;
    }
    return false;
}

unsigned int Range::GetLeftBound() const
{
    return m_leftBound;
}

unsigned int Range::GetRightBound() const
{
    return m_rightBound;
}

void Range::SetLeftBound(unsigned int LeftBound)
{
    m_leftBound = LeftBound;
}

void Range::SetRightBound(unsigned int RightBound)
{
    m_rightBound = RightBound;
}
