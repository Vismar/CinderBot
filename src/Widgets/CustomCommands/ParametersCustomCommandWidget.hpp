/*************************************************************************
***************  CinderBot - standalone bot for Twitch.tv ****************
******** Copyright (C) 2017  Ilya Lobanov (exanimoteam@gmail.com) ********
********         Check full copyright header in main.cpp          ********
**************************************************************************/
#pragma once
#include <QFrame>
#include <QLineEdit>
#include <QTimeEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QString>
#include <QTime>

namespace Ui
{

/*!
 * class ParametersCustomCommandWidget
 * Widget which handles storing and changing parameters for custom commands.
 */
class ParametersCustomCommandWidget : public QFrame
{
    Q_OBJECT
public:
    /*!
     * Constructor
     */
    explicit ParametersCustomCommandWidget(QWidget *parent = 0);
    /*!
     * Destructor
     */
    ~ParametersCustomCommandWidget();

    /*!
     * Set ability for command name field to be changeable
     * \param state - new state
     */
    void SetCommandNameChangeable(bool state);
    /*!
     * Get current text stored in "command name" field
     * \return stored text
     */
    QString GetCommandName() const;
    /*!
     * Set specified text to "command name" field
     * \param cmdName - new text for field
     */
    void SetCommandName(const QString& cmdName);
    /*!
     * Get cooldown which was setted for current command
     * \return stored time
     */
    QTime GetCooldown() const;
    /*!
     * Set cooldown for current command
     * \param cooldown - time value which will be setted
     */
    void SetCooldown(const QTime& cooldown);
    /*!
     * Get value which represents if this command can be executed only by moderators
     * \return true if command will be executed only by moderators
     */
    bool GetModOnly() const;
    /*!
     * Set value which represents if this command can be executed only by moderators
     * \param modOnly - new value for moderatorOnly flag
     */
    void SetModOnly(bool modOnly);
    /*!
     * Get price which will be taken from users when they will use this command
     * \return value which was setted
     */
    int GetPrice() const;
    /*!
     * Set price which will be taken from users when they will use this command
     * \param price - new price value
     */
    void SetPrice(int price);
    /*!
     * Get covenant of which members will be able to execute command
     * \return covenant name
     */
    QString GetCovenant() const;
    /*!
     * Set covenant of which members will be able to execute command
     * \param covenant - new covenant
     */
    void SetCovenant(const QString& covenant);

signals:
    void OnCommandNameFieldChanged(const QString &text);

private slots:
    void _OnCmdNameChanged(const QString &text);

private:
    /*!
     * Initialize widgets
     */
    void _Initialize();
    /*!
     * Intitialize command name widgets
     */
    void _InitializeCmdName();
    /*!
     * Initialize cooldown widgets
     */
    void _InitializeCooldown();
    /*!
     * Initialize moderator only widgets
     */
    void _InitializeModOnly();
    /*!
     * Initialize price widgets
     */
    void _InitializePrice();
    /*!
     * Inititalize covenant widgets
     */
    void _InitializeCovenant();

    /*! Main layout of whole widhet */
    QGridLayout *_mainLayout;
    /*! Label for command name */
    QLabel *_commandNameLabel;
    /*! Command name, editable text widget */
    QLineEdit *_commandName;
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
};

}
