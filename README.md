# CinderBot
## Description
Desktop twitch bot.
## Releases
###Version 0.4
First public version of CinderBot. Its include next features:
* Custom text commands

  In file data/data/baseFile.data you can write your own custom commands. One per line. For an example look at the file itself.

* Simple statisticks

  For every user bot create a line in data/data/userData.data (file will be created automaticly) which will store:
  
  * Message count
  * Currency count

    For now, you cannot use currency for anything. And it equals message count (it means, that 1 currency will be added per 1 message in a chat).In future versions it will be changed and upgraded. You can change name of a currency in data/config/config.cfg
    
  * Covenant name
  
    Not used now.
    
**P.S. To be mantioned, you can ignore some users in statisticks. Just add names in data/config/config.cfg.**

## How to make it work?
Build (only source code available now) with Qt5+, in data/config/config.cfg add your login and oathkey and room name, where bot should work. For more info look at config file itself.
