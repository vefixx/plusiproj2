
#ifndef PLUSPROJECT2_FILE_EXCEPTIONS_H
#define PLUSPROJECT2_FILE_EXCEPTIONS_H

/*
 * Ошибки при загрузки чиновников из файла
 */
enum FileException {
    FILE_NOT_OPENED = 0, // Файл не открыт
    FILE_INVALID_FORMAT = 1, // Некорректный формат файла
    FILE_NO_CHIEF = 2, // В файле нет главного чиновника
    FILE_MULTIPLE_CHIEFS = 3, // В файле есть несколько главных чиновников
    FILE_INVALID_PARENT = 4, // В файле есть чиновник с некорректными данными
    FILE_LOOP_DETECTED = 5, // В файле у главного чиновника есть родитель начальник (родитель)
    FILE_EMPTY = 6 // Файл пустой
};

#endif //PLUSPROJECT2_FILE_EXCEPTIONS_H