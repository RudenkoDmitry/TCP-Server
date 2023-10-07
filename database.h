#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <Task.h>

#include <types.h>
class DataBase
{
public:
    DataBase();
    bool openDataBase();
    StatusCode insertFromJson(const QJsonObject& object);
    StatusCode deleteFromJson(const QJsonObject& object);
    bool isTableHaveData(const QString table_name);
    QJsonArray getTaskTableData(const QDate date);
    QJsonArray getTaskTableData();
    void dropTables();

private:
    int getEmployeeID(const Task &task);
    int getParentTask(const QString task_name);
    bool insertIntoTaskTable(const Task &task, int parentTask = 0);
    bool deleteFromTaskTable(const Task &task);
    bool restoreDataBase();
    void closeDataBase();
    bool createEmploeeTable();
    bool createTaskTable();


private:
    QSqlDatabase db;
};

#endif // DATABASE_H
