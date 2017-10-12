#ifndef MAINWIDGET_H
#define MAINWIDGET_H

//#include "emitter.h"
#include "emitter/emitterslistview.h"
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
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QBasicTimer>

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);

    void Screenshot();

    void CreateUI();

    ~MainWidget();

    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void timerEvent(QTimerEvent *e) override;

    void paintEvent(QPaintEvent *event) override;

    MainOpenGLWidget *m_mainOpenGLWidget = nullptr;

    QGraphicsScene *m_scene;
    QGraphicsView *m_hiddenView;
    QOpenGLContext *m_context;

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

    bool isCreatedInterface = false;

    QBasicTimer timer;
};

#endif // MAINWIDGET_H
