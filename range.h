#ifndef RANGE_H
#define RANGE_H


class Range
{
public:

    Range();

    Range(int leftBound, int rightBound);

    bool operator == (const Range &other) const;

    bool operator != (const Range &other) const;

    unsigned int GetLeftBound() const;

    unsigned int GetRightBound() const;

    void SetLeftBound(unsigned int LeftBound);

    void SetRightBound(unsigned int RightBound);

private:

    unsigned int m_leftBound;
    unsigned int m_rightBound;
};

#endif // RANGE_H
