TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lGLEW -lGL -lGLU -lSDL2

HEADERS +=  glre/shader.h \
            glre/global.h \
            glre/camera.h \
            glre/transformation.h \
            glre/arraybuffer.h \
            glre/vertexarrayobject.h \
            glre/glre.h \
            glre/texture.h \
            glre/stb/stb_image.h \
            glre/solids.h \
            glre/exceptions.h \
            glre/timer.h \
            glre/skeleton.h \
            glre/transformsequence.h \
            glre/bone.h \
            glre/modelloader.h \
            glre/mesh.h \
            glre/types.h \
            glre/gputexturearray.h \
            glre/utils/window.h \
            glre/utils/event.h \
            glre/utils/app.h \
    glre/utils/utils.h \
    glre/engine/scene.h \
    glre/engine/node.h \
    glre/engine/engine.h \
    glre/engine/material.h


SOURCES +=  main.cpp \
            src/shader.cpp \
            src/transformation.cpp \
            src/texture.cpp \
            src/solids.cpp \
            test.cpp \
            glre/src/events.cpp \
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
    TextureArray.cpp


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
    shaders/PNU_TriPlanar.v


