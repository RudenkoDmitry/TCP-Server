#pragma once

enum StatusCode
{
    SUCCESSFULY = 0,
    ERROR
};

enum RequestType
{
    SELECT_TASK = 0,
    SELECT_OVERDUE_TASK,
    INSERT_TASK,
    REMOVE_TASK
};
