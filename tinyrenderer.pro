CONFIG -= qt
CONFIG += c++11

INCLUDEPATH += D:\\projects\SDL2-2.0.3\include
LIBS += -L"D:\\projects\SDL2-2.0.3\lib\x86" \
-lSDL2main\
-lSDL2\

HEADERS += \
    geometry.h \
    model.h \
    our_gl.h \
    tgaimage.h \
    sdlwindow.h \
    shader.h \
    frametile.h \
    threadworker.h \
    thread/threadworker.h \
    thread/threadpool.h

SOURCES += \
    geometry.cpp \
    main.cpp \
    model.cpp \
    our_gl.cpp \
    tgaimage.cpp \
    sdlwindow.cpp \
    shader.cpp \
    frametile.cpp
