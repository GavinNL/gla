TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lGLEW -lGL -lGLU -lSDL2

HEADERS +=  gla/gla.h                          \
            gla/core/shader.h                  \
            gla/core/global.h                  \
            gla/core/camera.h                  \
            gla/core/transformation.h          \
            gla/core/arraybuffer.h             \
            gla/core/vertexarrayobject.h       \
            gla/core/exceptions.h              \
            gla/core/texture.h                 \
            gla/core/types.h                   \
            gla/core/gputexturearray.h         \
            gla/core/timer.h                   \
            gla/core/framebufferobject.h       \
            gla/core/solids.h                  \
            gla/engine/skeleton.h              \
            gla/engine/transformsequence.h     \
            gla/engine/bone.h                  \
            gla/engine/modelloader.h           \
\
            gla/utils/window.h                 \
            gla/utils/app.h                    \
            gla/utils/utils.h                  \
\
            gla/engine/scene.h                 \
            gla/engine/cameracontrol.h          \
            gla/engine/node.h                  \
            gla/engine/engine.h                \
            gla/engine/material.h              \
            gla/stb/stb_image.h

SOURCES +=  main.cpp                                     \
            src/core/texture.cpp                         \
            src/core/solids.cpp                          \
            src/engine/transformsequence.cpp             \
            src/engine/skeleton.cpp                      \
            src/engine/bone.cpp                          \
            src/engine/modelloader.cpp                   \
            src/engine/scene.cpp                         \
            src/engine/node.cpp                          \
            src/engine/material.cpp                      \
            src/utils/window.cpp                         \
            examples/TextureArray.cpp                             \
            examples/HelloTriangle.cpp                            \
            examples/HelloTriangleVAO.cpp                         \
            examples/Textures.cpp                                 \
            examples/TransformSequence.cpp                        \
            examples/HelloTriangleEncapsulatedVAO.cpp             \
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
    shaders/TextureArray.f             \
    shaders/TextureArray.v             \
    shaders/PNU_TriPlanar.f            \
    shaders/PNU_TriPlanar.v            \
    shaders/HelloTriangle.f            \
    shaders/HelloTriangle.v            \
    shaders/Textures.f                 \
    shaders/Textures.v


