#-------------------------------------------------
#
# Project created by QtCreator 2017-07-19T01:40:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DnDtracker
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += -std=c++14 -Wno-c++1z-extensions
QMAKE_CXXFLAGS_RELEASE += -O3
QMAKE_CXXFLAGS_DEBUG += -fno-omit-frame-pointer -Wall -Werror -Wextra -ggdb -Wfloat-conversion -Wold-style-cast
QMAKE_CXXFLAGS_DEBUG += -fsanitize=undefined,address#,memory,safe-stack
QMAKE_LFLAGS_DEBUG += -fsanitize=undefined,address#,memory,safe-stack
QMAKE_CXXFLAGS_DEBUG += -Wno-unused-variable

SOURCES += \
        main.cpp \
        UI/mainwindow.cpp \
        UI/character_widget.cpp \
    Logic/character.cpp \
    Logic/skill_description.cpp \
    Logic/common.cpp \
    Logic/spellslots.cpp \
    Logic/class.cpp \
    Logic/dice.cpp \
    UI/character_selector_widget.cpp \
    UI/ui_utility.cpp \
    UI/character_skills_widget.cpp \
    Utility/overloaded_function.cpp \
    Utility/unreachable.cpp \
    Utility/assume.cpp

HEADERS += \
        UI/mainwindow.h \
        UI/character_widget.h \
    Logic/character.h \
    Logic/skill_description.h \
    Logic/common.h \
    Logic/spellslots.h \
    Logic/class.h \
    Logic/dice.h \
    UI/character_selector_widget.h \
    UI/ui_utility.h \
    UI/character_skills_widget.h \
    Utility/overloaded_function.h \
    Utility/unreachable.h \
    Utility/assume.h

FORMS += \
        UI/mainwindow.ui \
        UI/character_widget.ui \
    UI/character_selector.ui \
    UI/character_skills_widget.ui
