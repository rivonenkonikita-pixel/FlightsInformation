/*********************************************************************************************
* Project Type: win32_console_Application                                                    *
* Project Name: C:\Users\fe1nfly\OneDrive\Documents\IntroductoryPractice\FlightsInformation  *
* File Name: flights.cpp                                                                     *
* Language: C++, Microsoft  Visual Studio Code                                               *
* Programmer: Ривоненко Никита Павлович                                                      *
* Modified by:                                                                               *
* Created: 04.05.2026                                                                        *
* Last Revision: 25.05.2026                                                                  *
* Comment:                                                                                   *
* Тема:«Структуры»                                                                           *
* 1) подготовить программу, сортирующую записи с использованием индексной сортировки методом *
*    «пузырька» в порядке убывания номеров рейсов; определить суммарное количество           *
*    контейнеров; результаты печатать в виде таблицы;                                        *
* 2) обеспечить входной контроль номера рейса, бортового номера, веса груза и количества     *
*    контейнеров, выполнить отладку и тестирование.                                          *
* Чтение данных их файла производить с использованием функций ввода/вывода языка С++.        *
* Алгоритм должен быть параметризован; обмен данными с подпрограммой должен осуществляться   *
* только через параметры; исходные  данные  хранятся в отдельном файле.                      *
*********************************************************************************************/

#include <iostream> //Библиотека для ввода вывода
#include <fstream> //Библиотека для работы с файлами
#include <iomanip> //Библиотека для форматирования вывода
#include <cstdio> //Для функции sscanf
#include <cstring> //Для strlen
#include <cctype> //Библиотека для работы с символами
#include <limits> //Содержит информацию о пределах типов данных,
using namespace std;

const char* FILE_NAME = "data01.txt"; //Имя файла
const int MAX_LINE_LEN = 1000; //Максимальная длина одной строки в файле
const int MAX_TAIL_LEN = 100; //Максимальная длина бортового номера

//Структура для хранения данных
struct FlightInformation
{
    int numberFlight; //Номер рейса
    char numberTail[MAX_TAIL_LEN]; //Бортовой номер
    double weight; //Вес груза
    int countContainers; //Количество контейнеров
}; //FlightInformation

//Функция для выбора варианта обработки данных из файла(игнорировать некорректные строки или нет)
int ChooseOption();
//Вывод сообщений об ошибках
void PrintErrorMessage(
    int code, //Код ошибки
    const char* filename, //Имя файла
    int numberline = -1 //Номер строки
); //PrintErrorMessage

//Чтение одной строки из файла, проверка на наличие ошибок и инициализация переменных структуры
int ReadLine(
    ifstream& file, //Поток чтения из файла
    FlightInformation& flight, //Структура для заполнения данными из строки
    bool printline = false, //Флаг вывода содержимого строки на экран
    int linenum = -1 //Номер строки
); //ReadLine

//Вызов ReadLine для чтения всех строк из файла, заполнение массивов структур и индексов
int ArrayCreate(
    const char* filename, //Имя файла
    FlightInformation*& flights, //Указатель на массив структур
    int*& indexArray, //Указатель на массив индексов
    int& countRightLines, //Количество корректных строк
    int var //Вариант обработки данных из файла
); //ArrayCreate

//Индексная сортировка методом «пузырька» в порядке убывания номеров рейсов
void BubbleSort(
    FlightInformation* flights, //Массив структур
    int* flightsindex, //Массив индексов
    int n //Количество элементов в массиве
); //BubbleSort

//Определение суммарного количества контейнеров
int TotalSumContainers(
    FlightInformation* flights, //Массив структур
    int n //Количество элементов в массиве
); //TotalSumContainers

//Вывод отсортированных данных и суммарного количества контейнеров
void PrintTable(
    FlightInformation* flights, //Массив структур
    int* indexes, //Массив индексов
    int n, //Количество элементов в массиве
    int totalContainers //Суммарное количество контейнеров
); //PrintTable

//main, в котором вызываются функции
int main()
{   
    int option = ChooseOption();

    FlightInformation* flights = nullptr;
    int* flightsindex = nullptr;
    int countRightLines = 0;
    cout << "Чтение данных из файла" << endl;
    int ERROR = ArrayCreate(FILE_NAME, flights, flightsindex, countRightLines, option);
    if (ERROR != 0) {
        if (ERROR==18 || ERROR==19)
        {
            PrintErrorMessage(ERROR, FILE_NAME);
        }
        return 1;
    }

    cout << "Количество корректных строк: " << countRightLines << endl;
    if (countRightLines>0)
    {
        BubbleSort(flights, flightsindex, countRightLines);
        int totalSumContainers = TotalSumContainers(flights, countRightLines);
        PrintTable(flights, flightsindex, countRightLines, totalSumContainers);
    }
    else
    {
        cout << "Данные невозможно обработать" << endl;
    }

    delete[] flights;
    delete[] flightsindex;
    return 0;
} //main

//Функция для выбора варианта обработки данных из файла(игнорировать некорректные строки или нет)
int ChooseOption()
{
    cout << "Здравствуйте! Программа имеет 2 варианта обработки данных:" << endl;
    cout << "1)Игнорирует некорректные строки" << endl;
    cout << "2)Не игнорирует некорректные строки и завершается при нахождении ошибок" << endl << endl;
    int option;
    do
    {
        cout << "Выберете вариант(1 или 2): ";
        if (cin>>option && (option==1 || option==2))
        {
            break;
        }
        cout << "Такого варианта обработки данных нет. Повторите попытку, пожалуйста" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (true);
    cout << endl << "Выбран вариант " << option << endl << endl;
    return option;
}

//Вывод сообщений об ошибках
void PrintErrorMessage(int code, const char* filename, int numberline)
{
    cout << "Ошибка";
    if (numberline != -1 && code != 19)
    {
        cout << " в строке №" << numberline;
    }
    cout << ": ";
    switch (code)
    {
        case 18:
            cout << "файл " << filename << " не найден"<< endl;
            break;
        case 2:
            cout << "строка пустая"<< endl;
            break;
        case 3:
            cout << "строка содержит только пробелы"<< endl;
            break;
        case 4:
            cout << " ошибка при чтении номера рейса"<< endl;
            break;
        case 5:
            cout << "ошибка при чтении бортового номера"<< endl;
            break;
        case 6:
            cout << "ошибка при чтении веса груза"<< endl;
            break;
        case 7:
            cout << "ошибка при чтении количества контейнеров"<< endl;
            break;
        case 8:
            cout << "в строке есть лишние элементы"<< endl;
            break;
        case 9:
            cout << "номер рейса не является натуральным числом"<< endl;
            break;
        case 10:
            cout << "Бортовой номер должен быть формата Б-XXXX"<< endl;
            break;
        case 11:
            cout << "первый символ бортового номера не буква 'Б'"<< endl;
            break;
        case 12:
            cout << "второй символ бортового номера не '-'"<< endl;
            break;
        case 13:
            cout << "У бортового номера после '-' есть элементы, которые не являются цифрой"<< endl;
            break;
        case 14:
            cout << "вес груза отрицательный"<< endl;
            break;
        case 15:
            cout << "вес груза равен нулю при наличии контейнеров"<< endl;
            break;
        case 16:
            cout << "количество контейнеров отрицательно"<< endl;
            break;
        case 17:
            cout << "при отсутствии контейнеров вес не равен нулю"<< endl;
            break;
        case 19:
            cout << "в файле " << filename << " нет корректных строк"<< endl;
            break;
        case 20:
            cout << "строка превышает максимальную длину (" << MAX_LINE_LEN-1 << " символов)" << endl;
            break;
        case 21:
            cout << "такой номер рейса уже встречался ранее" << endl;
            break;
        default:
            cout << endl;
    }
}

//Чтение одной строки из файла, проверка на наличие ошибок и инициализация переменных структуры
int ReadLine(ifstream& file, FlightInformation& flight, bool printline, int linenum)
{
    char line[MAX_LINE_LEN];
    int lenline = 0;
    if (!file.getline(line, MAX_LINE_LEN)) 
    {
        if (file.eof())
        {
            return -1;
        }
        file.clear();
        file.ignore(numeric_limits<streamsize>::max(), '\n');
        return 20;
    }
    if (printline)
    {
        cout << "Строка №" << linenum+1 << ": \"" << line << "\"" << endl;
    }
    for (int i=0; line[i]!='\0'; i++)
    {
        lenline++;
    }
    if (lenline == 0)
    {
        return 2;
    }
    bool space = true;
    for (int i = 0; i < lenline; i++)
    {
        if (!isspace(line[i]))
        {
            space = false;
            break;
        }
    }
    if (space)
    {
        return 3;
    }
    int numflight, countcont, countsymb;
    char numtail[MAX_TAIL_LEN];
    double wght;
    int result=sscanf(line, "%d %s %lf %d %n", &numflight, numtail, &wght, &countcont, &countsymb);
    if (result!=4)
    {
        if (result < 1)
        {
            return 4;
        }
        if (result < 2)
        {
            return 5;
        }
        if (result < 3)
        {
            return 6;
        }
        return 7;
    }
    int i = countsymb;
    while (line[i] == ' ')
    {
        i++;
    }
    if (line[i] != '\0')
    {
        return 8;
    }
    if (numflight<=0)
    {
        return 9;
    }
    if (strlen(numtail)!=7) //Буква Б занимает 2 байта, поэтому длина бортового номера - 7
    {
        return 10;
    }
    if (!(numtail[0] == (char)0xD0 && numtail[1] == (char)0x91)) //Проверка, что первый символ 'Б'
    {
        return 11;
    }
    if (numtail[2]!='-')
    {
        return 12;
    }
    for (int i = 3; i < 7; i++)
    {
        if (!isdigit(numtail[i]))
        {
            return 13;
        }
    }
    if (wght<0.0)
    {
        return 14;
    }
    if (wght==0.0 && countcont>0)
    {
        return 15;
    }
    if (countcont<0)
    {
        return 16;
    }
    if (countcont == 0 && wght > 0.0)
    {
        return 17;
    }
    flight.numberFlight = numflight;
    i = 0;
    while (numtail[i]!='\0' && i<MAX_TAIL_LEN-1)
    {
        flight.numberTail[i] = numtail[i];
        i++;
    }
    flight.numberTail[i]='\0';
    flight.weight = wght;
    flight.countContainers = countcont;
    return 0;
}

//Вызов ReadLine для чтения всех строк из файла, заполнение массивов структур и индексов
int ArrayCreate(const char* filename, FlightInformation*& flights, int*& indexArray, int& countRightLines, int var)
{
    ifstream file(filename);
    if (!file)
    {
        file.close();
        return 18;
    }
    FlightInformation* tempFlights = nullptr;
    int tempCount = 0;
    int codeError = 0;
    FlightInformation structura;
    int numberLine = 0;
    while (true)
    {
        codeError = ReadLine(file, structura, true, numberLine);
        if (codeError == -1)
            break;
        numberLine++;
        
        if (codeError == 0)
        {
            bool duplicate = false;
            for (int i = 0; i < tempCount; i++)
            {
                if (tempFlights[i].numberFlight == structura.numberFlight)
                {
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate)
            {
                if (var != 2)
                {
                    cout << "В строке №" << numberLine << " ошибок не обнаружено" << endl;
                }
                FlightInformation* newTemp = new FlightInformation[tempCount + 1];
                for (int i = 0; i < tempCount; i++)
                {
                    newTemp[i] = tempFlights[i];
                }
                newTemp[tempCount] = structura;
                delete[] tempFlights;
                tempFlights = newTemp;
                tempCount++;
            }
            else
            {
                PrintErrorMessage(21, filename, numberLine);
                if (var == 2)
                {
                    file.close();
                    delete[] tempFlights;
                    return 21;
                }
            }
        }
        else
        {
            PrintErrorMessage(codeError, filename, numberLine);
            if (var == 2)
            {
                file.close();
                delete[] tempFlights;
                return codeError;
            }
        }
    }
    file.close();
    
    if (tempCount == 0)
    {
        flights = nullptr;
        indexArray = nullptr;
        countRightLines = 0;
        return 19;
    }
    flights = new FlightInformation[tempCount];
    indexArray = new int[tempCount];
    for (int i = 0; i < tempCount; i++)
    {
        flights[i] = tempFlights[i];
        indexArray[i] = i;
    }
    countRightLines = tempCount;
    delete[] tempFlights;
    return 0;
}

//Индексная сортировка методом «пузырька» в порядке убывания номеров рейсов
void BubbleSort(FlightInformation* flights, int* flightsindex, int n)
{
    for (int i=0; i<n-1; i++)
    {
        bool swap = false;
        for (int j=0; j<n-i-1; j++)
        {
            if (flights[flightsindex[j]].numberFlight<flights[flightsindex[j+1]].numberFlight)
            {
                int temp = flightsindex[j];
                flightsindex[j]=flightsindex[j+1];
                flightsindex[j+1]=temp;
                swap=true;
            }
        }
        if (!swap)
        {
            break;
        }
    }
}

//Определение суммарного количества контейнеров
int TotalSumContainers(FlightInformation* flights, int n)
{
    int sum = 0;
    for (int i=0; i<n; i++)
    {
        sum+=flights[i].countContainers;
    }
    return sum;
}

//Вывод отсортированных данных и суммарного количества контейнеров
void PrintTable(FlightInformation* flights, int* indexes, int n, int totalContainers)
{
    cout << endl << "================================= Отсортированная таблица ==================================" << endl;
    cout << "+------------------+------------------------+-------------------+--------------------------+" << endl;
    cout << "| "<< left << setw(25) << "Номер рейса"
         << "  | " << setw(35) << "Бортовой номер"
         << " | " << setw(25) << "Вес груза"
         << " | " << setw(40) << " Кол-во контейнеров" << " |" << endl;
    cout << "+------------------+------------------------+-------------------+--------------------------+" << endl;
    for (int i = 0; i < n; ++i) {
        FlightInformation& f = flights[indexes[i]];
        cout << "| " << left << setw(16) << f.numberFlight
             << " | " << setw(22) << f.numberTail
             << "  | " << setw(17) << fixed << setprecision(3) << f.weight
             << " | " << setw(24) << f.countContainers << " |" << endl;
        cout << "+------------------+------------------------+-------------------+--------------------------+" << endl;
    }
    cout << "============================================================================================" << endl;
    cout << "Суммарное количество контейнеров: " << totalContainers << endl;
    cout << "============================================================================================" << endl;
}