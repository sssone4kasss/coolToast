#include "toasts.h"

/**
\brief Конструктор для создания  объекта CoolToast
\param[in] dbfilepath Путь до файла

Реализована проверка на открытие БД.
*/
CoolToast::CoolToast(const char* dbfilepath) {
    if (sqlite3_open(dbfilepath, &this->database)) {
        std::cerr << "Error open database " << sqlite3_errmsg(this->database) << std::endl;
        std::exit(1);
    }
    else {
        std::cout << "Opened Database Successfully!" << std::endl;
    }
}

/**
\brief Деструктор для уничтожения объекта CoolToast.

С помощью вызова функции sqlite3_close(sqlite3*) происходит закрытие подключения с БД.
*/
CoolToast::~CoolToast() {
    sqlite3_close(this->database);
}

/**
\brief Метод для получения случайного тоста из БД
\return Текст тоста

Функция выполняет подготовку к запросу, запрос в БД, и получение ответа, с проверкой ответа от БД.
*/
std::string CoolToast::get_rand_toast() const {
    sqlite3_stmt *stmt;
    const char *sql_request = "SELECT text FROM toasts ORDER BY RANDOM() LIMIT 1;";
    int rc = sqlite3_prepare_v2(this->database, sql_request, -1, &stmt, NULL);

    std::string random_toast;

    if( rc != SQLITE_OK ) {
        std::cerr << "Error " << sqlite3_errmsg(this->database) << std::endl;
        return random_toast;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        random_toast = std::string(reinterpret_cast<const char*>(text));
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Error " << sqlite3_errmsg(this->database) << std::endl;
        return random_toast;
    }
    sqlite3_finalize(stmt);

    return random_toast;
}

/**
\brief Метод для получения тоста из БД по определенной категории 
\param[in] category Название категории
\return Текст тоста

Функция выполняет подготовку к запросу, запрос в БД, и получение ответа, с проверкой ответа от БД.
*/
std::string CoolToast::get_cat_toast(std::string category) const {
    std::string toast;

    sqlite3_stmt *stmt;
    std::string sql = "SELECT text FROM toasts WHERE category = \""+ category +"\" ORDER BY RANDOM() LIMIT 1;";

    int rc = sqlite3_prepare_v2(this->database, sql.c_str(), -1, &stmt, NULL);

    if( rc != SQLITE_OK ) {
        std::cerr << "Error " << sqlite3_errmsg(this->database) << std::endl;
        return toast;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        toast = std::string(reinterpret_cast<const char*>(text));
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Error " << sqlite3_errmsg(this->database) << std::endl;
        return toast;
    }

    sqlite3_finalize(stmt);

    return toast;
}

/**
\brief Метод выполняющий пагинацию из БД
\param[in] page Номер страницы 
\return Вектор структур ToastRecord 

Функция выполняет подготовку к запросу, запрос в БД, и получение ответа, с проверкой ответа от БД.

Данная функция выполняет пагинацию по 10 записей в странице.
*/
std::vector<ToastRecord> CoolToast::get_toasts(int page) const {
    std::vector<ToastRecord> toastRecords;

    sqlite3_stmt *stmt;
    std::string sql = "SELECT id,text,category FROM toasts LIMIT 10 OFFSET " + std::to_string(10 * page);
    int rc = sqlite3_prepare_v2(this->database, sql.c_str(), -1, &stmt, NULL);

    if( rc != SQLITE_OK ) {
        std::cerr << "Error " << sqlite3_errmsg(this->database) << std::endl;
        return toastRecords;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        unsigned long int id          = sqlite3_column_int64(stmt, 0);
        const unsigned char *text     = sqlite3_column_text(stmt, 1);
        const unsigned char *category = sqlite3_column_text(stmt, 2);

        toastRecords.push_back({
            id,
            std::string(reinterpret_cast<const char*>(text)),
            std::string(reinterpret_cast<const char*>(category))
        });
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Error " << sqlite3_errmsg(this->database) << std::endl;
        return toastRecords;
    }
    sqlite3_finalize(stmt);

    return toastRecords;
}

/**
\brief Метод для добавления тоста в БД 
\param[in] text Текст тоста
\param[in] category Название категории 
\return Статус выполнения функции

Функция выполняет подготовку к запросу, запрос в БД, и получение ответа, с проверкой ответа от БД.
*/
int CoolToast::add_toast(std::string text, std::string category) const {
    sqlite3_stmt *stmt;
    std::string sql = "INSERT INTO toasts (text, category) VALUES (\""+text+"\",\""+category+"\");";
    int rc = sqlite3_prepare_v2(this->database, sql.c_str(), -1, &stmt, NULL);

    if( rc != SQLITE_OK ) {
        std::cerr << "Error " << sqlite3_errmsg(this->database) << std::endl;
        return 1;
    }

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Error " << sqlite3_errmsg(this->database) << std::endl;
        return 1;
    }
    sqlite3_finalize(stmt);

    return 0;
}

/**
\brief Метод для удаления тоста в БД 
\param[in] id ID тоста, который хотим удалить
\return Статус удаления тоста

Функция выполняет подготовку к запросу, запрос для удаления из БД, и получение ответа, с проверкой ответа от БД.
*/
int CoolToast::delete_toast(int id) const {

    sqlite3_stmt *stmt;
    std::string sql = "DELETE FROM toasts WHERE id=" + std::to_string(id) + ";";
    int rc = sqlite3_prepare_v2(this->database, sql.c_str(), -1, &stmt, NULL);

    if( rc != SQLITE_OK ) {
        std::cerr << "Error " << sqlite3_errmsg(this->database) << std::endl;
        return 1;
    }

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "Error " << sqlite3_errmsg(this->database) << std::endl;
        return 1;
    }
    sqlite3_finalize(stmt);

    return 0;
}

