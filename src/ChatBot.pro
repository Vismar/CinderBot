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
    Chat/ChatMessage.cpp \
    Chat/TwitchClient.cpp \
    Utils/Config/ConfigurationManager.cpp \
    Utils/UserData/UserData.cpp \
    AI/ChatCommands/QuoteChatCommand.cpp \
    AI/ChatCommands/CovenantChatCommand.cpp \
    AI/ChatCommands/ChatCommand.cpp \
    AI/ChatCommands/CommandList.cpp \
    AI/ChatCommands/CustomCommanLlist.cpp \
    AI/ChatCommands/UserDataCommand.cpp \
    AI/ChatCommands/UserDataCommandList.cpp

HEADERS  += MainWindow.hpp \
    Widgets/ChatWidget.hpp \
    Widgets/ChatMessageWidget.hpp \
    AI/BotAI.hpp \
    AI/ChatCommands/BaseChatCommand.hpp \
    Chat/ChatMessage.hpp \
    Chat/TwitchClient.hpp \
    Utils/Config/ConfigurationManager.hpp \
    Utils/Config/ConfigurationParameters.hpp \
    Utils/UserData/UserData.hpp \
    AI/ChatCommands/QuoteChatCommand.hpp \
    AI/ChatCommands/CovenantChatCommand.hpp \
    AI/ChatCommands/ChatCommand.hpp \
    AI/ChatCommands/CommandList.hpp \
    AI/ChatCommands/CustomCommandList.hpp \
    AI/ChatCommands/UserDataCommand.hpp \
    AI/ChatCommands/UserDataCommandList.hpp

FORMS    +=
