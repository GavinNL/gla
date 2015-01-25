TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lGLEW -lGL -lGLU -lSDL2

HEADERS +=  glre/mesh.h \
            glre/shader.h \
            glre/global.h \
            glre/camera.h \
    glre/transformation.h \
    glre/line.h \
    glre/arraybuffer.h


SOURCES += src/mesh.cpp \
	   main.cpp \
    src/shader.cpp \
    src/camera.cpp \
    src/transformation.cpp \
    src/line.cpp


INCLUDEPATH += ./ 

QMAKE_CXXFLAGS += -std=c++11 -w -O3

OTHER_FILES += \ 
    CMakeLists.txt \


