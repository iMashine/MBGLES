#-------------------------------------------------
#
# Project created by QtCreator 2017-08-07T10:42:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MPGLES
TEMPLATE = app

CONFIG += warn_off

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += include/_core
INCLUDEPATH += include/opengl
INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

LIBS += -L$$PWD/lib/ -lmagic
LIBS += -L$$PWD/lib/ -lBox2D
LIBS += -lOpengl32

PRE_TARGETDEPS += $$PWD/lib/magic.lib
PRE_TARGETDEPS += $$PWD/lib/libmagic.a
PRE_TARGETDEPS += $$PWD/lib/libBox2D.a

HEADERS += \
    include/_core/magic.h \
    include/_core/mp.h \
    include/_core/platform_win_posix.h \
    include/Box2D/Collision/Shapes/b2ChainShape.h \
    include/Box2D/Collision/Shapes/b2CircleShape.h \
    include/Box2D/Collision/Shapes/b2EdgeShape.h \
    include/Box2D/Collision/Shapes/b2PolygonShape.h \
    include/Box2D/Collision/Shapes/b2Shape.h \
    include/Box2D/Collision/b2BroadPhase.h \
    include/Box2D/Collision/b2Collision.h \
    include/Box2D/Collision/b2Distance.h \
    include/Box2D/Collision/b2DynamicTree.h \
    include/Box2D/Collision/b2TimeOfImpact.h \
    include/Box2D/Common/b2BlockAllocator.h \
    include/Box2D/Common/b2Draw.h \
    include/Box2D/Common/b2GrowableStack.h \
    include/Box2D/Common/b2Math.h \
    include/Box2D/Common/b2Settings.h \
    include/Box2D/Common/b2StackAllocator.h \
    include/Box2D/Common/b2Timer.h \
    include/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.h \
    include/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.h \
    include/Box2D/Dynamics/Contacts/b2CircleContact.h \
    include/Box2D/Dynamics/Contacts/b2Contact.h \
    include/Box2D/Dynamics/Contacts/b2ContactSolver.h \
    include/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.h \
    include/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.h \
    include/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h \
    include/Box2D/Dynamics/Contacts/b2PolygonContact.h \
    include/Box2D/Dynamics/Joints/b2DistanceJoint.h \
    include/Box2D/Dynamics/Joints/b2FrictionJoint.h \
    include/Box2D/Dynamics/Joints/b2GearJoint.h \
    include/Box2D/Dynamics/Joints/b2Joint.h \
    include/Box2D/Dynamics/Joints/b2MotorJoint.h \
    include/Box2D/Dynamics/Joints/b2MouseJoint.h \
    include/Box2D/Dynamics/Joints/b2PrismaticJoint.h \
    include/Box2D/Dynamics/Joints/b2PulleyJoint.h \
    include/Box2D/Dynamics/Joints/b2RevoluteJoint.h \
    include/Box2D/Dynamics/Joints/b2RopeJoint.h \
    include/Box2D/Dynamics/Joints/b2WeldJoint.h \
    include/Box2D/Dynamics/Joints/b2WheelJoint.h \
    include/Box2D/Dynamics/b2Body.h \
    include/Box2D/Dynamics/b2ContactManager.h \
    include/Box2D/Dynamics/b2Fixture.h \
    include/Box2D/Dynamics/b2Island.h \
    include/Box2D/Dynamics/b2TimeStep.h \
    include/Box2D/Dynamics/b2World.h \
    include/Box2D/Dynamics/b2WorldCallbacks.h \
    include/Box2D/Rope/b2Rope.h \
    include/Box2D/Box2D.h \
    src/emitter/b2emitter.h \
    src/emitter/emitter.h \
    src/emitter/emitterlist.h \
    src/emitter/emitterslistview.h \
    src/emitter/mpemitter.h \
    src/camera.h \
    src/debugdraw.h \
     src/utils/floatrange.h \
    src/mainopenglwidget.h \
    src/mainwidget.h \
    src/ptextures.h \
    src/rendertriangles.h \
    src/shaderprogram.h

SOURCES += \
    include/_core/mp.cpp \
    include/_core/platform_win_posix.cpp \
    src/emitter/b2emitter.cpp \
    src/emitter/emitterlist.cpp \
    src/emitter/emitterslistview.cpp \
    src/emitter/mpemitter.cpp \
    src/camera.cpp \
    src/debugdraw.cpp \
    src/utils/floatrange.cpp \
    src/main.cpp \
    src/mainopenglwidget.cpp \
    src/mainwidget.cpp \
    src/ptextures.cpp \
    src/rendertriangles.cpp \
    src/shaderprogram.cpp


