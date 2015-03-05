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
    glre/utils/window.h \
    glre/utils/event.h \
    glre/skeleton.h \
    glre/transformsequence.h \
    glre/bone.h \
    glre/modelloader.h \
    glre/mesh.h


SOURCES +=  main.cpp \
    src/shader.cpp \
    src/transformation.cpp \
    src/texture.cpp \
    src/solids.cpp \
    test.cpp \
    src/utils/window.cpp \
    glre/src/events.cpp \
    src/skeleton.cpp \
    src/transformsequence.cpp \
    src/bone.cpp \
    src/modelloader.cpp \
    src/mesh.cpp


INCLUDEPATH += ./ 

QMAKE_CXXFLAGS += -std=c++11 -w -O3

OTHER_FILES += \ 
    CMakeLists.txt \
    shaders/Basic_PNCU.v \
    shaders/Basic_PNCU.f \
    shaders/Line_PC.v \
    shaders/Line_PC.f \
    resources/ExtraInfo.json \
    resources/marble.jpg


