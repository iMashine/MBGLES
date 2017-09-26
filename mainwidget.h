#ifndef MAINWIDGET_H
#define MAINWIDGET_H

//#include "emitter.h"
#include "emitterslistview.h"
#include "mainopenglwidget.h"
//#include "obstacle.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListView>
#include <QSplitter>
#include <QLabel>
#include <QGroupBox>
#include <QResizeEvent>
#include <QLineEdit>
#include <QCheckBox>
#include <QPalette>
#include <QComboBox>
#include <QList>
#include <QSlider>
#include <QColor>
#include <QColorDialog>

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);

    ~MainWidget();

    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void keyPressEvent(QKeyEvent *event);

    MainOpenGLWidget *m_mainOpenGLWidget = nullptr;

    QGroupBox *m_mainMenu = nullptr;
    QVBoxLayout *m_mainMenuLayout = nullptr;

    // список эмиттеров
    EmittersListView *m_emittersList = nullptr;

    // список препятствий
    QListView *m_obstaclesList = nullptr;

    QSplitter *m_mainLayout = nullptr;

    void refreshUI();

    unsigned int m_currentEmitter = false;

private:

    // скорость
    QSlider *m_particlesSpeedSettingSlider;

    // размер
    QSlider *m_particlesSizeSettingSlider;

    // насыщенность
    QSlider *m_particlesSaturationSettingSlider;

    // время жизни
    QSlider *m_particlesLifeSettingSlider;

    // вес
    QSlider *m_particlesWeightSettingSlider;
};

#endif // MAINWIDGET_H
