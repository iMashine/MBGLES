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

/// mainwidget of application
/// this widget will show controllers and paint effects on paint surface (openglsurface)
class MainWidget : public QWidget
{
    Q_OBJECT

public:

    /// fucntions block
    ///

    MainWidget(QWidget *parent = 0);

    void Screenshot();

    void CreateUI();

    void refreshUI();

    ~MainWidget();

    /// events block
    ///

    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void timerEvent(QTimerEvent *e) override;

    void paintEvent(QPaintEvent *event) override;

    /// members block
    ///

    // opengl surface when will be proccess render
    MainOpenGLWidget *m_mainOpenGLWidget = nullptr;

    QGraphicsScene *m_scene;
    QGraphicsView *m_hiddenView;
    QOpenGLContext *m_context;

    QGroupBox *m_mainMenu = nullptr;
    QVBoxLayout *m_mainMenuLayout = nullptr;

    EmittersListView *m_emittersList = nullptr;

    QListView *m_obstaclesList = nullptr;

    QSplitter *m_mainLayout = nullptr;

    unsigned int m_currentEmitter = false;

private:

    QSlider *m_particlesSpeedSettingSlider;

    QSlider *m_particlesSizeSettingSlider;

    QSlider *m_particlesSaturationSettingSlider;

    QSlider *m_particlesLifeSettingSlider;

    QSlider *m_particlesWeightSettingSlider;

    QSlider *m_particlesTransparencySettingSlider;

    QPushButton *m_particlesColorSettingButton;

    bool isCreatedInterface = false;

    QBasicTimer timer;
};

#endif // MAINWIDGET_H
