
#ifndef PLUSPROJECT2_SUBORDINATE_LIST_H
#define PLUSPROJECT2_SUBORDINATE_LIST_H
#include <cstddef>

/*
 *  Узел списка подчиненных
 */
struct  SubordinateNode {
    int official_id;
    SubordinateNode* next;

    SubordinateNode(int id) {
        official_id = id;
        next = nullptr;
    }
};

class SubordinateList {
private:
    SubordinateNode* head;
    std::size_t count;

public:
    SubordinateList() {
        head = nullptr;
        count = 0;
    }

    ~SubordinateList() {
        Clear();
    }

    /*
     *  Очистка списка
     */
    void Clear() {
        // Переходим к следующему узлу, удаляем предыдущий
        while (head != nullptr) {
            SubordinateNode* temp = head;
            head = head->next;
            delete temp;
        }
        count = 0;
    }

    bool IsEmpty() const {
        return head == nullptr;
    }

    SubordinateNode* GetHead() const {
        return head;
    }

    size_t Size() const {
        return count;
    }

    /*
     *  Добавляет нового чиновника в список
     */
    void Add(int official_id) {
        // Новому узлу присваем текущий head как следующий
        // head устанавливаем на новый узел

        SubordinateNode* new_node = new SubordinateNode(official_id);
        new_node->next = head;
        head = new_node;
        count++;
    }
};

#endif //PLUSPROJECT2_SUBORDINATE_LIST_H