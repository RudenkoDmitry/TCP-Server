#include "task.h"

Task::Task(const QString& name,
           const QString& surname,
           const QString& patronymic,
           const QString& taskName,
           const bool& dependency,
           const QString& parentTask,
           const QDate& date,
           const QString& status) :
    name(name),surname(surname),
    patronymic(patronymic),
    taskName(taskName),
    dependency(dependency),
    parentTask(parentTask),
    date(date),
    status(status) {}

Task::Task(const QString &name,
           const QString &surname,
           const QString &patronymic,
           const QString &taskName) :
    name(name),
    surname(surname),
    patronymic(patronymic),
    taskName(taskName) {}


QJsonObject Task::toJson() const
{
    QJsonObject object;
    object.insert("name", name);
    object.insert("surname", surname);
    object.insert("patronymic", patronymic);
    object.insert("task_name", taskName);
    object.insert("dependency", dependency);
    object.insert("parent_task", parentTask);
    object.insert("last_date", date.toString());
    object.insert("status", status);
    return object;
}

Task Task::fromJson(const QJsonObject &object)
{
    return Task(object.value("name").toString(),
                object.value("surname").toString(),
                object.value("patronymic").toString(),
                object.value("task_name").toString(),
                object.value("dependency").toBool(),
                object.value("parent_task").toString(),
                QDate::fromString(object.value("last_date").toString()),
                object.value("status").toString());
}

const QString &Task::getName() const
{
    return name;
}

const QString &Task::getSurname() const
{
    return surname;
}

const QString &Task::getTaskName() const
{
    return taskName;
}

const QString &Task::getStatus() const
{
    return status;
}

const QDate &Task::getDate() const
{
    return date;
}

const QString &Task::getPatronymic() const
{
    return patronymic;
}

bool Task::getDependency() const
{
    return dependency;
}

const QString &Task::getParentTask() const
{
    return parentTask;
}
