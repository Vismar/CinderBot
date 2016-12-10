#include "QuoteCommandList.hpp"
#include "MainQuoteCommand.hpp"
#include "AddQuoteCommand.hpp"
#include "DeleteQuoteCommand.hpp"
#include "EditQuoteCommand.hpp"
#include <QFile>

using namespace Command;

///////////////////////////////////////////////////////////////////////////

QuoteCommandList::QuoteCommandList()
{
    _commands.push_back(new MainQuoteCommand());
    _commands.push_back(new AddQuoteCommand());
    _commands.push_back(new DeleteQuoteCommand());
    _commands.push_back(new EditQuoteCommand());
    for (int i = 0; i < _commands.size(); ++i)
    {
        BaseQuoteCommand* command = static_cast<BaseQuoteCommand*>(_commands[i]);
        if (command)
        {
            command->SetQuoteRef(&_quotes);
        }
    }
    _Initialize();
}

///////////////////////////////////////////////////////////////////////////

QuoteCommandList::~QuoteCommandList()
{
    QFile commandsFile("./data/data/Quotes.xml");
    if (commandsFile.open(QIODevice::WriteOnly))
    {
        _xmlWriter.setDevice(&commandsFile);
        _xmlWriter.setAutoFormatting(true);
        _xmlWriter.writeStartDocument();
        _xmlWriter.writeStartElement("Quotes");
        _SaveQuotes();
        _xmlWriter.writeEndElement();
        _xmlWriter.writeEndDocument();
    }
    commandsFile.close();
}

///////////////////////////////////////////////////////////////////////////

void QuoteCommandList::_SaveQuotes()
{
    for (int i = 0; i < _quotes.size(); ++i)
    {
        _xmlWriter.writeTextElement("Quote", _quotes.at(i));
    }
}

///////////////////////////////////////////////////////////////////////////

void QuoteCommandList::_Initialize()
{
    QFile commandsFile("./data/data/Quotes.xml");
    // Try to open file
    if (commandsFile.open(QIODevice::ReadOnly))
    {
        _xmlReader.setDevice(&commandsFile);

        // Read xml file
        while (!_xmlReader.atEnd())
        {
            _xmlReader.readNext();
            if (_xmlReader.isStartElement())
            {
                // If it is start section of quote, read it
                if (_xmlReader.name() == "Quote")
                {
                    QString quote = _xmlReader.readElementText();
                    _quotes.push_back(quote);
                }
            }
        }
    }
    commandsFile.close();
}

///////////////////////////////////////////////////////////////////////////
