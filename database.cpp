#include "database.h"

DataBase::DataBase()
{

}

StatusCode DataBase::insertFromJson(const QJsonObject &object)
{
    auto task = Task::fromJson(object);

    return insertIntoTaskTable(task, getParentTask(object.value("parent_task").toString())) ? SUCCESSFULY : ERROR;
}

StatusCode DataBase::deleteFromJson(const QJsonObject &object)
{
    auto task = Task::fromJson(object);
    return deleteFromTaskTable(task) ? SUCCESSFULY : ERROR;
}

void DataBase::dropTables(){
    QSqlQuery query;
    query.exec("DROP TABLE task_table CASCADE;");
    query.exec("DROP TABLE employee_table CASCADE;");
}

bool DataBase::isTableHaveData(const QString table_name ){
    QString query = "select * from "+table_name;
    QSqlQuery qsql;
    qsql.exec(query);
    return qsql.next();
}

bool DataBase::restoreDataBase()
{
    if (!openDataBase())
    {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }

    dropTables();
    return !createEmploeeTable() || !createTaskTable();
}

bool DataBase::openDataBase()
{
    if(db.isOpen())
        db.close();

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("postgres");
    db.setUserName("postgres");
    db.setPassword("Fakel2004");

    return db.open();
}

void DataBase::closeDataBase()
{
    db.close();
}

bool DataBase::createEmploeeTable(){
    QString query = "CREATE TABLE employee_table(employee_id bigserial PRIMARY KEY, "
                    "surname VARCHAR(20), "
                    "name VARCHAR(20), "
                    "patronymic VARCHAR(20), "
                    "position VARCHAR(20))";

    QSqlQuery qsql;
    bool result = qsql.exec(query);
    if(!result)
    {
        qDebug() << "DataBase: error of create employee table";
        qDebug() << qsql.lastError().text().toUtf8();
    }

    return result;
}

bool DataBase::createTaskTable(){

    QString query = "CREATE TABLE task_table("
                    "task_id bigserial PRIMARY KEY , "
                    "task_name VARCHAR(100), "
                    "parent_task int ,"
                    "dependence boolean,"
                    "employee_id int ,"
                    "last_date date ,"
                    "status varchar(20),"
                    "FOREIGN KEY (employee_id) REFERENCES employee_table(employee_id))";
    QSqlQuery qsql;
    bool result = qsql.exec(query);
    if(!result)
    {
        qDebug() << "DataBase: error of create task table";
        qDebug() << qsql.lastError().text().toUtf8();
    }

    return result;
}

bool DataBase::insertIntoTaskTable(const Task &task, int parentTask)
{
    QString query = "INSERT INTO  task_table ("
                    "task_name,"
                    "parent_task,"
                    "dependence,"
                    "employee_id,"
                    "last_date,"
                    "status)"
                    "VALUES (:Task_name, :Parent_task, :Dependence, :Employee_id, :Last_date, :Status)";
    QSqlQuery qsql;

    qsql.prepare(query);

    qsql.bindValue(":Task_name",       task.getTaskName());
    qsql.bindValue(":Parent_task",     parentTask);
    qsql.bindValue(":Dependence",      task.getDependency());
    qsql.bindValue(":Employee_id",     getEmployeeID(task));
    qsql.bindValue(":Last_date",       task.getDate());
    qsql.bindValue(":Status",          task.getStatus());

    bool result = qsql.exec();
    if(!result){
        qDebug() << "error insert into task table";
        qDebug() << qsql.lastError().text();
    }

    return result;
}

bool DataBase::deleteFromTaskTable(const Task &task)
{
    QString query = "DELETE FROM task_table "
                    "where task_name = :bindTask and "
                    "employee_id = :bindEmployee";
    QSqlQuery qsql;
    qsql.prepare(query);

    qsql.bindValue(":bindEmployee", getEmployeeID(task));
    qsql.bindValue(":bindTask", task.getTaskName());

    bool result = qsql.exec();
    if(!result)
    {
        qDebug()<<"error in delete request";
        qDebug() << qsql.lastError();
    }

    return result;
}

QJsonArray DataBase::getTaskTableData(const QDate date)
{
    QString query = "select "
                    "employee_table.name,"
                    "employee_table.surname,"
                    "task_table.task_name,"
                    "task_table.last_date,"
                    "task_table.status"
                    " from "
                    "employee_table "
                    " left join task_table "
                    " on "
                    "(employee_table.employee_id = task_table.employee_id) "
                    "where task_table.last_date <= :bindDate";


    QSqlQuery qsql;
    qsql.prepare(query);
    qsql.bindValue(":bindDate", date);

    QJsonArray data;
    if(qsql.exec())
    {
        while(qsql.next())
        {
            QJsonObject object;
            object.insert("name", qsql.value(0).toString());
            object.insert("surname",qsql.value(1).toString());
            object.insert("task_name",qsql.value(2).toString());
            object.insert("last_date",qsql.value(3).toString());
            object.insert("status",qsql.value(4).toString());
            data.append(object);
        }
    } else
        qDebug()<<"Query not success " << qsql.lastError();

    return data;
}

QJsonArray DataBase::getTaskTableData()
{
    QString query = "select "
                    "employee_table.name,"
                    "employee_table.surname,"
                    "task_table.task_name,"
                    "task_table.last_date,"
                    "task_table.status"
                    " from "
                    "employee_table "
                    " left join task_table "
                    " on "
                    "employee_table.employee_id = task_table.employee_id ";


    QSqlQuery qsql;

    QJsonArray data;
    if(qsql.exec(query))
    {
        while(qsql.next())
        {
            QJsonObject object;
            object.insert("name", qsql.value(0).toString());
            object.insert("surname",qsql.value(1).toString());
            object.insert("task_name",qsql.value(2).toString());
            object.insert("last_date",qsql.value(3).toString());
            object.insert("status",qsql.value(4).toString());
            data.append(object);
        }
    } else
        qDebug()<<"Query not success " << qsql.lastError();

    return data;
}

int DataBase::getEmployeeID(const Task &task)
{
    QString query ="select employee_id "
                   " from employee_table "
                   " where name = :bindName and "
                   " surname = :bindSurname and "
                   " patronymic = :bindPantronymic;";


    QSqlQuery qsql;

    qsql.prepare(query);

    qsql.bindValue(":bindSurname", task.getSurname());
    qsql.bindValue(":bindName", task.getName());
    qsql.bindValue(":bindPantronymic",  task.getPatronymic());

    if(qsql.exec())
    {
        if((qsql.size()!=0) && (qsql.size() <= 1))
        {
            qsql.next();
            return qsql.value(0).toInt();
        }
    } else
    {
        qDebug()<<"error in select id request";
        qDebug() << qsql.lastError();
    }

    return -1;
}

int DataBase::getParentTask(const QString task_name){
    QString query ="select task_id "
                   " from task_table "
                   "where task_table.task_name = :bindTask";

    QSqlQuery qsql;
    qsql.prepare(query);
    qsql.bindValue(":bindTask",task_name);

    if(qsql.exec())
    {
        if((qsql.size()!=0) && (qsql.size() <= 1))
        {
            qsql.next();
            return qsql.value(0).toInt();
        }

    } else {
        qDebug()<<"error in select  parent id request";
        qDebug() << qsql.lastError().text();
    }

    return 0;
}




