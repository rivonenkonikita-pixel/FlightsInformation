#include <iostream> //Библиотека для ввода вывода
#include <fstream> //Библиотека для работы с файлами
#include <iomanip>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <limits>
using namespace std;

const int MAX_LINE_LEN = 1000; //Максимальная длина одной строки в файле
const int MAX_TAIL_LEN = 100; //Максимальная длина бортового номера

struct FlightInformation
{
    int numberFlight; //Номер рейса
    char numberTail[MAX_TAIL_LEN]; //Бортовой номер
    double weight; //Вес груза
    int countContainers; //Количество контейнеров
};

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

void PrintErrorMessage(int code, const char* filename, int numberline = -1)
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
            cout << "бортовой номер слишком короткий"<< endl;
            break;
        case 11:
            cout << "первый символ бортового номера не является заглавной русской буквой"<< endl;
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
        default:
            cout << endl;
    }
}

int ReadLine(ifstream& file, FlightInformation& flight, bool printline = false, int linenum=-1)
{
    char line[MAX_LINE_LEN];
    int lenline = 0;
     if (!file.getline(line, MAX_LINE_LEN)) {
        if (file.eof()) return -1;
        file.clear();
        file.ignore(numeric_limits<streamsize>::max(), '\n');
        return 20;
    }
    if (printline)
    {
        cout << "Строка №" << linenum+1 << ": \"" << line << "\"" << endl;
    }
    for (int i = 0; i < MAX_LINE_LEN; i++)
    {
        if (line[i]=='.')
        {
            line[i]=',';
        }
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
    int result=sscanf(line, "%d %99s %lf %d %n", &numflight, numtail, &wght, &countcont, &countsymb);
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
    if (strlen(numtail)<3)
    {
        return 10;
    }
    if (!((numtail[0]>='А' && numtail[0]<='Я') || numtail[0]=='Ё'))
    {
        return 11;
    }
    if (numtail[1]!='-')
    {
        return 12;
    }
    for (int i = 2; i < strlen(numtail); i++)
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

int ArrayCreate(const char* filename, FlightInformation*& flights, int*& indexArray, int& countRightLines, int var)
{
    ifstream file(filename);
    if (!file)
    {
        file.close();
        return 18;
    }
    int countLine = 0;
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
            if (var!=2)
            {
                cout << "В строке №" << numberLine << " ошибок не обнаружено" << endl;
            }
            countLine++;
        }
        else
        {
            PrintErrorMessage(codeError, filename, numberLine);
            if (var==2)
            {
                file.close();
                return codeError;
            }
        }
    }
    file.close();
    if (countLine == 0)
    {
        flights = nullptr;
        indexArray = nullptr;
        countRightLines = countLine;
        return 19;
    }
    flights=new FlightInformation[countLine];
    indexArray=new int[countLine];
    ifstream file2(filename);
    if (!file2)
    {
        delete[] flights;
        delete[] indexArray;
        return 18;
    }
    int index = 0;
    numberLine = 0;
    while (index<countLine)
    {
        codeError = ReadLine(file2, structura);
        if (codeError==-1)
        {
            break;
        }
        numberLine++;
        if (codeError==0)
        {
            flights[index]=structura;
            indexArray[index]=index;
            index++;
        }
    }
    file2.close();
    countRightLines=countLine;
    return 0;
}

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

int TotalSumContainers(FlightInformation* flights, int n)
{
    int sum = 0;
    for (int i=0; i<n; i++)
    {
        sum+=flights[i].countContainers;
    }
    return sum;
}

void PrintTable(FlightInformation* flights, int* indexes, int n, int totalContainers)
{
    cout << endl << "================================= Отсортированная таблица ==================================" << endl;
    cout << "+------------------+------------------------+-------------------+--------------------------+" << endl;
    cout << "| "<< left << setw(15) << "Номер рейса"
         << "  | " << setw(22) << "Бортовой номер"
         << " | " << setw(17) << "Вес груза"
         << " | " << setw(24) << " Кол-во контейнеров" << " |" << endl;
    cout << "+------------------+------------------------+-------------------+--------------------------+" << endl;
    for (int i = 0; i < n; ++i) {
        FlightInformation& f = flights[indexes[i]];
        cout << "| " << left << setw(16) << f.numberFlight
             << " | " << setw(21) << f.numberTail
             << "  | " << setw(17) << fixed << setprecision(3) << f.weight
             << " | " << setw(24) << f.countContainers << " |" << endl;
        cout << "+------------------+------------------------+-------------------+--------------------------+" << endl;
    }
    cout << "============================================================================================" << endl;
    cout << "Суммарное количество контейнеров: " << totalContainers << endl;
    cout << "============================================================================================" << endl;
}

int main()
{   
    setlocale(LC_ALL, "Russian");
    int option = ChooseOption();

    const char* filename = "data.txt";
    FlightInformation* flights = nullptr;
    int* flightsindex = nullptr;
    int countRightLines = 0;
    cout << "Чтение данных из файла" << endl;
    int ERROR = ArrayCreate(filename, flights, flightsindex, countRightLines, option);
    if (ERROR != 0) {
        if (ERROR==18 || ERROR==19)
        {
            PrintErrorMessage(ERROR, filename);
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
}