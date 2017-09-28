#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    m_mainOpenGLWidget = new MainOpenGLWidget(this);
    m_mainMenu = new QGroupBox("Меню");
    m_mainMenuLayout = new QVBoxLayout();
    m_mainLayout = new QSplitter(this);
    m_mainLayout->resize(this->size());
    m_obstaclesList = new QListView();

    /// меню настройки количества - насыщенности частиц
    {
        m_particlesSaturationSettingSlider = new QSlider(Qt::Horizontal);
        m_particlesSaturationSettingSlider->setMinimum(1);
        m_particlesSaturationSettingSlider->setMaximum(100);
        m_particlesSaturationSettingSlider->setTickPosition(QSlider::TicksLeft);

        connect(m_particlesSaturationSettingSlider, &QSlider::valueChanged, [ = ](const int newValue) {
            if (m_emittersList->selection() != Q_NULLPTR) {
                m_emittersList->selection()->SetParticlesSaturation(newValue);
            }
        });

        QHBoxLayout *settingsSaturationLayout = new QHBoxLayout();
        settingsSaturationLayout->addWidget(m_particlesSaturationSettingSlider);

        QGroupBox *saturationSetting = new QGroupBox("Насыщенность");
        saturationSetting->setLayout(settingsSaturationLayout);

        m_mainMenuLayout->addWidget(saturationSetting);
    }

    /// меню настройки размера частиц
    {
        m_particlesSizeSettingSlider = new QSlider(Qt::Horizontal);
        m_particlesSizeSettingSlider->setMinimum(1);
        m_particlesSizeSettingSlider->setMaximum(100);
        m_particlesSizeSettingSlider->setTickPosition(QSlider::TicksLeft);

        connect(m_particlesSizeSettingSlider, &QSlider::valueChanged, [ = ](const int newValue) {
            if (m_emittersList->selection() != Q_NULLPTR) {
                m_emittersList->selection()->SetParticlesSize(newValue);
            }
        });

        QHBoxLayout *settingsSizeLayout = new QHBoxLayout();
        settingsSizeLayout->addWidget(m_particlesSizeSettingSlider);

        QGroupBox *sizeSetting = new QGroupBox("Размер");
        sizeSetting->setLayout(settingsSizeLayout);

        m_mainMenuLayout->addWidget(sizeSetting);
    }

    /// меню настройки скорости частиц
    {
        m_particlesSpeedSettingSlider = new QSlider(Qt::Horizontal);
        m_particlesSpeedSettingSlider->setMinimum(1);
        m_particlesSpeedSettingSlider->setMaximum(100);
        m_particlesSpeedSettingSlider->setTickPosition(QSlider::TicksLeft);

        connect(m_particlesSpeedSettingSlider, &QSlider::valueChanged, [ = ](const int newValue) {
            if (m_emittersList->selection() != Q_NULLPTR) {
                m_emittersList->selection()->SetParticlesSpeed(newValue);
            }
        });

        QHBoxLayout *settingsSpeedLayout = new QHBoxLayout();
        settingsSpeedLayout->addWidget(m_particlesSpeedSettingSlider);

        QGroupBox *speedSetting = new QGroupBox("Скорость");
        speedSetting->setLayout(settingsSpeedLayout);

        m_mainMenuLayout->addWidget(speedSetting);
    }

    /// меню настройки время жизни частиц
    {
        m_particlesLifeSettingSlider = new QSlider(Qt::Horizontal);
        m_particlesLifeSettingSlider->setMinimum(1);
        m_particlesLifeSettingSlider->setMaximum(100);
        m_particlesLifeSettingSlider->setTickPosition(QSlider::TicksLeft);

        connect(m_particlesLifeSettingSlider, &QSlider::valueChanged, [ = ](const int newValue) {
            if (m_emittersList->selection() != Q_NULLPTR) {
//                m_emittersList->selection()->SetParticlesLife(newValue);
            }
        });

        QHBoxLayout *settingsLifeLayout = new QHBoxLayout();
        settingsLifeLayout->addWidget(m_particlesLifeSettingSlider);

        QGroupBox *lifeSetting = new QGroupBox("Время жизни");
        lifeSetting->setLayout(settingsLifeLayout);

        m_mainMenuLayout->addWidget(lifeSetting);
    }

    /// меню настройки веса частиц
    {
        m_particlesWeightSettingSlider = new QSlider(Qt::Horizontal);
        m_particlesWeightSettingSlider->setMinimum(1);
        m_particlesWeightSettingSlider->setMaximum(100);
        m_particlesWeightSettingSlider->setTickPosition(QSlider::TicksLeft);

        connect(m_particlesWeightSettingSlider, &QSlider::valueChanged, [ = ](const int newValue) {
            if (m_emittersList->selection() != Q_NULLPTR) {
//                m_emittersList->selection()->SetParticlesWeight(newValue);
            }
        });

        QHBoxLayout *settingsWeightLayout = new QHBoxLayout();
        settingsWeightLayout->addWidget(m_particlesWeightSettingSlider);

        QGroupBox *weightSetting = new QGroupBox("Вес");
        weightSetting->setLayout(settingsWeightLayout);

        m_mainMenuLayout->addWidget(weightSetting);
    }

    /// меню работы с эмиттерами
    {
        QPushButton *addEmitterButton = new QPushButton("Добавить");
        QPushButton *removeEmitterButton = new QPushButton("Удалить");
        QHBoxLayout *buttonsLayout = new QHBoxLayout();
        buttonsLayout->addWidget(addEmitterButton);
        buttonsLayout->addWidget(removeEmitterButton);

        m_emittersList = new EmittersListView(m_mainOpenGLWidget->GetEmitters(), this);

        QVBoxLayout *settingsfillTypeLayout = new QVBoxLayout();

        settingsfillTypeLayout->addWidget(m_emittersList);
        settingsfillTypeLayout->addLayout(buttonsLayout);

        QGroupBox *fillTypeSetting = new QGroupBox("Эмиттер");
        fillTypeSetting->setLayout(settingsfillTypeLayout);

        m_mainMenuLayout->addWidget(fillTypeSetting);
    }

    /// меню работы с препятствиями
    {
        QPushButton *addEmitterButton = new QPushButton("Добавить");
        QPushButton *removeEmitterButton = new QPushButton("Удалить");
        QHBoxLayout *buttonsLayout = new QHBoxLayout();
        buttonsLayout->addWidget(addEmitterButton);
        buttonsLayout->addWidget(removeEmitterButton);

        QVBoxLayout *settingsfillTypeLayout = new QVBoxLayout();

        settingsfillTypeLayout->addWidget(m_obstaclesList);
        settingsfillTypeLayout->addLayout(buttonsLayout);

        QGroupBox *fillTypeSetting = new QGroupBox("Препятствия");
        fillTypeSetting->setLayout(settingsfillTypeLayout);

        m_mainMenuLayout->addWidget(fillTypeSetting);
    }


    m_mainMenu->setLayout(m_mainMenuLayout);

    m_mainLayout->addWidget(m_mainOpenGLWidget);
    m_mainLayout->addWidget(m_mainMenu);
}

MainWidget::~MainWidget()
{

}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    m_mainLayout->resize(event->size());
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
    m_currentEmitter = -1;

    if (m_emittersList->selection() != Q_NULLPTR) {
        if (m_mainOpenGLWidget->GetEmitters()->first() != Q_NULLPTR) {
            MPEmitter *emitter = (MPEmitter *)m_mainOpenGLWidget->GetEmitters()->first();
            MPEmitter *firstEmitter = (MPEmitter *)m_mainOpenGLWidget->GetEmitters()->first();

            do {
                if (emitter->Contains(event->pos())) {
                    m_currentEmitter = emitter->GetId();
                    break;
                }
                emitter = (MPEmitter *)m_mainOpenGLWidget->GetEmitters()->next();
            }
            while (emitter != firstEmitter);

        }
    }
}

void MainWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_currentEmitter = -1;
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_currentEmitter != -1) {
        ((MPEmitter *)m_mainOpenGLWidget->GetEmitters()->get(m_currentEmitter))->
        SetPosition(QPointF(event->pos().x(), m_mainOpenGLWidget->GetHeight() - event->pos().y()));
    }
}

void MainWidget::refreshUI()
{
    if (m_emittersList->selection() != Q_NULLPTR) {
        m_currentEmitter = m_emittersList->selection()->GetId();
        m_particlesSpeedSettingSlider->setValue(m_emittersList->selection()->GetParticlesSpeed());
        m_particlesSizeSettingSlider->setValue(m_emittersList->selection()->GetParticlesSize());
        m_particlesSaturationSettingSlider->setValue(m_emittersList->selection()->GetParticlesSaturation());
//        m_particlesLifeSettingSlider->setValue(m_emittersList->selection()->GetParticlesLife());
//        m_particlesWeightSettingSlider->setValue(m_emittersList->selection()->GetParticlesWeight());
    }
    m_currentEmitter = -1;
}
