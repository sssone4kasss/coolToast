#pragma once


#include <iostream>
#include <string>
#include <vector>

#include "sqlite/sqlite3.h"

/**
Структура для хранения информации о тосте.
*/
struct ToastRecord {
    unsigned long int id;   ///< ID тоста
    std::string text;       ///< Текст тоста 
    std::string category;   ///< Название категории тоста
};


class CoolToast {
public:
    CoolToast(const char* dbfilepath);
    ~CoolToast();
   
    std::string get_rand_toast() const;

   
    std::string get_cat_toast(std::string category) const;


    std::vector<ToastRecord> get_toasts(int page) const;


    int add_toast(std::string text, std::string category) const;

 
    int delete_toast(int id) const;

 
private:
    sqlite3* database;  ///< Обработчик объекта соединения с БД 
};

