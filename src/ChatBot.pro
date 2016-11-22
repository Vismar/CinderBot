#-------------------------------------------------
#
# Project created by QtCreator 2016-09-06T11:05:11
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatBot
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        MainWindow.cpp \
    Widgets/ChatWidget.cpp \
    Widgets/ChatMessageWidget.cpp \
    AI/BotAI.cpp \
    AI/ChatCommands/BaseFileChatCommand.cpp \
    Chat/ChatMessage.cpp \
    Chat/TwitchClient.cpp \
    Utils/Config/ConfigurationManager.cpp \
    Utils/UserData/UserData.cpp \
    AI/ChatCommands/UserDataChatCommand.cpp \
    AI/ChatCommands/QuoteChatCommand.cpp

HEADERS  += MainWindow.hpp \
    Widgets/ChatWidget.hpp \
    Widgets/ChatMessageWidget.hpp \
    AI/BotAI.hpp \
    AI/ChatCommands/BaseChatCommand.hpp \
    AI/ChatCommands/BaseFileChatCommand.hpp \
    Chat/ChatMessage.hpp \
    Chat/TwitchClient.hpp \
    Utils/Config/ConfigurationManager.hpp \
    Utils/Config/ConfigurationParameters.hpp \
    Utils/UserData/UserData.hpp \
    AI/ChatCommands/UserDataChatCommand.hpp \
    AI/ChatCommands/QuoteChatCommand.hpp

FORMS    +=
