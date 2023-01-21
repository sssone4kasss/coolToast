
#include "toasts_cli.h"

/**
\brief Функция для проверки строки на ввод только чисел.
\param[in] s Входная строка
*/
bool is_number(const std::string &s) {
    bool is_all = true;
    for (const auto& c : s) {
        is_all = std::isdigit(c);
    }
    return is_all && !s.empty();
}

/**
\brief Конструктор для создания  объекта CoolToastCLI

\param[in] ct Объект CoolToast, который хранит тосты
*/
CoolToastCLI::CoolToastCLI(const CoolToast& ct)
    : cool_toast(ct), should_exit(false) {}


/**
\brief Метод для  работы с командной строкой

Функция выполняет обработку ввода данных от пользователя, а также вывод записей из БД и меню для работы с программой.
Программа выполняется в бесконечном цикле while, пока не произойдет ввода 'exit' или 'quit'.

Команда help выводит информацию о командах для работы с программой.

Поддерживаемые команды:

rand   - рандомный тост

cat    - рандомный тост с заданной категорией

ls     - список тостов

add    - добавить новый тост

delete - удалить тост по ID

exit|quit - выйти из программы


Для постраничного вывода записей поддерживаются вспомогательные команды:

n - следующая страница 

p - предыдущая страница 

r - обновить вывод

q - выход из данного режима

<number> - переход к странице <number>
*/
void CoolToastCLI::start() {

    while (!this->should_exit) {
        std::string user_input;
        std::cout << ">> ";
        std::getline(std::cin, user_input);

        if (user_input == "rand") {
            std::cout << this->cool_toast.get_rand_toast() << std::endl;
        } else if (user_input == "cat") {
            std::cout << "Enter category: ";
            std::string category;
            std::getline(std::cin, category);

            std::cout << this->cool_toast.get_cat_toast(category) << std::endl;
        } else if (user_input == "ls") {
            int current_page = 0;
            bool is_done = false;

            while (!is_done) {
                std::cout << "Page " << current_page << "\n";
                auto toasts = this->cool_toast.get_toasts(current_page);

                std::cout << "| ID |  Category |  Text |\n";
                for (const auto& toast : toasts) {
                    std::cout << "| " << toast.id << " |  " << toast.category << " |  " << toast.text << " | " << std::endl;
                }

                while (true) {
                    std::cout << "\n >> [n - next page, p - previous, r - refresh, q - quit, <number> - go to page]: ";
                    std::string action;
                    std::getline(std::cin, action);
                    std::cout << std::endl;

                    if (action == "n") {
                        current_page++;
                        break;
                    } else if (action == "q") {
                        is_done = true;
                        break;
                    } else if (action == "p") {
                        if (current_page - 1 < 0) {
                            std::cout << "It's last page\n";
                            continue;
                        }
                        current_page--;
                        break;
                    } else if (action == "r") {
                        break;
                    } else if (is_number(action)) {
                        int requested_page = std::stoi(action);
                        if (requested_page < 0) {
                            std::cout << "Invalid page\n";
                            break;
                        }
                        current_page = requested_page;
                        break;
                    } else {
                        std::cout << "Invalid action\n";
                        continue;
                    }
                }

            }

        } else if (user_input == "help") {
            std::cout << "Avaible commands:\n";
            std::cout << "rand      - random toast\n";
            std::cout << "cat       - get random toast from category\n";
            std::cout << "ls        - list toasts\n";
            std::cout << "add       - add new toast\n";
            std::cout << "delete    - delete toast by id\n";
            std::cout << "exit|quit - exit the program\n";
        } else if (user_input == "exit" || user_input == "quit") {
            this->should_exit = true;
        } else if (user_input == "add") {
            std::cout << "Enter text: ";
            std::string text;
            std::getline(std::cin, text);
            std::cout << "Enter category: ";
            std::string cat;
            std::getline(std::cin, cat);
            this->cool_toast.add_toast(text, cat);

        } else if (user_input == "delete") {

            std::cout << "Enter id: ";
            std::string input_id;
            std::getline(std::cin, input_id);
            if (!is_number(input_id)) {
                std::cout << "Must provide number";
            } else {
                this->cool_toast.delete_toast(std::stoi(input_id));
            }

        } else {
            std::cout << "Invalid command\n";
        }
    }
}

