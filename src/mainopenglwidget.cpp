#include "mainopenglwidget.h"
#include "mainwidget.h"

#include <QString>

MainOpenGLWidget::MainOpenGLWidget(MainWidget *parent)
{
    m_emitters = new EmitterList(Q_NULLPTR);
    m_parent = parent;
    MPEmitter::m_widget = this;
}

MainOpenGLWidget::~MainOpenGLWidget()
{
    makeCurrent();
    m_manager->Destroy();
    m_manager = 0;
    m_device->Destroy();
    m_device = 0;
    m_program.release();
    m_textures.Destroy();
    ClearBuffers();
}

EmitterList *MainOpenGLWidget::GetEmitters()
{
    return m_emitters;
}

void MainOpenGLWidget::Refresh()
{
    m_textures.RefreshTextures();
    m_program.disableAttributeArray(QOpenGLShaderProgramAttribute::QOpenGLShaderProgramAttributeVertex);
    m_program.disableAttributeArray(QOpenGLShaderProgramAttribute::QOpenGLShaderProgramAttributeColor);
}

void MainOpenGLWidget::ClearBuffers()
{
    delete m_vertices;
    m_vertices = NULL;
    delete m_colors;
    m_colors = NULL;
    delete m_textureCoords;
    m_textureCoords = NULL;
    delete m_indexes;
    m_indexes = NULL;
}

void MainOpenGLWidget::Draw(int starting_index, int indexes_count, int max_vertices)
{
    m_program.bind();

    float screenscale_x = 2.f / (float)GetWidth();
    float screenscale_y = 2.f / (float)GetHeight();

    m_program.setUniformValue(m_program.uniform_screenscale, QPointF(screenscale_x, screenscale_y));
    m_program.setUniformValue(m_program.uniform_sampler, 0);

    m_program.enableAttributeArray(QOpenGLShaderProgramAttribute::QOpenGLShaderProgramAttributeVertex);
    m_program.setAttributeArray(QOpenGLShaderProgramAttribute::QOpenGLShaderProgramAttributeVertex, GL_FLOAT, m_vertices, 2);

    m_program.enableAttributeArray(QOpenGLShaderProgramAttribute::QOpenGLShaderProgramAttributeColor);
    m_program.setAttributeArray(QOpenGLShaderProgramAttribute::QOpenGLShaderProgramAttributeColor, GL_UNSIGNED_BYTE, m_colors, 4);

    m_program.enableAttributeArray(QOpenGLShaderProgramAttribute::QOpenGLShaderProgramAttributeTexturePosition);
    m_program.setAttributeArray(QOpenGLShaderProgramAttribute::QOpenGLShaderProgramAttributeTexturePosition, GL_FLOAT, m_textureCoords, 2);

    functions()->glDrawElements(GL_TRIANGLES, indexes_count, GL_UNSIGNED_SHORT, m_indexes);
}

void MainOpenGLWidget::Update()
{
    m_manager->UpdateByTimer();
    update();
}

uint MainOpenGLWidget::GetId()
{
    uint newId = 0;
    for (int i = 0; i < occupiedIdentificatos.count(); i++) {
        uint currentId = occupiedIdentificatos.at(i);
        if (newId < currentId) {
            newId = currentId;
        }
    }

    newId++;

    return newId;
}

void MainOpenGLWidget::initializeGL()
{
    functions()->initializeOpenGLFunctions();
    functions()->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_program.Create();

    g_camera = new Camera(GetWidth(), GetHeight());

    g_debugDraw.Create(g_camera, functions());

    initializeMP();

    m_isInitialized = true;
}

void MainOpenGLWidget::initializeMP()
{
    int width = GetWidth();
    int height = GetHeight();

    m_device = new MP_Device(width, height);
    m_device->Create();

    m_manager = &MP_Manager::GetInstance();
    m_manager->device = m_device;
    m_manager->m_program = &m_program;
    m_manager->m_functions = functions();
    m_manager->m_textures = &m_textures;

    MP_Platform *platform = new MP_Platform_WIN_POSIX;
    MAGIC_AXIS_ENUM axis = MAGIC_pXnY;

    bool filters[MAGIC_RENDER_STATE__MAX];
    for (int i = 0; i < MAGIC_RENDER_STATE__MAX; i++)
        filters[i] = false;
    filters[MAGIC_RENDER_STATE_BLENDING] = true;
    filters[MAGIC_RENDER_STATE_TEXTURE] = true;
    filters[MAGIC_RENDER_STATE_ADDRESS_U] = true;
    filters[MAGIC_RENDER_STATE_ADDRESS_V] = true;
    filters[MAGIC_RENDER_STATE_ZENABLE] = true;
    filters[MAGIC_RENDER_STATE_ZWRITE] = true;

    m_manager->Initialization(filters, true, axis, platform, MAGIC_INTERPOLATION_ENABLE, MAGIC_CHANGE_EMITTER_DEFAULT, 1024, 1024, 1, 1.f, 0.1f, true);

    m_manager->LoadAllEmitters();

    m_manager->RefreshAtlas();

    m_manager->CloseFiles();

    m_manager->Stop();

    HM_EMITTER hmEmitter = m_manager->GetFirstEmitter();

    while (hmEmitter) {
        Emitter *addEmitter = m_manager->GetEmitter(hmEmitter);
        m_emitters->add(addEmitter);
        occupiedIdentificatos.append(addEmitter->GetId());
        Magic_CorrectEmitterPosition(hmEmitter, width, height);
        hmEmitter = m_manager->GetNextEmitter(hmEmitter);
    }

    uint AvailableId = GetId();

    B2Emitter *emitter = new B2Emitter(AvailableId, "Figures", g_debugDraw); m_emitters->add(emitter);
//    emitter->g_debugDraw = g_debugDraw;

    MainWidget *widget = (MainWidget *)m_parent;
    widget->m_emittersList->setCurrentItemIndex(m_emitters->selectItem());
    m_emitters->selectItem(m_emitters->index(0, 0));
}

// ?
void MainOpenGLWidget::resizeGL(int w, int h)
{
    if (m_isInitialized) {
        qreal aspect = qreal(w) / qreal(h ? h : 1);
        const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;
        m_projection.setToIdentity();
        m_projection.perspective(fov, aspect, zNear, zFar);

        m_device->window_width = w;
        m_device->window_height = h;
    }
}

void MainOpenGLWidget::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e)

    m_manager->UpdateByTimer();
    update();
}

void MainOpenGLWidget::keyPressEvent(QKeyEvent *e)
{
    if (m_parent->m_emittersList->selection() != Q_NULLPTR) {
        m_parent->m_currentEmitter = m_parent->m_emittersList->selection()->GetId();
        switch (e->key()) {
        case Qt::Key_Left: {
            MAGIC_POSITION pos;
            Magic_GetEmitterPosition(m_parent->m_currentEmitter, &pos);
            pos.x -= 1;
            Magic_SetEmitterPosition(m_parent->m_currentEmitter, &pos);
            break;
        }
        case Qt::Key_Right: {
            MAGIC_POSITION pos;
            Magic_GetEmitterPosition(m_parent->m_currentEmitter, &pos);
            pos.x += 1;
            Magic_SetEmitterPosition(m_parent->m_currentEmitter, &pos);
            break;
        }
        case Qt::Key_Down: {
            MAGIC_POSITION pos;
            Magic_GetEmitterPosition(m_parent->m_currentEmitter, &pos);
            pos.y -= 1;
            Magic_SetEmitterPosition(m_parent->m_currentEmitter, &pos);
            break;
        }
        case Qt::Key_Up: {
            MAGIC_POSITION pos;
            Magic_GetEmitterPosition(m_parent->m_currentEmitter, &pos);
            pos.y += 1;
            Magic_SetEmitterPosition(m_parent->m_currentEmitter, &pos);
            break;
        }
        }
    }
}

void MainOpenGLWidget::paintGL()
{
    functions()->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    functions()->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QOpenGLPaintDevice *device = ((QOpenGLPaintDevice *)getPaintDevice());

    if (device->size() != bufferSize()) {
        device->setSize(bufferSize());
        m_painter = new QPainter(device);
    }
    if (!m_painter) {
        m_painter = new QPainter(device);
    }

    m_painter->beginNativePainting();

    if (!m_emitters->isEmpty() && m_emitters->selectItem().isValid()) {
        if (m_emitters->get(m_emitters->selectItem())->GetType() == EmitterType::MP) {
            m_manager->Render();
        }
        else if (m_emitters->get(m_emitters->selectItem())->GetType() == EmitterType::B2) {
            B2Emitter *emitter = ((B2Emitter *)m_emitters->get(m_emitters->selectItem()));
            emitter->g_debugDraw = this->g_debugDraw;
            emitter->m_painter = this->m_painter;
            emitter->Step(&settings);
        }
    }

    m_program.setUniformValue(m_program.uniform_screenscale, m_projection);

    m_painter->endNativePainting();
}

