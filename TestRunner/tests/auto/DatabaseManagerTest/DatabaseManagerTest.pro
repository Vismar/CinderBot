QT += testlib
QT += sql
CONFIG += qt warn_on depend_includepath testcase
CONFIG += c++11

TEMPLATE = app

SOURCES += tst_DatabaseManagerTest.cpp \
    ../../../../src/Utils/DatabaseManager.cpp

HEADERS += \
    ../../../../src/Utils/DatabaseManager.hpp
