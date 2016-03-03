QT += widgets

CONFIG += console
CONFIG -= app_bundle

debug: DBG = -dgb

DESTDIR = build$$DBG-$$[QMAKE_SPEC]/bin
OBJECTS_DIR = build$$DBG-$$[QMAKE_SPEC]/$$TARGET
MOC_DIR = build$$DBG-$$[QMAKE_SPEC]/$$TARGET
UI_DIR = build$$DBG-$$[QMAKE_SPEC]/$$TARGET
RCC_DIR = build$$DBG-$$[QMAKE_SPEC]/$$TARGET


SOURCES += \
	imgui_impl_qt.cpp main.cpp \
	../../imgui.cpp ../../imgui_demo.cpp ../../imgui_draw.cpp

HEADERS += imgui_impl_qt.h

INCLUDEPATH += ../../