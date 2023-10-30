#include "pch.h"
#define MIN_ARRAY_SIZE 2
#define MAX_ARRAY_SIZE 10
#define MIN_ARRAY_VALUE 10
#define MAX_ARRAY_VALUE 100


struct ThreadData {
    int* result;
    int id;
    std::vector<int> data;
};

//доп. метод подсчета НОД
int GCD(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

//основной метод подсчета НОД
DWORD WINAPI FindGCD(LPVOID lpParam)  {

    ThreadData* threadData = static_cast<ThreadData*>(lpParam);

    int id = threadData->id;
    std::vector<int> data = threadData->data;

    if (data.empty()) {
        return 0; 
    }

    int gcd = data[0];

    for (size_t i = 1; i < data.size(); ++i) {
        gcd = GCD(gcd, data[i]);
    }

    *(threadData->result) = gcd;
    return 0;
}


int main(int argc, char* argv[])
{
    //сумма потоков, устанавливаемая аргументом командной строки
    std::string arg = argv[1];
    int numberOfThreads = atoi(argv[1]);

    //инициализация указателей на потоки и место сохранения результатов
    srand(time(NULL));
    int* results = new int[numberOfThreads];
    HANDLE* threads = new HANDLE[numberOfThreads];

    CRITICAL_SECTION resultLock; // Создаем мьютекс
    InitializeCriticalSection(&resultLock);

    for (size_t i = 0; i < numberOfThreads; i++)
    {
        //создание случайного массива
        int size = MIN_ARRAY_SIZE + std::rand() % (MAX_ARRAY_SIZE - MIN_ARRAY_SIZE + 1);
        printf_s("%d: \tArray size: %d \t{", i, size);
        std::vector<int> numbers;
        for (size_t t = 0; t < size; t++)
        {
            numbers.push_back(MIN_ARRAY_VALUE + std::rand() % (MAX_ARRAY_VALUE - MIN_ARRAY_VALUE + 1));
            printf_s("%d ", numbers.back());
        }
        printf_s("}\r\n");
        //создание потока с аргументом в виде структуры.
        ThreadData* threadData = new ThreadData();
        threadData->id = i;
        threadData->result = &results[i];
        threadData->data = numbers;

        threads[i] = CreateThread(NULL, 0, FindGCD, threadData, 0, NULL);
    }

    //синхронизация
    WaitForMultipleObjects(numberOfThreads, threads, TRUE, INFINITE);
    
    //очистка памяти
    for (size_t i = 0; i < numberOfThreads; i++) {
        CloseHandle(threads[i]);
    }
    delete[] threads;

    int sum = 0;
    //подсчет
    for (size_t i = 0; i < numberOfThreads; i++) {
        sum += *(results+i);
        printf_s("%d: %d\n\r", i, *(results + i));
    }


    printf_s("Sum of numbers %d", sum);

    return 0;
}