#include <iostream>

#include "task.h"
#include "io.h"
#include "windows.h"

int main() {
    using namespace std;

    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    while (true) {
        cout << "Введите номер действия: " << endl;
        cout << "1. Рассчитать минимальную стоимость из файла" << endl;
        cout << "0. Выход из программы" << endl;

        int n;
        bool success_read_int = Io::ReadInt(n);

        if (!success_read_int) {
            cout << "Некорректный ввод. Попробуйте еще раз" << endl;
            continue;
        }

        switch (n) {
            case 1:
                StartTask();
                break;
            case 0:
                cout << "Выход из программы" << endl;
                return 0;
            default:
                cout << "Некорректное действие" << endl;
                break;
        }
    }
}
