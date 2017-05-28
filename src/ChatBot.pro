#-------------------------------------------------
#
# Project created by QtCreator 2016-09-06T11:05:11
#
#-------------------------------------------------

QT       += core gui network charts sql

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
    AI/ChatCommands/QuoteCommands/BaseQuoteCommand.cpp \
    AI/ChatCommands/QuoteCommands/MainQuoteCommand.cpp \
    AI/ChatCommands/QuoteCommands/QuoteCommandList.cpp \
    AI/ChatCommands/QuoteCommands/AddQuoteCommand.cpp \
    AI/ChatCommands/QuoteCommands/DeleteQuoteCommand.cpp \
    AI/ChatCommands/QuoteCommands/EditQuoteCommand.cpp \
    Widgets/Statistics/StatisticsWidget.cpp \
    Widgets/Statistics/ViewerStatisticsWidget.cpp \
    Widgets/Statistics/ViewerCounterWidget.cpp \
    Utils/UserData/RealTimeUserData.cpp \
    Widgets/Statistics/ViewerGraphWidget.cpp \
    Widgets/Statistics/MessageStatisticsWidget.cpp \
    Widgets/Statistics/MessageGraphWidget.cpp \
    Utils/DatabaseManager.cpp \
    Widgets/Quotes/QuotesWidget.cpp \
    Widgets/Quotes/SingleQuoteWidget.cpp \
    Widgets/Quotes/CreateQuoteWidget.cpp \
    Widgets/Quotes/QuoteTabWidget.cpp \
    AI/ChatCommands/CovenantCommands/LeaveCovenantCommand.cpp \
    AI/ChatCommands/CovenantCommands/JoinCovenantCommand.cpp \
    AI/ChatCommands/CovenantCommands/ListCovenantCommand.cpp \
    AI/ChatCommands/CovenantCommands/CreateCovenantCommand.cpp \
    AI/ChatCommands/CovenantCommands/RenameCovenantCommand.cpp \
    AI/ChatCommands/CovenantCommands/DisbandCovenantCommand.cpp \
    AI/ChatCommands/CovenantCommands/CovenantCommandList.cpp

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
    AI/ChatCommands/QuoteCommands/BaseQuoteCommand.hpp \
    AI/ChatCommands/QuoteCommands/MainQuoteCommand.hpp \
    AI/ChatCommands/QuoteCommands/QuoteCommandList.hpp \
    AI/ChatCommands/QuoteCommands/AddQuoteCommand.hpp \
    AI/ChatCommands/QuoteCommands/DeleteQuoteCommand.hpp \
    AI/ChatCommands/QuoteCommands/EditQuoteCommand.hpp \
    Widgets/Statistics/StatisticsWidget.hpp \
    Widgets/Statistics/ViewerStatisticsWidget.hpp \
    Widgets/Statistics/ViewerCounterWidget.hpp \
    Utils/UserData/RealTimeUserData.hpp \
    Widgets/Statistics/ViewerGraphWidget.hpp \
    Widgets/Statistics/MessageStatisticsWidget.hpp \
    Widgets/Statistics/MessageGraphWidget.hpp \
    Utils/DatabaseManager.hpp \
    Widgets/Quotes/QuotesWidget.hpp \
    Widgets/Quotes/SingleQuoteWidget.hpp \
    Widgets/Quotes/CreateQuoteWidget.hpp \
    Widgets/Quotes/QuoteTabWidget.hpp \
    AI/ChatCommands/CovenantCommands/LeaveCovenantCommand.hpp \
    AI/ChatCommands/CovenantCommands/JoinCovenantCommand.hpp \
    AI/ChatCommands/CovenantCommands/ListCovenantCommand.hpp \
    AI/ChatCommands/CovenantCommands/CreateCovenantCommand.hpp \
    AI/ChatCommands/CovenantCommands/RenameCovenantCommand.hpp \
    AI/ChatCommands/CovenantCommands/DisbandCovenantCommand.hpp \
    AI/ChatCommands/CovenantCommands/CovenantCommandList.hpp

RESOURCES += \
    Resources.qrc

win32: RC_ICONS = Resources/Icons/IconEmber.ico
