#include <QtTest/QtTest>
#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include "../../../../../CinderBot/src/Utils/DatabaseManager.hpp"

class DatabaseManagerTest : public QObject
{
    Q_OBJECT

public:
    DatabaseManagerTest();
    ~DatabaseManagerTest();

private slots:
    void CreateDatabase();
    void CreateTable();
    void CreateIndex();
    void Insert();
    void Select();
    void Update();
    void Delete();
};

///////////////////////////////////////////////////////////////////////////

DatabaseManagerTest::DatabaseManagerTest() { }

///////////////////////////////////////////////////////////////////////////

DatabaseManagerTest::~DatabaseManagerTest() { }

///////////////////////////////////////////////////////////////////////////

void DatabaseManagerTest::CreateDatabase()
{
    // Check if directory exist
    QDir dir("data");
    if (!dir.exists())
    {
        QVERIFY(dir.mkpath("."));
    }
    // Check if file exist
    QFileInfo checkDB("./data/CinderBotDatabase.sqlite");
    if (checkDB.exists() && checkDB.isFile())
    {
        // Remove database file if it is exist
        QVERIFY(QFile::remove("./data/CinderBotDatabase.sqlite"));
    }
    // Initialize database
    QCOMPARE(DatabaseManager::Instance().Initialize(), QString("OK"));
}

///////////////////////////////////////////////////////////////////////////

void DatabaseManagerTest::CreateTable()
{
    /* Tests */
    // Try to create first table
    QVERIFY(DB_CREATE_TABLE("FirstTable", "Id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                          "Text TEXT NOT NULL"));
    // Try to create the same table, manager should return true, because this command will be ignored
    QVERIFY(DB_CREATE_TABLE("FirstTable", "Id INTEGER PRIMARY KEY UNIQUE AUTOINCREMENT,"
                                          "Text TEXT NOT NULL"));
    // Update first table with new column, new column should be added and true returned
    QVERIFY(DB_CREATE_TABLE("FirstTable", "Id INTEGER PRIMARY KEY UNIQUE AUTOINCREMENT,"
                                          "Text TEXT NOT NULL,"
                                          "Number INTEGER"));
}

///////////////////////////////////////////////////////////////////////////

void DatabaseManagerTest::CreateIndex()
{
    // Try to create index on one column
    QVERIFY(DB_CREATE_INDEX("FirstTable", "Index_Name", "Text"));
    // Try to do the same, but this command will be ignored and return true
    QVERIFY(DB_CREATE_INDEX("FirstTable", "Index_Name", "Text"));
}

///////////////////////////////////////////////////////////////////////////

void DatabaseManagerTest::Insert()
{
    // Try to add the standard record
    QVERIFY(DB_INSERT("FirstTable", "NULL, 'Some Text Goes Here', 123"));
    // Try to add record with wrong syntax in text field
    QVERIFY(!DB_INSERT("FirstTable", "NULL, Some WRONG Text Goes Here', 123"));
}

///////////////////////////////////////////////////////////////////////////

void DatabaseManagerTest::Select()
{
    // Check if query was done
    DB_QUERY_PTR query = DB_SELECT("FirstTable", "*");
    QVERIFY(query != nullptr);
    // Check first record
    QVERIFY(query->next());
    QCOMPARE(query->value("Text").toString(), QString("Some Text Goes Here"));
    QCOMPARE(query->value("Number").toInt(), 123);
    // Check that second record was not added and cannot be selected
    QVERIFY(!query->next());
}

///////////////////////////////////////////////////////////////////////////

void DatabaseManagerTest::Update()
{
    // Check if update was done
    QVERIFY(DB_UPDATE("FirstTable", "Number=321"));
    // Check if query was done
    DB_QUERY_PTR query = DB_SELECT("FirstTable", "Number");
    QVERIFY(query != nullptr);
    // Check first record
    QVERIFY(query->next());
    QCOMPARE(query->value("Number").toInt(), 321);
    // Check that second record was not added and cannot be selected
    QVERIFY(!query->next());
}

///////////////////////////////////////////////////////////////////////////

void DatabaseManagerTest::Delete()
{
    // Check if deletion was done
    QVERIFY(DB_DELETE("FirstTable"));
    // Check if query was done
    DB_QUERY_PTR query = DB_SELECT("FirstTable", "Number");
    QVERIFY(query != nullptr);
    // Check that even first record was deleted
    QVERIFY(!query->next());
}

///////////////////////////////////////////////////////////////////////////

QTEST_MAIN(DatabaseManagerTest)

#include "tst_DatabaseManagerTest.moc"
