/**
\mainpage Приложение для генерации тостов

Принцип работы программы: 

- Программа взаимодействует с базой данных SQL Lite. 

- БД хранится вся необходимая информация о тостах: ID, текст тоста, категория тоста. 

- Программа реализует функции, которые осуществляют запросы в БД, а также функции работы с командной строкой: пользовательский ввод и вывод.

- Для работы с SQL Lite используются функции из sqlite3.h, реализующие API БД.
*/


/**
\file 
Функция main принимает в качестве входных аргументов путь до базы данных. 
Реализована проверка на ввод входных аргументов.
*/

#include <cctype>
#include <iostream>
#include <functional>


#include "toasts.h"
#include "toasts_cli.h"


int main(int argc, char** argv) {

    if (argc < 2) {
        std::cout << "Error: must provide path to toasts sqlite3 database\n";
        return 1;
    } else if (argc > 2) {
        std::cout << "Error: got more than 1 arguments; must provide path to toats sqlite3 database\n";
        return 1;
    }

    const char* dbfilepath = argv[1];
    std::cout << "Got path: " << dbfilepath << std::endl;

    CoolToast ct(dbfilepath);

    CoolToastCLI toasts_cli(ct);

    toasts_cli.start();

    return 0;
}

