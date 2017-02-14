TEMPLATE = app
CONFIG += c++14
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += strict_c++

QMAKE_CXXFLAGS += -pedantic

LIBS += -lglfw -lGL -ldl -lfreetype

INCLUDEPATH +=

SOURCES += source/main.cpp \
    source/app.cpp \
    source/glad/glad.c \
    source/opengl/buffers.cpp \
    source/opengl/texture.cpp \
    source/opengl/shader.cpp

HEADERS += \
    source/app.hpp \
    source/glad/glad.h \
    source/opengl/stb_image.h \
    source/opengl/buffers.hpp \
    source/opengl/texture.hpp \
    source/opengl/base.hpp \
    source/opengl/shader.hpp
