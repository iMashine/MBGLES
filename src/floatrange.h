#ifndef FLOATRANGE_H
#define FLOATRANGE_H

class FloatRange
{
public:

    FloatRange();

    FloatRange(float leftBound, float rightBound);

    bool operator == (const FloatRange &other) const;

    bool operator != (const FloatRange &other) const;

    float GetLeftBound() const;

    float GetRightBound() const;

    void SetLeftBound(float LeftBound);

    void SetRightBound(float RightBound);

private:

    float m_leftBound;
    float m_rightBound;
};

#endif // FLOATRANGE_H
