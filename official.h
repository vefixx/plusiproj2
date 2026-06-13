
#ifndef PLUSPROJECT2_OFFICIAL_H
#define PLUSPROJECT2_OFFICIAL_H

/*
 * Чиновник
 */
class Official {
private:
    int id; // ID чиновника
    int parent_id;  // ID его начальника
    int bribe;  // взятка
    int best_subordinate_id; // ID подчиненного с самой минимальной стоимости подписи (нужен будет для восстановлении пути)
public:
    Official() {
        id = 0;
        parent_id = -1;
        bribe = 0;
    }

    Official(int i, int p_id, int b) {
        id = i;
        parent_id = p_id;
        bribe = b;
    }

    int GetId() const {
        return id;
    }

    int GetParentId() const {
        return parent_id;
    }

    int GetBribe() const {
        return bribe;
    }

    int GetBestSubordinateId() const {
        return best_subordinate_id;
    }

    void SetBestSubordinateId(int sub_id) {
        best_subordinate_id = sub_id;
    }
};

#endif //PLUSPROJECT2_OFFICIAL_H