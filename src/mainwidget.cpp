#include "mainopengloffscreensurface.h"
#include "mainwidget.h"

#include <QOpenGLFramebufferObjectFormat>
#include <windows.h>

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    CreateUI();
    timer.start(12, this);
}

void MainWidget::CreateUI()
{
    m_mainOpenGLWidget = new MainOpenGLWidget(this);
    m_mainOpenGLWidget->resize(800, 600);
    m_mainOpenGLWidget->render();
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

    /// меню настройки прозрачности частиц
    {
        m_particlesTransparencySettingSlider = new QSlider(Qt::Horizontal);
        m_particlesTransparencySettingSlider->setMinimum(1);
        m_particlesTransparencySettingSlider->setMaximum(100);
        m_particlesTransparencySettingSlider->setTickPosition(QSlider::TicksLeft);

        connect(m_particlesTransparencySettingSlider, &QSlider::valueChanged, [ = ](const int newValue) {
            if (m_emittersList->selection() != Q_NULLPTR) {
                m_emittersList->selection()->SetParticlesTransparency(newValue);
            }
        });

        QHBoxLayout *settingsTransparencyLayout = new QHBoxLayout();
        settingsTransparencyLayout->addWidget(m_particlesTransparencySettingSlider);

        QGroupBox *transparencySetting = new QGroupBox("Прозрачность");
        transparencySetting->setLayout(settingsTransparencyLayout);

        m_mainMenuLayout->addWidget(transparencySetting);
    }

    /// меню настройки цвета эффекта
    {
        m_particlesColorSettingButton = new QPushButton();

        connect(m_particlesColorSettingButton, &QPushButton::clicked, [ = ] {
            QColor m_currentColor = QColorDialog::getColor();

            QString qss = QString("background-color: %1").arg(m_currentColor.name());
            m_particlesColorSettingButton->setStyleSheet(qss);

            if (m_emittersList->selection() != Q_NULLPTR)
            {
                int color;
                color = m_currentColor.alpha();
                color = (color << 8) + m_currentColor.blue();
                color = (color << 8) + m_currentColor.green();
                color = (color << 8) + m_currentColor.red();
                m_emittersList->selection()->SetParticlesColor(color);
            }
        });

        m_mainMenuLayout->addWidget(m_particlesColorSettingButton);
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

    QWidget *widget = new QWidget();
    widget->resize(800, 600);
    m_mainLayout->addWidget(widget);
    m_mainLayout->addWidget(m_mainMenu);

    isCreatedInterface = true;
}

MainWidget::~MainWidget()
{

}

void MainWidget::resizeEvent(QResizeEvent *event)
{
    if (isCreatedInterface) {
        m_mainLayout->resize(event->size());
    }
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
    if (isCreatedInterface) {
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
}

void MainWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (isCreatedInterface) {
        m_currentEmitter = -1;
    }
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (isCreatedInterface) {
        if (m_currentEmitter != -1) {
            ((MPEmitter *)m_mainOpenGLWidget->GetEmitters()->get(m_currentEmitter))->
            SetPosition(QPointF(event->pos().x(), m_mainOpenGLWidget->GetHeight() - event->pos().y()));
        }
    }
}

void MainWidget::timerEvent(QTimerEvent *e)
{
    update();
}

void MainWidget::paintEvent(QPaintEvent *event)
{
    m_mainOpenGLWidget->Update();
    QPainter painter;
    painter.begin(this);
    QRectF target(0.0, 0.0, 800.0, 600.0);
    QRectF source(0.0, 0.0, 800.0, 600.0);
    QImage image = m_mainOpenGLWidget->grabFramebuffer();
    painter.drawImage(target, image, source);
}

void MainWidget::refreshUI()
{
    if (isCreatedInterface) {
        if (m_emittersList->selection() != Q_NULLPTR) {
            m_currentEmitter = m_emittersList->selection()->GetId();
            m_particlesSpeedSettingSlider->setValue(m_emittersList->selection()->GetParticlesSpeed());
            m_particlesSizeSettingSlider->setValue(m_emittersList->selection()->GetParticlesSize());
            m_particlesSaturationSettingSlider->setValue(m_emittersList->selection()->GetParticlesSaturation());
            m_particlesTransparencySettingSlider->setValue(m_emittersList->selection()->GetParticlesTransparency());

            QColor m_currentColor = m_emittersList->selection()->GetParticlesColor();

            int color;
            color = m_currentColor.alpha();
            color = (color << 8) + m_currentColor.blue();
            color = (color << 8) + m_currentColor.green();
            color = (color << 8) + m_currentColor.red();

            m_currentColor = QColor(color);

            QString qss = QString("background-color: %1").arg(m_currentColor.name());
            m_particlesColorSettingButton->setStyleSheet(qss);
        };
    }
    m_currentEmitter = -1;
}
