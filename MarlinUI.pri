INCLUDEPATH += $$PWD \
#               $$PWD/extensible_ui

DEFINES += FULL_CHINESE_FONT

HEADERS +=  \
    $$PWD/Levelingscreen.h \
    $$PWD/checkscreen.h \
    $$PWD/extuiextend.h \
    $$PWD/marlinui.h \
    $$PWD/motionunit.h \
    $$PWD/screen.h \
    $$PWD/screentask.h \
    $$PWD/splashscreen.h \
#    $$PWD/extensible_ui/ui_api.h \
    $$PWD/event.h \
    $$PWD/multilang.h \
    $$PWD/langenglish.h \
    $$PWD/homescreen.h \
    $$PWD/langchinese.h \
    $$PWD/resources.h \
    $$PWD/menuscreen.h \
    $$PWD/printingscreen.h \
    $$PWD/temperaturescreen.h \
    $$PWD/theme.h \
    $$PWD/themedark.h \
    $$PWD/themelight.h \
    $$PWD/motionscreen.h \
    $$PWD/filescreen.h \
    $$PWD/numberimputscreen.h \
    $$PWD/extrudescreen.h \
    $$PWD/messagescreen.h

SOURCES += \
#    $$PWD/extensible_ui/ui_api.cpp \
    $$PWD/checkscreen.cpp \
    $$PWD/extuiextend.cpp \
    $$PWD/levelingscreen.cpp \
    $$PWD/marlinui.cpp \
    $$PWD/homescreen.cpp \
    $$PWD/screen.cpp \
    $$PWD/screentask.cpp \
    $$PWD/splashscreen.cpp \
#    $$PWD/extensible_ui/lib/example.cpp \
    $$PWD/event.cpp \
    $$PWD/multilang.cpp \
    $$PWD/resources.c \
    $$PWD/menuscreen.cpp \
    $$PWD/printingscreen.cpp \
    $$PWD/temperaturescreen.cpp \
    $$PWD/theme.cpp \
    $$PWD/themedark.cpp \
    $$PWD/themelight.cpp \
    $$PWD/motionscreen.cpp \
    $$PWD/filescreen.cpp \
    $$PWD/numberimputscreen.cpp \
    $$PWD/extrudescreen.cpp \
    $$PWD/theme_c.c \
    $$PWD/messagescreen.cpp
