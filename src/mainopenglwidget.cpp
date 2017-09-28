#include "mainopenglwidget.h"
#include "mainwidget.h"

MainOpenGLWidget::MainOpenGLWidget(MainWidget *parent)
{
    m_emitters = new EmitterList(Q_NULLPTR);
    m_parent = parent;
    MPEmitter::m_widget = this;
    setFocusPolicy(Qt::StrongFocus);
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

    float screenscale_x = 2.f / (float)width();
    float screenscale_y = 2.f / (float)height();

    m_program.setUniformValue(m_program.uniform_screenscale, QPointF(screenscale_x, screenscale_y));
    m_program.setUniformValue(m_program.uniform_sampler, 0);

    m_program.enableAttributeArray(QOpenGLShaderProgramAttribute::QOpenGLShaderProgramAttributeVertex);
    m_program.setAttributeArray(QOpenGLShaderProgramAttribute::QOpenGLShaderProgramAttributeVertex, GL_FLOAT, m_vertices, 2);

    m_program.enableAttributeArray(QOpenGLShaderProgramAttribute::QOpenGLShaderProgramAttributeColor);
    m_program.setAttributeArray(QOpenGLShaderProgramAttribute::QOpenGLShaderProgramAttributeColor, GL_UNSIGNED_BYTE, m_colors, 4);

    m_program.enableAttributeArray(QOpenGLShaderProgramAttribute::QOpenGLShaderProgramAttributeTexturePosition);
    m_program.setAttributeArray(QOpenGLShaderProgramAttribute::QOpenGLShaderProgramAttributeTexturePosition, GL_FLOAT, m_textureCoords, 2);

    m_functions.glDrawElements(GL_TRIANGLES, indexes_count, GL_UNSIGNED_SHORT, m_indexes);
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
    makeCurrent();
    m_functions.initializeOpenGLFunctions();
    m_functions.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    m_program.Create();
    initializeMP();
    timer.start(12, this);
}

void MainOpenGLWidget::initializeMP()
{
    m_device = new MP_Device(width(), height());
    m_device->Create();

    m_manager = &MP_Manager::GetInstance();
    m_manager->device = m_device;
    m_manager->m_program = &m_program;
    m_manager->m_functions = &m_functions;
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
        int width = this->width();
        int height = this->height();
        Magic_CorrectEmitterPosition(hmEmitter, width, height);
        hmEmitter = m_manager->GetNextEmitter(hmEmitter);
    }

    uint AvailableId = GetId();

//    B2Emitter *emitter = new CharacterCollision();
//    emitter->m_id = AvailableId;
//    emitter->m_name = "Character Collision";
//    m_emitters->add(emitter);

//    g_testEntries = {
//        {AvailableId++, "Character Collision", CharacterCollision::Create},
//        {AvailableId++, "Tiles", Tiles::Create},
//        {AvailableId++, "Heavy on Light", HeavyOnLight::Create},
//        {AvailableId++, "Heavy on Light Two", HeavyOnLightTwo::Create},
//        {AvailableId++, "Vertical Stack", VerticalStack::Create},
//        {AvailableId++, "Basic Slider Crank", BasicSliderCrank::Create},
//        {AvailableId++, "Slider Crank", SliderCrank::Create},
//        {AvailableId++, "Sphere Stack", SphereStack::Create},
//        {AvailableId++, "Convex Hull", ConvexHull::Create},
//        {AvailableId++, "Tumbler", Tumbler::Create},
//        {AvailableId++, "Ray-Cast", RayCast::Create},
//        {AvailableId++, "Dump Shell", DumpShell::Create},
//        {AvailableId++, "Apply Force", ApplyForce::Create},
//        {AvailableId++, "Continuous Test", ContinuousTest::Create},
//        {AvailableId++, "Time of Impact", TimeOfImpact::Create},
//        {AvailableId++, "Motor Joint", MotorJoint::Create},
//        {AvailableId++, "One-Sided Platform", OneSidedPlatform::Create},
//        {AvailableId++, "Mobile", Mobile::Create},
//        {AvailableId++, "MobileBalanced", MobileBalanced::Create},
//        {AvailableId++, "Conveyor Belt", ConveyorBelt::Create},
//        {AvailableId++, "Gears", Gears::Create},
//        {AvailableId++, "Varying Restitution", VaryingRestitution::Create},
//        {AvailableId++, "Cantilever", Cantilever::Create},
//        {AvailableId++, "Edge Test", EdgeTest::Create},
//        {AvailableId++, "Body Types", BodyTypes::Create},
//        {AvailableId++, "Shape Editing", ShapeEditing::Create},
//        {AvailableId++, "Car", Car::Create},
//        {AvailableId++, "Prismatic", Prismatic::Create},
//        {AvailableId++, "Revolute", Revolute::Create},
//        {AvailableId++, "Pulleys", Pulleys::Create},
//        {AvailableId++, "Polygon Shapes", PolyShapes::Create},
//        {AvailableId++, "Web", Web::Create},
//        {AvailableId++, "RopeJoint", RopeJoint::Create},
//        {AvailableId++, "Pinball", Pinball::Create},
//        {AvailableId++, "Bullet Test", BulletTest::Create},
//        {AvailableId++, "Confined", Confined::Create},
//        {AvailableId++, "Pyramid", Pyramid::Create},
//        {AvailableId++, "Theo Jansen's Walker", TheoJansen::Create},
//        {AvailableId++, "Edge Shapes", EdgeShapes::Create},
//        {AvailableId++, "PolyCollision", PolyCollision::Create},
//        {AvailableId++, "Bridge", Bridge::Create},
//        {AvailableId++, "Breakable", Breakable::Create},
//        {AvailableId++, "Chain", Chain::Create},
//        {AvailableId++, "Collision Filtering", CollisionFiltering::Create},
//        {AvailableId++, "Collision Processing", CollisionProcessing::Create},
//        {AvailableId++, "Compound Shapes", CompoundShapes::Create},
//        {AvailableId++, "Distance Test", DistanceTest::Create},
//        {AvailableId++, "Dominos", Dominos::Create},
//        {AvailableId++, "Dynamic Tree", DynamicTreeTest::Create},
//        {AvailableId++, "Sensor Test", SensorTest::Create},
//        {AvailableId++, "Varying Friction", VaryingFriction::Create},
//        {AvailableId++, "Add Pair Stress Test", AddPair::Create}
//    };

//    for (int i = 0; i < g_testEntries.count(); i++) {
//        B2Emitter *emitter = g_testEntries[i].createFcn();
//        emitter->m_id = g_testEntries[i].id;
//        emitter->m_name = g_testEntries[i].name;
//        m_emitters->add(emitter);
//    }

    MainWidget *widget = (MainWidget *)m_parent;
    widget->m_emittersList->setCurrentItemIndex(m_emitters->selectItem());
}

// ?
void MainOpenGLWidget::resizeGL(int w, int h)
{
    qreal aspect = qreal(w) / qreal(h ? h : 1);
    const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;
    m_projection.setToIdentity();
    m_projection.perspective(fov, aspect, zNear, zFar);

    m_device->window_width = w;
    m_device->window_height = h;
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
    makeCurrent();

    m_functions.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    m_functions.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (!m_painter || m_painter->window().width() != width() ||
            m_painter->window().height() != height()) {
        delete m_painter;
        m_mainGLPaintDevice = new QOpenGLPaintDevice(width(), height());
        m_painter = new QPainter(m_mainGLPaintDevice);
    }

    m_painter->beginNativePainting();

    m_manager->Render();

    m_program.setUniformValue(m_program.uniform_screenscale, m_projection);

    m_painter->endNativePainting();
}

