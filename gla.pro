TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lGLEW -lGL -lGLU -lSDL2

HEADERS +=  gla/shader.h \
            gla/global.h \
            gla/camera.h \
            gla/transformation.h \
            gla/arraybuffer.h \
            gla/vertexarrayobject.h \
            gla/gla.h \
            gla/texture.h \
            gla/stb/stb_image.h \
            gla/solids.h \
            gla/exceptions.h \
            gla/timer.h \
            gla/skeleton.h \
            gla/transformsequence.h \
            gla/bone.h \
            gla/modelloader.h \
            gla/mesh.h \
            gla/types.h \
            gla/gputexturearray.h \
            gla/utils/window.h \
            gla/utils/event.h \
            gla/utils/app.h \
    gla/utils/utils.h \
    gla/engine/scene.h \
    gla/engine/node.h \
    gla/engine/engine.h \
     gla/engine/material.h


SOURCES +=  main.cpp \
            src/shader.cpp \
            src/transformation.cpp \
            src/texture.cpp \
            src/solids.cpp \
            test.cpp \
            gla/src/events.cpp \
            src/skeleton.cpp \
            src/transformsequence.cpp \
            src/bone.cpp \
            src/modelloader.cpp \
            src/mesh.cpp \
            src/gputexturearray.cpp \
            src/utils/window.cpp \
    src/engine/scene.cpp \
    src/engine/node.cpp \
    src/engine/material.cpp \
    TextureArray.cpp \
    HelloTriangle.cpp


INCLUDEPATH += ./ 

QMAKE_CXXFLAGS += -std=c++11 -w -O3

OTHER_FILES += \ 
    CMakeLists.txt \
    shaders/Basic_PNCU.v \
    shaders/Basic_PNCU.f \
    shaders/Line_PC.v \
    shaders/Line_PC.f \
    resources/ExtraInfo.json

DISTFILES += \
    Doxygen.cfg \
    shaders/PNU_TextureArray.f \
    shaders/PNU_TextureArray.v \
    shaders/PNU_TriPlanar.f \
    shaders/PNU_TriPlanar.v \
    shaders/HelloTriangle.f \
    shaders/HelloTriangle.v


