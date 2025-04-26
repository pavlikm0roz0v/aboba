#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <cstdio>
#include <io.h>
#include <stdio.h>
using namespace std;
//структура
struct Patient {
    char fullName[50];
    char adres[50];
    char phoneNumber[15];
    char visitDate[11];
};

void Create(FILE* file);
void ShowPatients(FILE* file);
void SearchPatient(FILE* file);
void Sort(FILE* file);
void QuickSortMain(FILE* file);
void QuickSort(FILE* file, int left, int right);
void BinarySearch(FILE* file);
void Task(FILE* file);
void NewFile(FILE* file);
void WriteToFile1(FILE* file);
void ShowPatients2(FILE* file2);
void DeletePatient(FILE* file);
void DeleteList(FILE* file);
void ShowPatientInfo(Patient* p);
int data(Patient* p);
int data(char* data);

void Menu() {
    FILE* file = fopen("patients.txt", "rb");
    if (!file)
    {
        file = fopen("patients.txt", "wb");
        cout << "Так как файл обнаружить не удалось, создан новый!" << endl;
    }
    fclose(file);
    bool menu = true;
    while (menu) {
        cout << "\n(1) - Добавить пациента" << endl;
        cout << "(2) - Просмотреть всех пациентов" << endl;
        cout << "(3) - Поиск пациента" << endl;
        cout << "(4) - Сортировка методом прямого выбора" << endl;
        cout << "(5) - Сортировка методом QuickSort" << endl;
        cout << "(6) - Двоичный поиск" << endl;
        cout << "(7) - Пациенты, посетившие поликлинику за определенный месяц" << endl;
        cout << "(8) - Создать новый файл" << endl;
        cout << "(9) - Записать данные из второго файла в первый" << endl;
        cout << "(10) - Просмотреть всех пациентов из второго файла" << endl;
        cout << "(11) - Удалить запись пациента" << endl;
        cout << "(12) - Очистить список пациентов" << endl;
        cout << "(0) - Выйти из программы" << endl;
        cout << ">";
        int choice;
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nНекорректный ввод! Пожалуйста, введите значение от 0 до 9" << endl;
            continue;
        }
        switch (choice) {
        case 1: Create(file); break;
        case 2: ShowPatients(file); break;
        case 3: SearchPatient(file); break;
        case 4: Sort(file); break;
        case 5: QuickSortMain(file); break;
        case 6: BinarySearch(file); break;
        case 7: Task(file); break;
        case 8: NewFile(file); break;
        case 9: WriteToFile1(file); break;
        case 10: ShowPatients2(file); break;
        case 11: DeletePatient(file); break;
        case 12: DeleteList(file); break;
        case 0: menu = false; break;
        default: cout << "\nНекорректный ввод! Введите число от 0 до 9" << endl; break;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    setlocale(LC_ALL, "");
    system("chcp 1251");
    Menu();
    return 0;
}

void Create(FILE* file) {
    file = fopen("patients.txt", "ab");
    Patient p;
    cout << "\nВведите фамилию, адрес проживания, номер телефона, дату посещения (год.месяц.день)\n>";
    cin >> p.fullName >> p.adres >> p.phoneNumber >> p.visitDate;
    fwrite(&p, sizeof(Patient), 1, file);
    fclose(file);
}

void ShowPatients(FILE* file) {
    file = fopen("patients.txt", "rb");
    if (_filelength(_fileno(file)) == 0) {
        cout << "\nФайл пуст!" << endl;
        fclose(file);
        return;
    }
    Patient p;
    while (fread(&p, sizeof(Patient), 1, file)) {
        ShowPatientInfo(&p);
    }
    fclose(file);
}

void SearchPatient(FILE* file) {
    file = fopen("patients.txt", "rb");
    if (_filelength(_fileno(file)) == 0) {
        cout << "\nФайл пуст!" << endl;
        fclose(file);
        return;
    }
    int found = 0;
    char searchKey[50];
    cout << "\nВведите фамилию/адрес проживания/номер телефона/дату посещения (год.месяц.день) для поиска\n>";
    cin >> searchKey;
    while (strlen(searchKey) > 49)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nНекорректный ввод! Пожалуйста, правильно введите данные\n>";
        cin >> searchKey;
    }
    Patient p;
    while (fread(&p, sizeof(Patient), 1, file)) {
        if (strcmp(p.fullName, searchKey) == 0 || strcmp(p.adres, searchKey) == 0 || strcmp(p.phoneNumber, searchKey) == 0 || strcmp(p.visitDate, searchKey) == 0) {
            found = 1;
            ShowPatientInfo(&p);
        }
    }
    if (found == 0) cout << "\nСовпадений не найдено!" << endl;
    fclose(file);
}

void Sort(FILE* file) {
    file = fopen("patients.txt", "rb+");
    if (_filelength(_fileno(file)) == 0) {
        cout << "\nФайл пуст!" << endl;
        fclose(file);
        return;
    }
    Patient min, temp;
    int i, j, minPos, numP = _filelength(_fileno(file)) / sizeof(Patient);
    for (i = 0; i < numP - 1; i++) {
        fseek(file, sizeof(Patient) * i, 0);
        fread(&min, sizeof(Patient), 1, file);
        minPos = i;
        for (j = i + 1; j < numP; j++) {
            fseek(file, sizeof(Patient) * j, 0);
            fread(&temp, sizeof(Patient), 1, file);
            if (strcmp(temp.visitDate, min.visitDate) < 0) {
                min = temp;
                minPos = j;
            }
        }
        if (minPos != i) {
            fseek(file, i * sizeof(Patient), 0);
            fread(&temp, sizeof(Patient), 1, file);
            fseek(file, i * sizeof(Patient), 0);
            fwrite(&min, sizeof(Patient), 1, file);
            fseek(file, minPos * sizeof(Patient), 0);
            fwrite(&temp, sizeof(Patient), 1, file);
        }
    }
    cout << "\nСписок был успешно отсортирован!" << endl;
    fclose(file);
}

void QuickSortMain(FILE* file) {
    file = fopen("patients.txt", "rb+");
    if (_filelength(_fileno(file)) == 0) {
        cout << "\nФайл пуст!" << endl;
        fclose(file);
        return;
    }
    int numP = _filelength(_fileno(file)) / sizeof(Patient);
    QuickSort(file, 0, numP - 1);
    cout << "\nСписок был успешно отсортирован!" << endl;
    fclose(file);
}

void QuickSort(FILE* file, int l, int r) {
    if (l >= r) return;
    int mid = (l + r) / 2;
    Patient p, pI, pJ;
    fseek(file, mid * sizeof(Patient), 0);
    fread(&p, sizeof(Patient), 1, file);
    int i = l, j = r;
    while (i <= j) {
        do {
            fseek(file, i * sizeof(Patient), 0);
            fread(&pI, sizeof(Patient), 1, file);
        } while (strcmp(pI.visitDate, p.visitDate) < 0 && ++i <= r);
        do {
            fseek(file, j * sizeof(Patient), 0);
            fread(&pJ, sizeof(Patient), 1, file);
        } while (strcmp(pJ.visitDate, p.visitDate) > 0 && --j >= l);
        if (i <= j) {
            fseek(file, i * sizeof(Patient), 0);
            fwrite(&pJ, sizeof(Patient), 1, file);
            fseek(file, j * sizeof(Patient), 0);
            fwrite(&pI, sizeof(Patient), 1, file);
            i++;
            j--;
        }
    }
    QuickSort(file, l, j);
    QuickSort(file, i, r);
}

void BinarySearch(FILE* file) {
    QuickSortMain(file);
    file = fopen("patients.txt", "rb");
    if (_filelength(_fileno(file)) == 0) {
        cout << "\nФайл пуст!" << endl;
        fclose(file);
        return;
    }
    Patient p;
    int i = 0, found = 0, numP = _filelength(_fileno(file)) / sizeof(Patient);
    int j = numP - 1;
    char searchData[11];
    cout << "\nВведите дату посещения (год.месяц.день)\n>";
    cin >> searchData;
    while (strlen(searchData) != 10 || searchData[4] != '.' || searchData[7] != '.')
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nНекорректный ввод! Пожалуйста, правильно введите дату (год.месяц.день)\n>";
        cin >> searchData;
    }
    while (i <= j) {
        int mid = i + (j - i) / 2;
        fseek(file, mid * sizeof(Patient), 0);
        fread(&p, sizeof(Patient), 1, file);
        if (strcmp(p.visitDate, searchData) == 0) {
            ShowPatientInfo(&p);
            found = 1;
            int temp = mid - 1;
            while (temp >= i) {
                fseek(file, temp * sizeof(Patient), 0);
                fread(&p, sizeof(Patient), 1, file);
                if (strcmp(p.visitDate, searchData) == 0) {
                    ShowPatientInfo(&p);
                    temp--;
                }
                else {
                    break;
                }
            }
            temp = mid + 1;
            while (temp <= j) {
                fseek(file, temp * sizeof(Patient), 0);
                fread(&p, sizeof(Patient), 1, file);
                if (strcmp(p.visitDate, searchData) == 0) {
                    ShowPatientInfo(&p);
                    temp++;
                }
                else {
                    break;
                }
            }
            break;
        }
        else if (strcmp(p.visitDate, searchData) < 0) {
            i = mid + 1;
        }
        else {
            j = mid - 1;
        }
    }
    if (found == 0) cout << "\nСовпадений не найдено!" << endl;
    fclose(file);
}

void Task(FILE* file) {
    file = fopen("patients.txt", "rb");
    if (_filelength(_fileno(file)) == 0) {
        cout << "\nФайл пуст!" << endl;
        fclose(file);
        return;
    }
    int i = 0, j = 0, n = 0, numP = _filelength(_fileno(file)) / sizeof(Patient);
    char searchData[8];
    Patient p, temp;
    cout << "\nВведите год и месяц\n>";
    cin >> searchData;
    while (strlen(searchData) != 7 || searchData[4] != '.')
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nНекорректный ввод! Пожалуйста, правильно введите дату (год.месяц)\n>";
        cin >> searchData;
    }
    rewind(file);
    while (fread(&p, sizeof(Patient), 1, file)) {
        if (data(&p) == data(searchData)) {
            n++;
        }
    }
    if (n == 0) {
        cout << "\nСовпадений не найдено!" << endl;
        fclose(file);
        return;
    }
    Patient* strArr = new Patient[n];
    rewind(file);
    while (fread(&p, sizeof(Patient), 1, file)) {
        if (data(&p) == data(searchData)) {
            strArr[i++] = p;
        }
    }
    for (i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (strcmp(strArr[j].fullName, strArr[j + 1].fullName) > 0) {
                temp = strArr[j];
                strArr[j] = strArr[j + 1];
                strArr[j + 1] = temp;
            }
        }
    }
    for (i = 0; i < n; i++)
    {
        cout << "\nФамилия: " << strArr[i].fullName << "\nАдрес проживания: " << strArr[i].adres << endl;
        cout << "Телефон: " << strArr[i].phoneNumber << "\nДата посещения: " << strArr[i].visitDate << endl;
    }
    delete[] strArr;
    fclose(file);
}

void NewFile(FILE* file) {
    cout << "\nВы уверены, что хотите создать новый файл (данные из текущего файла перезапишутся в другой)?" << endl;
    cout << "(1) - Да" << endl;
    cout << "(0) - Нет" << endl;
    cout << ">";
    bool choice;
    cin >> choice;
    while (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nНекорректный ввод! Пожалуйста, введите 1 или 0\n>" << endl;
        cin >> choice;
    }
    if (choice == 1)
    {       
        file = fopen("patients.txt", "rb");
        FILE* file2 = fopen("patients2.txt", "wb");
        Patient p;
        while (fread(&p, sizeof(Patient), 1, file)) {
            fwrite(&p, sizeof(Patient), 1, file2);
        }
        cout << "\nНовый файл был успешно создан!" << endl; 
        fclose(file2);
        fclose(file);
    }
    else cout << "\nОперация отменена!" << endl;
}

void WriteToFile1(FILE* file) {
    cout << "\nВы уверены, что хотите записать данные из второго файла в первый?" << endl;
    cout << "(1) - Да" << endl;
    cout << "(0) - Нет" << endl;
    cout << ">";
    bool choice;
    cin >> choice;
    while (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nНекорректный ввод! Пожалуйста, введите 1 или 0\n>" << endl;
        cin >> choice;
    }
    if (choice == 1)
    {
        file = fopen("patients.txt", "rb+");
        FILE* file2 = fopen("patients2.txt", "rb");
        Patient p;
        while (fread(&p, sizeof(Patient), 1, file2)) {
            fwrite(&p, sizeof(Patient), 1, file);
        }
        cout << "\nФайл 1 успешно перезаписан!" << endl;
        fclose(file2);
        fclose(file);
    }
    else cout << "\nОперация отменена!" << endl;
}

void ShowPatients2(FILE* file2) {
    file2 = fopen("patients2.txt", "rb");
    if (_filelength(_fileno(file2)) == 0) {
        cout << "\nФайл пуст!" << endl;
        fclose(file2);
        return;
    }
    Patient p;
    while (fread(&p, sizeof(Patient), 1, file2)) {
        ShowPatientInfo(&p);
    }
    fclose(file2);
}

void DeletePatient(FILE* file) {
    file = fopen("patients.txt", "rb+");
    if (_filelength(_fileno(file)) == 0) {
        cout << "\nФайл пуст!" << endl;
        fclose(file);
        return;
    }
    cout << "\nВы уверены, что хотите удалить запись пациента(ов)?" << endl;
    cout << "(1) - Да" << endl;
    cout << "(0) - Нет" << endl;
    cout << ">";
    bool choice;
    cin >> choice;
    while (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nНекорректный ввод! Пожалуйста, введите 1 или 0\n>" << endl;
        cin >> choice;
    }
    if (choice == 0) {
        cout << "\nОперация отменена!" << endl;
        fclose(file);
        return;
    }
    int i = 0, found = 0, numP = _filelength(_fileno(file)) / sizeof(Patient);
    char searchKey[50];
    Patient p;
    cout << "\nВведите фамилию/адрес проживания/номер телефона/дату посещения (год.месяц.день) для поиска\n>";
    cin >> searchKey;
    while (strlen(searchKey) > 49)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nНекорректный ввод! Пожалуйста, правильно введите данные\n>";
        cin >> searchKey;
    }
    for (int i = 0; i < numP; ) {
        fseek(file, i * sizeof(Patient), 0);
        fread(&p, sizeof(Patient), 1, file);
        if (strcmp(p.fullName, searchKey) == 0 || strcmp(p.adres, searchKey) == 0 || strcmp(p.phoneNumber, searchKey) == 0 || strcmp(p.visitDate, searchKey) == 0) {
            found = 1;
            for (int j = i; j < numP - 1; j++) {
                fseek(file, (j + 1) * sizeof(Patient), 0);
                fread(&p, sizeof(Patient), 1, file);
                fseek(file, j * sizeof(Patient), 0);
                fwrite(&p, sizeof(Patient), 1, file);
            }
            numP--;
            _chsize(_fileno(file), numP * sizeof(Patient));
        }
        else {
            i++;
        }
    }
    if (found == 1) cout << "\nЗаписи пациента(ов) с подходящими данными успешно удалены!" << endl;
    else cout << "\nСовпадений не найдено!" << endl;
    fclose(file);
}

void DeleteList(FILE* file) {
    cout << "\nВы уверены, что хотите очистить список?" << endl;
    cout << "(1) - Да" << endl;
    cout << "(0) - Нет" << endl;
    cout << ">";
    bool choice;
    cin >> choice;
    while (cin.fail())
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\nНекорректный ввод! Пожалуйста, введите 1 или 0\n>" << endl;
        cin >> choice;
    }
    if (choice == 1)
    {
        file = fopen("patients.txt", "wb");
        cout << "\nСписок был успешно очищен!" << endl;
        fclose(file);
    }
    else cout << "\nОперация отменена!" << endl;
}

void ShowPatientInfo(Patient* p) {
    cout << "\nФамилия: " << p->fullName << "\nАдрес проживания: " << p->adres << endl;
    cout << "Телефон: " << p->phoneNumber << "\nДата посещения: " << p->visitDate << endl;
}

int data(Patient* p) {
    int day, month, year;
    sscanf(p->visitDate, "%d.%d.%d", &day, &month, &year);
    return year * 100 + month;
}

int data(char* data) {
    int year, month;
    sscanf(data, "%d.%d", &year, &month);
    return year * 100 + month;
}