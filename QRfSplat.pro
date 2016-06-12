QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS += \
    mainwindow.h \
    textprinter.h

SOURCES += \
    mainwindow.cpp \
    main.cpp \
    textprinter.cpp

FORMS += \
    mainwindow.ui

TRANSLATIONS    = qrfsplat_es_MX.ts
