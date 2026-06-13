
#ifndef PLUSPROJECT2_IO_H
#define PLUSPROJECT2_IO_H

#include "file_exception.h"
#include "iostream"

class Io {
private:
    /*
     * Очищает буфер ввода
     */
    static void ClearCin() {
        using namespace std;

        cin.clear();
        cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    }
public:
    /*
     * Пытается получить число типа int из консоли. Если ввод корректный, то возвращает true и задает в переменную value введенное значение.
     * Иначе возвращает false
     */
    static bool ReadInt(int &value) {
        using namespace std;

        cin >> value;

        if (cin.fail()) {
            ClearCin();
            return false;
        }

        ClearCin();
        return true;
    }

    static void PrintFileExceptionDesc(const FileException& code) {
        using namespace std;

        switch (code) {
            case FILE_NOT_OPENED:
                cout << "Не удалось открыть файл" << endl;
                break;
            case FILE_INVALID_FORMAT:
                cout << "Неверный формат данных в файле" << endl;
                break;
            case FILE_NO_CHIEF:
                cout << "Не указан главный чиновник" << endl;
                break;
            case FILE_MULTIPLE_CHIEFS:
                cout << "Указано несколько главных чиновников" << endl;
                break;
            case FILE_INVALID_PARENT:
                cout << "Некорректный ID начальника" << endl;
                break;
            case FILE_LOOP_DETECTED:
                cout << "Обнаружен цикл в иерархии" << endl;
                break;
            case FILE_EMPTY:
                cout << "Файл пуст" << endl;
                break;
            default:
                cout << "Неизвестная ошибка" << endl;
                break;
        }
    }
};

#endif //PLUSPROJECT2_IO_H