/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QTimeEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QString>
#include <QTime>
#include "Widgets/Common/EnhLineEdit.hpp"

/*!
* \brief All Ui widgets and windows.
*/
namespace Ui
{
/*!
 * \brief All wigets related to custom commands.
 */
namespace CustomCommand
{

/*!
 * \brief Widget which handles storing and changing parameters for custom commands.
 * 
 * Contains all needed params for custom command and displays it via widgets. 
 * Everything can setted and goten throught setters and getters.
 */
class ParametersCustomCommandWidget : public QFrame
{
    Q_OBJECT
public:
    explicit ParametersCustomCommandWidget(QWidget *parent = nullptr);
    ~ParametersCustomCommandWidget();

    /*!
     * \brief Sets ability for command name field to be changeable.
     * \param state - new state. If true - _commandName can be edited. If false - cannot.
     * 
     * Chanes state of _commandName depends on specified parameter.
     */
    void SetCommandNameChangeable(bool state) const;
    /*!
     * \brief Gets command name.
     * \return stored text in _commandName.
     * 
     * Returns current text stored in _commandName;
     */
    QString GetCommandName() const;
    /*!
     * \brief Sets command name.
     * \param cmdName - new text for field.
     * 
     * Sets specified text to _commandName;
     */
    void SetCommandName(const QString& cmdName) const;
    /*!
     * \brief Gets cooldown.
     * \return stored time ib _cooldown.
     * 
     * Returns cooldown which was setted for current command in _cooldown.
     */
    QTime GetCooldown() const;
    /*!
     * \brief Sets cooldown.
     * \param cooldown - time value which will be setted.
     * 
     * Set specified cooldown value to _cooldown.
     */
    void SetCooldown(const QTime& cooldown) const;
    /*!
     * \brief Gets ModOnly flag value.
     * \return true if command will be executed only by moderators.
     * 
     * returns value which represents if this command can be executed only by moderators.
     */
    bool GetModOnly() const;
    /*!
     * \brief Sets ModOnly flag value.
     * \param modOnly - new value for moderatorOnly flag.
     * 
     * Sets value which represents if this command can be executed only by moderators.
     */
    void SetModOnly(bool modOnly) const;
    /*!
     * \brief Gets price.
     * \return value which was setted in _price.
     * 
     * Return price which will be taken from users when they will use this command.
     */
    int GetPrice() const;
    /*!
     * \brief Sets price.
     * \param price - new price value.
     * 
     * Sets price which will be taken from users when they will use this command.
     */
    void SetPrice(int price) const;
    /*!
     * \brief Gets covenant name.
     * \return covenant name.
     * 
     * Returns covenant of which members will be able to execute command. "None" means everyone can execute command.
     */
    QString GetCovenant() const;
    /*!
     * \brief Sets covenant value.
     * \param covenant - new covenant.
     * 
     * Sets covenant of which members will be able to execute command.
     */
    void SetCovenant(const QString& covenant) const;
    /*!
     * \brief Gets work in whisper flag.
     * 
     * Returns work in whisper flag value that represents if this command can be executed via whisper.
     */
    bool GetWorkInWhisper() const;
    /*!
     * \brief Sets work in whisper flag.
     * \param workInWhisper - new flag value.
     * 
     * Sets work in whisper flag value that represents if this command can be executed via whisper.
     */
    void SetWorkInWhisper(bool workInWhisper) const;
    /*!
     * \brief Gets work in chat flag.
     * 
     * Returns work in chat flag value that represents if this command can be executed via chat.
     */
    bool GetWorkInChat() const;
    /*!
     * \brief Sets work in chat flag.
     * \param workInChat - new flag value.
     * 
     * Sets work in chat flag value that represents if this command can be executed via chat.
     */
    void SetWorkInChat(bool workInChat) const;

signals:
    /*!
     * \brief Signal to notify if _commandName was changed.
     * 
     * Notifies listeners about changes in _commandName and make everything there lowercase.
     */
    void OnCommandNameFieldChanged(const QString &text);

private slots:
    /*!
     * \brief Simple slot tp emit OnCommandNameFieldChanged().
     * \param text - new value of _commandName.
     * 
     * Only emits signal OnCommandNameFieldChanged().
     */
    void _OnCmdNameChanged(const QString &text);

private:
    /*!
     * \brief Initializes widgets.
     * 
     * Calls all initialize function to get ready all widgets.
     */
    void _Initialize();
    /*!
     * \brief Intitializes command name widgets.
     * 
     * Creates label and text edit field to display/edit command name. Adds them to layout in 0 line.
     * Connects textChanged signal from _commandName with _OnCmdNameChanged slot.
     */
    void _InitializeCmdName();
    /*!
     * \brief Initializes cooldown widgets.
     * 
     * Creates label and time edit widget to display/edit cooldown. Adds them to layout in 1 line.
     */
    void _InitializeCooldown();
    /*!
     * \brief Initialize moderator only widgets.
     * 
     * Creates label and check box widget to display/edit mod only flag. Adds them to layout in 2 line.
     */
    void _InitializeModOnly();
    /*!
     * \brief Initialize price widgets.
     * 
     * Creates label and spin box widget to display/edit price. Adds them to layout in 3 line.
     */
    void _InitializePrice();
    /*!
     * \brief Inititalize covenant widgets.
     * 
     * Creates label and combo box widget to display/edit covenant. Adds them to layout in 4 line.
     */
    void _InitializeCovenant();
    /*!
     * \brief Initializes work in whisper widgets.
     * 
     * Creates label and check box widget to display/edit work in whisper. Adds them to layout in 5 line.
     */
    void _InitializeWorkInWhisper();
    /*!
     * \brief Initializes work in chat widgets.
     * 
     * Creates label and check box widget to display/edit work in chat. Adds them to layout in 6 line.
     */
    void _InitializeWorkInChat();

    /*! Main layout of the whole widget */
    QGridLayout *_mainLayout;
    
    /*! Label for command name */
    QLabel *_commandNameLabel;
    /*! Command name, editable text widget */
    Common::EnhLineEdit *_commandName;
    
    /*! Label for cooldown */
    QLabel *_cooldownLabel;
    /*! Cooldown editable time */
    QTimeEdit *_cooldown;
    
    /*! Label for moderator only */
    QLabel *_modOnlyLabel;
    /*! Moderator only check box */
    QCheckBox *_moderatorOnly;
    
    /*! Label for price */
    QLabel *_priceLabel;
    /*! covenant spin widget */
    QSpinBox *_price;
    
    /*! Label for covenant */
    QLabel *_covenantLabel;
    /*! Covenant combox box widget */
    QComboBox *_covenant;
    
    /*! Label for Work in whisper */
    QLabel *_wokrInWhisperLabel;
    /*! Work in whisper check box */
    QCheckBox *_workInWhisper;
    
    /*! Label for Work in chat */
    QLabel *_wokrInChatLabel;
    /*! Work in chat check box */
    QCheckBox *_workInChat;
};

}
}
