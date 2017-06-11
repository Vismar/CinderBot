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
    AI/ChatCommands/CommandList.cpp \
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
    AI/ChatCommands/CovenantCommands/CovenantCommandList.cpp \
    AI/ChatCommands/BaseChatCommand.cpp \
    AI/ChatCommands/InbuiltChatCommand.cpp \
    AI/ChatCommands/CustomCommands/CustomCovChatCommand.cpp \
    AI/ChatCommands/CustomCommands/CustomChatCommand.cpp \
    AI/ChatCommands/CustomCommands/CustomCovCommandList.cpp \
    AI/ChatCommands/CustomCommands/CustomCommanList.cpp \
    AI/ChatCommands/CovenantCommands/DescriptionCovenantCommand.cpp \
    AI/ChatCommands/CovenantCommands/InfoCovenantCommand.cpp \
    AI/ChatCommands/UserDataCommands/MessagesUserDataCommand.cpp \
    AI/ChatCommands/UserDataCommands/CurrencyUserDataCommand.cpp \
    AI/ChatCommands/UserDataCommands/CovenantUserDataCommand.cpp \
    AI/ChatCommands/UserDataCommands/FullUserDataCommand.cpp \
    AI/ChatCommands/UserDataCommands/UserDataCommandList.cpp

HEADERS  += MainWindow.hpp \
    Widgets/ChatWidget.hpp \
    Widgets/ChatMessageWidget.hpp \
    AI/BotAI.hpp \
    Chat/ChatMessage.hpp \
    Chat/TwitchClient.hpp \
    Utils/Config/ConfigurationManager.hpp \
    Utils/Config/ConfigurationParameters.hpp \
    Utils/UserData/UserData.hpp \
    AI/ChatCommands/CommandList.hpp \
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
    AI/ChatCommands/CovenantCommands/CovenantCommandList.hpp \
    AI/ChatCommands/BaseChatCommand.hpp \
    AI/ChatCommands/InbuiltChatCommand.hpp \
    AI/ChatCommands/CustomCommands/CustomCovChatCommand.hpp \
    AI/ChatCommands/CustomCommands/CustomChatCommand.hpp \
    AI/ChatCommands/CustomCommands/CustomCommandList.hpp \
    AI/ChatCommands/CustomCommands/CustomCovCommandList.hpp \
    AI/ChatCommands/CovenantCommands/DescriptionCovenantCommand.hpp \
    AI/ChatCommands/CovenantCommands/InfoCovenantCommand.hpp \
    AI/ChatCommands/UserDataCommands/MessagesUserDataCommand.hpp \
    AI/ChatCommands/UserDataCommands/CurrencyUserDataCommand.hpp \
    AI/ChatCommands/UserDataCommands/CovenantUserDataCommand.hpp \
    AI/ChatCommands/UserDataCommands/FullUserDataCommand.hpp \
    AI/ChatCommands/UserDataCommands/UserDataCommandList.hpp

RESOURCES += \
    Resources.qrc

win32: RC_ICONS = Resources/Icons/IconEmber.ico
