#pragma once

#include <QDate>
#include <QString>
#include <QJsonObject>

class Task{

public:
    Task(const QString& name, const QString& surname, const QString& patronymic, const QString& taskName,const bool& dependency,const QString& parent_task, const QDate& date, const QString& status);
    Task(const QString& name, const QString& surname, const QString& patronymic, const QString& taskName);


    QJsonObject toJson() const;

    static Task fromJson(const QJsonObject& object);

    const QString &getName() const;

    const QString &getSurname() const;

    const QString &getTaskName() const;

    const QString &getStatus() const;

    const QDate &getDate() const;

    const QString &getPatronymic() const;

    bool getDependency() const;

    const QString &getParentTask() const;

private:
    QString name;
    QString surname;
    QString patronymic;
    QString taskName;
    bool dependency = false;
    QString parentTask = "";
    QDate date = {};
    QString status = "";

};
