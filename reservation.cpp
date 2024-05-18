#include <iostream>
#include <fstream>
#include <string>
#include <locale.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <regex>

using namespace std;

struct Reservation {
    string tableNumber;
    string date;
    string time;
    int numberOfPeople;
};

bool isReserved(Reservation res) {
    string line;
    ifstream inFileRead("reservations.csv");

    if (inFileRead.is_open()) {
        while (getline(inFileRead, line)) {
            string delimiter = ";\t";
            size_t pos = 0;
            string token;

            string tableNumber, date, time;

            pos = line.find(delimiter);
            tableNumber = line.substr(0, pos);
            line.erase(0, pos + delimiter.length());

            pos = line.find(delimiter);
            date = line.substr(0, pos);
            line.erase(0, pos + delimiter.length());

            pos = line.find(delimiter);
            time = line.substr(0, pos);
            line.erase(0, pos + delimiter.length());

            if (tableNumber == res.tableNumber && date == res.date && time == res.time) {
                return true;
            }
        }
        inFileRead.close();
    }
    else {
        cout << "Не удалось открыть файл.";
    }
    return false;
}


int main() {
    setlocale(LC_ALL, "Rus");
    ifstream inFile("reservations.csv");
    if (!inFile.good()) {
        ofstream outFile("reservations.csv");
        if (outFile.is_open()) {
            outFile << "Номер столика;Дата;Время;Количество человек\n";
            outFile.close();
        }
        else {
            cout << "Не удалось открыть файл.";
            return 1;
        }
    }

    inFile.close();
    Reservation res;
   
   

    cout << "Введите номер столика: ";
    cin >> res.tableNumber;

    cin.ignore();  // Игнорировать символ новой строки после ввода числа

    string inputDate;
    time_t inputTime;
    time_t tomorrowTime;
    do {
        cout << "Введите дату бронирования (дд.мм.гггг): ";
        getline(cin, inputDate);

        // Приводим введенную дату к формату "дд.мм.гггг" для корректного сравнения

        istringstream ss(inputDate);
        tm tm = {};
        ss >> get_time(&tm, "%d.%m.%Y");

        if (ss.fail()) {
            cout << "Некорректный формат даты. Пожалуйста, введите дату в формате дд.мм.гггг\n";
        }

        inputTime = mktime(&tm);
        tomorrowTime = chrono::system_clock::to_time_t(chrono::system_clock::now() + chrono::hours(24));

        if (inputTime < tomorrowTime) {
            cout << "Вы можете бронировать только с завтрашнего дня. Пожалуйста, введите корректную дату.\n";
        }
    } while (inputTime < tomorrowTime);

    res.date = inputDate;

    regex timePattern("^([01]?[0-9]|2[0-3]):[0-5][0-9]$");
    while (true) {
    cout << "Введите время бронирования (чч:мм): ";
    getline(cin, res.time);
    if (regex_match(res.time, timePattern)) {
       cout << "Время введено корректно." << endl;
        break;
    }
    else {
        cout << "Ошибка! Пожалуйста, введите время в корректном формате." << endl;
    }
    }

    cout << "Введите количество человек: ";
    cin >> res.numberOfPeople;
    //проверка на соответствие количеству людей (не больше 4)
    if (res.numberOfPeople > 4) {
        cout << "Таких столиков нет ";
        exit(1);
    }
    //сравнение
    if (isReserved(res)) {
        cout << "Место уже забронировано.";
        return 0;
    }
    //запись в файл
    ofstream outFile("reservations.csv", ios::app);
    if (outFile.is_open()) {
        outFile << res.tableNumber << ";\t";
        outFile << res.date << ";\t";
        outFile << res.time << ";\t";
        outFile << res.numberOfPeople << "\n";
        outFile.close();
    }
    else {
        cout << "Не удалось открыть файл.";
    }
    //вывод всего 
    string line;
    ifstream inFileRead("reservations.csv");
    if (inFileRead.is_open()) {
        while (getline(inFileRead, line)) {
            cout << line << '\n';
        }
        inFileRead.close();
    }
    else {
        cout << "Не удалось открыть файл.";
    }
    cout << "Презентация на тему Github в качестве интегрированной среды разработки";
    return 0;
}
