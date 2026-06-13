#ifndef PLUSPROJECT2_MINISTRY_H
#define PLUSPROJECT2_MINISTRY_H

#include <string>

#include "file_exception.h"
#include "official.h"
#include "subordinate_list.h"
#include "vector"
#include "fstream"
#include "iostream"

/*
 *  Дерево чиновников, где chief_id - ID чиновника, который является главным и корнем
 */
class Ministry {
private:
    std::vector<Official> officials;
    std::vector<SubordinateList> subordinate_lists;
    int chief_id; // ID главного чиновника

    /*
     * Используя обход в глубину DFS, проверяем наличие цикла, начиная с заданного чиновника.
     * visited - отметка для каждого узла (true/false), чтобы не обходить повторно
     * in_stack - отметка для каждого узла (true/false), находящийся в текущем пути обхода (то есть в текущем стеке рекурсии)
     *
     * Если при обходе встречается такой узел, который уже есть в текущем пути (стеке), значит обнаружен цикл и возвращает true.
     * Иначе, false.
     */
    bool HasLoopFrom(int official_id, std::vector<bool> &visited, std::vector<bool> &in_stack) const {
        // Сразу помечаем текущего чиновника как посещенного и находящегося в текущем пути
        visited[official_id] = true;
        in_stack[official_id] = true;


        // Перебираем всех подчиненных текущего чиновника
        SubordinateNode *current = subordinate_lists[official_id].GetHead();
        while (current != nullptr) {
            int sub_id = current->official_id;

            // Если подчиненный еще не был посещен, то рекурсией проверяем его поддерево
            if (!visited[sub_id]) {
                if (HasLoopFrom(sub_id, visited, in_stack))
                    return true; // Возвращаем true, если в его поддереве был найден цикл
            } else if (in_stack[sub_id]) // Если подчиненный уже посещен и есть в текущем пути обхода (стеке),
                // То возвращаем true, так как мы по факту вернулись обратно к нему
                return true;

            current = current->next;
        }

        // После обхода текущего чиновника "убираем" его из стека
        in_stack[official_id] = false;
        return false;
    }

    /*
     *  Проверка наличия циклов во всем министерстве. Проходит по всем чиновникам и запускает проверку для каждого его подчиненного
     */
    bool HasLoop() const {
        using namespace std;

        size_t n = officials.size();

        vector<bool> visited(n, false); // Метки посещения чиновника
        vector<bool> in_stack(n, false); // Стек рекурсии

        return HasLoopFrom(chief_id, visited, in_stack);
    }

public:
    Ministry() {
        chief_id = -1;
    }

    /*
     * Загружает чиновников из указанного файла. Проверяет формат входных данных и их валидность
     */
    void LoadFromFile(const std::string &filename) {
        using namespace std;

        ifstream file(filename);
        if (!file.is_open())
            throw FILE_NOT_OPENED;


        // Подсчет количества строк
        size_t lines_count = 0;
        string line;
        while (getline(file, line))
            if (!line.empty())
                lines_count++;

        if (lines_count == 0) {
            file.close();
            throw FILE_EMPTY;
        }

        file.clear();
        file.seekg(0, ios::beg); // Возврат к началу файла

        // Изменяем размер контейнеров
        officials.resize(lines_count);
        subordinate_lists.resize(lines_count);
        chief_id = -1;

        // Заполням чиновников
        int id, parent_id, bribe;
        long long bribe_ll;
        int chief_count = 0; // Количество главных чиновников. Для проверки, что чиновник в файле один
        for (size_t i = 0; i < lines_count; i++) {
            // Если файл имеет некорректный фромат
            if (!(file >> id >> parent_id >> bribe_ll)) {
                file.close();
                throw FILE_INVALID_FORMAT;
            }

            // Если ID некорректный (т.е. он превышает размер контейнера или отрицательный)
            if (id < 0 || static_cast<size_t>(id) >= lines_count) {
                file.close();
                throw FILE_INVALID_PARENT;
            }

            // переполнение взятки
            if (bribe_ll < 0 || bribe_ll > std::numeric_limits<int>::max()) {
                file.close();
                throw FILE_INVALID_FORMAT;
            }
            int bribe = static_cast<int>(bribe_ll);

            // Если взятка отрицательная, то выбрасываем ошибку
            if (bribe < 0) {
                file.close();
                throw FILE_INVALID_FORMAT;
            }

            if (parent_id != -1) {
                if (parent_id < 0 || static_cast<size_t>(parent_id) >= lines_count || parent_id == id) {
                    file.close();
                    throw FILE_INVALID_PARENT;
                }
            }

            officials[id] = Official(id, parent_id, bribe);
            if (parent_id == -1) {
                // Если у чиновника нет начальника, то он становится главным
                chief_id = id;
                chief_count++;
            } else
                subordinate_lists[parent_id].Add(id); // Добавляем текущего чиновника
        }

        file.close();

        // Если количество главные чиновников 0, то выбрасываем ошибку
        if (chief_count == 0) {
            throw FILE_NO_CHIEF;
        }

        // Если главных чиновников больше 1, то так же выбрасываем ошибку
        if (chief_count > 1) {
            throw FILE_MULTIPLE_CHIEFS;
        }

        // Если в дереве есть цикл, то выбрасываем ошибку
        if (HasLoop()) {
            throw FILE_LOOP_DETECTED;
        }
    }

    int GetChiefId() const {
        return chief_id;
    }

    /*
     * Возвращает взятку для указанного чиновника по ID
     */
    int GetBribe(int id) const {
        return officials[id].GetBribe();
    }

    /*
     * Возвращает подчиненных указанного чиновника
     */
    SubordinateList& GetSubordinates(int id) {
        return subordinate_lists[id];
    }

    size_t Size() const {
        return officials.size();
    }

    /*
     *  Устанавливает у чиновника ID его подчиненного, чья стоимость самая наименьшая
     */
    void SetBestSubordinate(int official_id, int sub_id) {
        officials[official_id].SetBestSubordinateId(sub_id);
    }

    /*
     * Рекурсивно вычисляет стоимость получения подписи указанного чиновника.
     * Для каждого чиновника стоимость равна его взятке + минимальная стоимость подписи одного из его подчиненных
     * Если у чиновника нет подчиненных, то стоимость равна только его взятке
     */
    int CalcMinCost(int official_id) {
        // Список подчиненных текущего чиновника
        SubordinateList& subordinates = GetSubordinates(official_id);

        if (subordinates.IsEmpty()) {
            SetBestSubordinate(official_id, -1); // Так как у чиновника нет подчиненных, то устанвливаем на -1
            return GetBribe(official_id);
        }

        // Поиск подчинненного с минимальной стоимостью получения его подписи
        int min_sub_cost = INT_MAX;
        int best_sub_id = -1;

        SubordinateNode* current = subordinates.GetHead();
        while (current != nullptr) {
            // У этого подчиненного ищем так же минимальную стоимость у его подчиненных
            int sub_cost = CalcMinCost(current->official_id);

            if (sub_cost < min_sub_cost) {
                // Обновляем минимальную стоимость и устанавливаем чиновника, у которого эту стоимость нашли
                min_sub_cost = sub_cost;
                best_sub_id = current->official_id;
            }

            current = current->next;
        }

        SetBestSubordinate(official_id, best_sub_id);

        // В результате возвращаем сумму между взяткой текущего чиновника и минимальной взяткой среди его подчиненных
        return SafetyIntAdd(GetBribe(official_id), min_sub_cost);
    }

    inline int SafetyIntAdd(int a, int b) {
        if (b > 0 && a > std::numeric_limits<int>::max() - b) {
            throw std::overflow_error("Сумма взяток превышает максимальное допустимое значение");
        }
        if (b < 0 && a < std::numeric_limits<int>::min() - b) {
            throw std::underflow_error("Сумма взяток вышла за минимальное допустимое значение");
        }
        return a + b;
    }

    /*
     * Выводит порядок получения подписей от главного чиновника до листа дерева (последнего чиновника).
     */
    void PrintMinCalcPath() const {
        using namespace std;

        int current_id = chief_id;
        size_t step = 1;

        // Проходимся по цепочке лучших подчиненных, пока не встретим конец (-1)
        while (current_id != -1) {
            int bribe = officials[current_id].GetBribe();
            cout << step << ") Чиновник №" << current_id << " (взятка " << bribe << " у.е.)" << endl;
            current_id = officials[current_id].GetBestSubordinateId();
            step++;
        }
    }
};

#endif //PLUSPROJECT2_MINISTRY_H
