TEMPLATE = app
CONFIG += c++14
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += strict_c++

QMAKE_CXXFLAGS += -pedantic

DEFINES += GLM_FORCE_NO_CTOR_INIT

LIBS += -lglfw -lGL -ldl -lfreetype -lassimp

INCLUDEPATH += /usr/include/freetype2

SOURCES += source/main.cpp \
    source/app.cpp \
    source/glad/glad.c \
    source/opengl/buffers.cpp \
    source/opengl/texture.cpp \
    source/opengl/shader.cpp \
    source/rendering/renderer_2d.cpp \
    source/rendering/font_loader.cpp \
    source/rendering/postprocessor.cpp \
    source/rendering/text.cpp \
    source/opengl/base.cpp \
    source/rendering/mesh.cpp \
    source/rendering/model_3d.cpp \
    source/rendering/renderer_3d.cpp \
    source/rendering/camera_3d.cpp

HEADERS += source/app.hpp \
    source/glad/glad.h \
    source/opengl/stb_image.h \
    source/opengl/buffers.hpp \
    source/opengl/texture.hpp \
    source/opengl/base.hpp \
    source/opengl/shader.hpp \
    source/rendering/renderer_2d.hpp \
    source/rendering/sprite.hpp \
    source/rendering/text.hpp \
    source/rendering/font_loader.hpp \
    source/rendering/postprocessor.hpp \
    source/rendering/render_obj_base.hpp \
    source/rendering/mesh.hpp \
    source/rendering/model_3d.hpp \
    source/rendering/renderer_3d.hpp \
    source/rendering/camera_3d.hpp \
    source/key_input.hpp

DISTFILES += \
    shaders/shader_2d.frag \
    shaders/shader_2d.vert \
    shaders/shader_fb.vert \
    shaders/shader_fb_blur.frag \
    shaders/shader_fb_blend.frag \
    shaders/shader_3d.frag \
    shaders/shader_3d.vert
