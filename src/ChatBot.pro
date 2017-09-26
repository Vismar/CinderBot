#-------------------------------------------------
#
# Project created by QtCreator 2016-09-06T11:05:11
#
#-------------------------------------------------

QT += core gui network charts sql widgets webenginewidgets

TARGET = ChatBot
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        MainWindow.cpp \
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
    Widgets/Quotes/QuotesWidget.cpp \
    Widgets/Quotes/SingleQuoteWidget.cpp \
    Widgets/Quotes/CreateQuoteWidget.cpp \
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
    AI/ChatCommands/CovenantCommands/DescriptionCovenantCommand.cpp \
    AI/ChatCommands/CovenantCommands/InfoCovenantCommand.cpp \
    AI/ChatCommands/UserDataCommands/MessagesUserDataCommand.cpp \
    AI/ChatCommands/UserDataCommands/CurrencyUserDataCommand.cpp \
    AI/ChatCommands/UserDataCommands/CovenantUserDataCommand.cpp \
    AI/ChatCommands/UserDataCommands/FullUserDataCommand.cpp \
    AI/ChatCommands/UserDataCommands/UserDataCommandList.cpp \
    AI/ChatCommands/UserDataCommands/CheckUserDataCommand.cpp \
    Widgets/Chat/ChatMessageWidget.cpp \
    Widgets/Chat/ChatWidget.cpp \
    Widgets/Common/PageListWidget.cpp \
    Widgets/CustomCommands/ParametersCustomCommandWidget.cpp \
    Widgets/CustomCommands/CreateCustomCommandWindow.cpp \
    Widgets/CustomCommands/EditCustomCommandWindow.cpp \
    Widgets/Common/EditSaveWidget.cpp \
    Widgets/CustomCommands/EditSaveAnswerCustomCommandWidget.cpp \
    Widgets/CustomCommands/AnswersCustomCommandWidget.cpp \
    Widgets/CustomCommands/EntryCustomCommandWidget.cpp \
    Widgets/CustomCommands/ListCustomCommandWidget.cpp \
    Widgets/CustomCommands/CustomCommandWindow.cpp \
    Chat/ChatAnswer.cpp \
    Widgets/Quotes/QuoteWindow.cpp \
    Widgets/Chat/ChatWindow.cpp \
    Widgets/Statistics/StatisticsWindow.cpp \
    Widgets/Login/ConnectWidget.cpp \
    Widgets/Login/LoginWindow.cpp \
    Widgets/Common/EnhLineEdit.cpp \
    Widgets/Common/EnhTextEdit.cpp \
    Utils/Logger.cpp \
    AI/TimerCommands/BaseTimerCommand.cpp \
    AI/TimerCommands/BaseTimerCommandList.cpp \
    AI/TimerCommands/UserData/UserDataTimerCommandList.cpp \
    AI/TimerCommands/UserData/CurrencyToOnlineTimerCommand.cpp \
    AI/TimerCommands/UserData/TimeInChatTimerCommand.cpp \
    AI/ChatCommands/UserDataCommands/TimeInChatCommand.cpp \
    AI/ChatCommands/CovenantCommands/MembersCovenantCommand.cpp \
    AI/ChatCommands/UserDataCommands/UserDataBaseCommand.cpp \
    AI/ChatCommands/CovenantCommands/CovenantBaseCommand.cpp \
    Utils/Database/DatabaseManager.cpp \
    Utils/Database/CustomCommandDBHelper.cpp \
    AI/ChatCommands/CustomCommands/CustomCommandList.cpp \
    AI/ChatCommands/CovenantCommands/AdminCovCmdCommand.cpp \
    AI/ChatCommands/CovenantCommands/AdminCovCmdEditCommand.cpp \
    AI/ChatCommands/CovenantCommands/AdminCovCmdAnswerCommand.cpp \
    AI/ChatCommands/CovenantCommands/AdminCovCmdDeleteCommand.cpp \
    Utils/Database/QuoteDBHelper.cpp \
    Twitch/KrakenClient.cpp \
    Twitch/KrakenResponse.cpp

HEADERS  += MainWindow.hpp \
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
    Widgets/Quotes/QuotesWidget.hpp \
    Widgets/Quotes/SingleQuoteWidget.hpp \
    Widgets/Quotes/CreateQuoteWidget.hpp \
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
    AI/ChatCommands/UserDataCommands/UserDataCommandList.hpp \
    AI/ChatCommands/UserDataCommands/CheckUserDataCommand.hpp \
    Widgets/Chat/ChatMessageWidget.hpp \
    Widgets/Chat/ChatWidget.hpp \
    Widgets/Common/PageListWidget.hpp \
    Widgets/CustomCommands/ParametersCustomCommandWidget.hpp \
    Widgets/CustomCommands/CreateCustomCommandWindow.hpp \
    Widgets/CustomCommands/EditCustomCommandWindow.hpp \
    Widgets/Common/EditSaveWidget.hpp \
    Widgets/CustomCommands/EditSaveAnswerCustomCommandWidget.hpp \
    Widgets/CustomCommands/AnswersCustomCommandWidget.hpp \
    Widgets/CustomCommands/EntryCustomCommandWidget.hpp \
    Widgets/CustomCommands/ListCustomCommandWidget.hpp \
    Widgets/CustomCommands/CustomCommandWindow.hpp \
    Chat/ChatAnswer.hpp \
    Widgets/Quotes/QuoteWindow.hpp \
    Widgets/Chat/ChatWindow.hpp \
    Widgets/Statistics/StatisticsWindow.hpp \
    Widgets/Login/ConnectWidget.hpp \
    Widgets/Login/LoginWindow.hpp \
    Widgets/Common/EnhLineEdit.hpp \
    Widgets/Common/EnhTextEdit.hpp \
    Utils/Logger.hpp \
    AI/TimerCommands/BaseTimerCommand.hpp \
    AI/TimerCommands/BaseTimerCommandList.hpp \
    AI/TimerCommands/UserData/UserDataTimerCommandList.hpp \
    AI/TimerCommands/UserData/CurrencyToOnlineTimerCommand.hpp \
    AI/TimerCommands/UserData/TimeInChatTimerCommand.hpp \
    AI/ChatCommands/UserDataCommands/TimeInChatCommand.hpp \
    AI/ChatCommands/CovenantCommands/MembersCovenantCommand.hpp \
    AI/ChatCommands/UserDataCommands/UserDataBaseCommand.hpp \
    AI/ChatCommands/CovenantCommands/CovenantBaseCommand.hpp \
    Utils/Database/DatabaseManager.hpp \
    Utils/Database/CustomCommandDBHelper.hpp \
    AI/ChatCommands/CovenantCommands/AdminCovCmdCommand.hpp \
    AI/ChatCommands/CovenantCommands/AdminCovCmdEditCommand.hpp \
    AI/ChatCommands/CovenantCommands/AdminCovCmdAnswerCommand.hpp \
    AI/ChatCommands/CovenantCommands/AdminCovCmdDeleteCommand.hpp \
    Utils/Database/QuoteDBHelper.hpp \
    Twitch/KrakenClient.hpp \
    Twitch/KrakenResponse.hpp

RESOURCES += \
    Resources.qrc

win32: RC_ICONS = Resources/Icons/IconEmber.ico
