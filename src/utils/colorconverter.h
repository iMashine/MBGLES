#ifndef COLORCONVERTER_H
#define COLORCONVERTER_H

#include <QColor>


class ColorConverter
{
public:
    ColorConverter();

    uint GetColorFromQt(QColor color);

    QColor GetColorFromUint(uint color);
};

#endif // COLORCONVERTER_H
