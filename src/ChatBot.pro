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
    AI/ChatCommands/ChatCommand.cpp \
    AI/ChatCommands/CommandList.cpp \
    AI/ChatCommands/CustomCommanLlist.cpp \
    AI/ChatCommands/UserDataCommand.cpp \
    AI/ChatCommands/UserDataCommandList.cpp \
    AI/ChatCommands/CovenantCommand.cpp \
    AI/ChatCommands/CovenantCommandList.cpp \
    AI/ChatCommands/QuoteCommands/BaseQuoteCommand.cpp \
    AI/ChatCommands/QuoteCommands/MainQuoteCommand.cpp \
    AI/ChatCommands/QuoteCommands/QuoteCommandList.cpp \
    AI/ChatCommands/QuoteCommands/AddQuoteCommand.cpp \
    AI/ChatCommands/QuoteCommands/DeleteQuoteCommand.cpp \
    AI/ChatCommands/QuoteCommands/EditQuoteCommand.cpp \
    Widgets/Statistics/StatisticsWidget.cpp \
    Widgets/Statistics/ViewerStatisticsWidget.cpp \
    Widgets/Statistics/ViewerCounterWidget.cpp \
    Utils/UserData/RealTimeUserData.cpp

HEADERS  += MainWindow.hpp \
    Widgets/ChatWidget.hpp \
    Widgets/ChatMessageWidget.hpp \
    AI/BotAI.hpp \
    Chat/ChatMessage.hpp \
    Chat/TwitchClient.hpp \
    Utils/Config/ConfigurationManager.hpp \
    Utils/Config/ConfigurationParameters.hpp \
    Utils/UserData/UserData.hpp \
    AI/ChatCommands/ChatCommand.hpp \
    AI/ChatCommands/CommandList.hpp \
    AI/ChatCommands/CustomCommandList.hpp \
    AI/ChatCommands/UserDataCommand.hpp \
    AI/ChatCommands/UserDataCommandList.hpp \
    AI/ChatCommands/CovenantCommand.hpp \
    AI/ChatCommands/CovenantCommandList.hpp \
    AI/ChatCommands/QuoteCommands/BaseQuoteCommand.hpp \
    AI/ChatCommands/QuoteCommands/MainQuoteCommand.hpp \
    AI/ChatCommands/QuoteCommands/QuoteCommandList.hpp \
    AI/ChatCommands/QuoteCommands/AddQuoteCommand.hpp \
    AI/ChatCommands/QuoteCommands/DeleteQuoteCommand.hpp \
    AI/ChatCommands/QuoteCommands/EditQuoteCommand.hpp \
    Widgets/Statistics/StatisticsWidget.hpp \
    Widgets/Statistics/ViewerStatisticsWidget.hpp \
    Widgets/Statistics/ViewerCounterWidget.hpp \
    Utils/UserData/RealTimeUserData.hpp

FORMS    +=

RESOURCES += \
    Resources.qrc

win32: RC_ICONS = Resources/Icons/IconEmber.ico
