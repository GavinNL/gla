TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lGLEW -lGL -lGLU -lSDL2

HEADERS +=  gla/gla.h                          \
            gla/shader.h                  \
            gla/global.h                  \
            gla/camera.h                  \
            gla/transformation.h          \
            gla/arraybuffer.h             \
            gla/vertexarrayobject.h       \
            gla/exceptions.h              \
            gla/texture.h                 \
            gla/types.h                   \
            gla/gputexturearray.h         \
            gla/timer.h                   \
            gla/framebufferobject.h       \
            gla/uniformbuffer.h       \
            gla/solids.h \
            gla/utils/app.h \
            gla/utils/cameracontrol.h \
    gla/memoryalignedtuple.h


SOURCES +=  main.cpp                                     \
            examples/TextureArray.cpp                             \
            examples/HelloTriangle.cpp                            \
            examples/HelloTriangleVAO.cpp                         \
            examples/Textures.cpp                                 \
            examples/UniformBuffer.cpp                                 \
            examples/TransformSequence.cpp                        \
            examples/HelloTriangleEncapsulatedVAO.cpp             \
            examples/engine.cpp             \
            test.cpp

INCLUDEPATH += ./ 

QMAKE_CXXFLAGS += -std=c++11 -w -O3

OTHER_FILES +=                         \
    CMakeLists.txt                     \
    shaders/Basic_PNCU.v               \
    shaders/Basic_PNCU.f               \
    shaders/Line_PC.v                  \
    shaders/Line_PC.f                  \
    resources/ExtraInfo.json

DISTFILES +=                           \
    Doxygen.cfg                        \
    shaders/PNU_TextureArray.f         \
    shaders/PNU_TextureArray.v         \
    shaders/UniformBuffer.f            \
    shaders/UniformBuffer.v            \
    shaders/TextureArray.f             \
    shaders/TextureArray.v             \
    shaders/PNU_TriPlanar.f            \
    shaders/PNU_TriPlanar.v            \
    shaders/HelloTriangle.f            \
    shaders/HelloTriangle.v            \
    shaders/Textures.f                 \
    shaders/Textures.v                 \
    shaders/Standard_PUN.f            \
    shaders/Standard_PUN.v            \



