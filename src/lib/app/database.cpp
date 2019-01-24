#include "common-defs.h"
#include "database.h"

#include <QDebug>
#include <QSqlError>

DataBase::DataBase(QObject *parent)
    : QObject (parent)
{
    QSqlDatabase db = QSqlDatabase::addDatabase(QSL("QSQLITE"));
    db.setDatabaseName(QSL("data"));
    if (!db.open()) {
        qWarning() << db.lastError();
        return;
    }
    createTables();
}

void DataBase::createTables()
{
    QSqlQuery query;
    query.exec(QSL("CREATE TABLE IF NOT EXISTS history (lastVisited DATETIME, title TEXT, url TEXT PRIMARY KEY)"));
    query.exec(QSL("CREATE TABLE IF NOT EXISTS bookmarks (title TEXT, url TEXT PRIMARY KEY, folder TEXT)"));
}

void DataBase::addHistory(const HistoryItem &item)
{
    QSqlQuery query(QSL("INSERT OR REPLACE INTO history VALUES (?, ?, ?)"));
    query.addBindValue(item.dateTime.toTime_t());
    query.addBindValue(item.title);
    query.addBindValue(item.url);
    query.exec();
}

void DataBase::removeHistory(const QString &address)
{
    QSqlQuery query(QSL("DELETE FROM history WHERE url = ?"));
    query.addBindValue(address);
    query.exec();
}

QList<HistoryItem> DataBase::history() const
{
    QList<HistoryItem> entries;
    QSqlQuery query(QSL("SELECT * FROM history"));
    while (query.next()) {
        HistoryItem item;
        item.dateTime = QDateTime::fromTime_t(query.value(0).toUInt());
        item.title = query.value(1).toString();
        item.url = query.value(2).toString();
        entries.append(item);
    }

    return entries;
}

void DataBase::addBookmark(const BookmarkItem &item)
{
    QSqlQuery query(QSL("INSERT INTO bookmarks VALUES (?, ?, ?)"));
    query.addBindValue(item.title);
    query.addBindValue(item.address);
    query.addBindValue(item.folder);
    query.exec();
}

void DataBase::removeBookmark(const QString &address)
{
    QSqlQuery query(QSL("DELETE FROM bookmarks WHERE url = ?"));
    query.addBindValue(address);
    query.exec();
}

void DataBase::updateBookmark(const BookmarkItem &item)
{
    QSqlQuery query(QSL("UPDATE bookmarks SET title = ?, folder = ? WHERE url = ?"));
    query.addBindValue(item.title);
    query.addBindValue(item.folder);
    query.addBindValue(item.address);
    query.exec();
}

QStringList DataBase::bookmarkFolders() const
{
    QStringList folders;
    QSqlQuery query(QSL("SELECT DISTINCT folder FROM bookmarks"));
    while (query.next()) {
        folders << query.value(0).toString();
    }
    return folders;
}
