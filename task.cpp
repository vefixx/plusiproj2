#include "task.h"

#include "io.h"
#include "iostream"
#include "ministry.h"

void StartTask() {
    using namespace std;

    cout << "Введите имя файла с данными о чиновниках: ";
    string filename;
    cin >> filename;

    Ministry ministry;

    try {
        ministry.LoadFromFile(filename);
    } catch (const FileException& e) {
        cout << "Ошибка при чтении файла: ";
        Io::PrintFileExceptionDesc(e);
        return;
    }

    cout << "Данные загружены" << endl;
    cout << "Количество чиновников: " << ministry.Size() << endl;
    cout << "ID главного чиновника: " << ministry.GetChiefId() << endl;

    int chief_id = ministry.GetChiefId();
    int min_cost;
    
    try {
        // Начинаем рассчет от главного чиновника
        min_cost = ministry.CalcMinCost(chief_id);
    }
    catch (const std::overflow_error& e) {
        cout << "Ошибка при расчете: " << e.what() << endl;
        return;
    }

    cout << "Минимальная сумма взятки: " << min_cost << endl;
    cout << "Порядок получения подписей: " << endl;
    ministry.PrintMinCalcPath();

    return;
}
