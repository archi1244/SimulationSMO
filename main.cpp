#include <iostream>
#include <iomanip>
#include "Simulator.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");

    cout << "========================================" << endl;
    cout << "  ИМИТАЦИОННОЕ МОДЕЛИРОВАНИЕ СМО" << endl;
    cout << "  Одноканальная система с отказами" << endl;
    cout << "========================================" << endl;
    cout << endl;

    // Параметры моделирования
    double lambda;      // интенсивность входного потока (заявок/ед. времени)
    double mu;          // интенсивность обслуживания (заявок/ед. времени)
    double timeMax;     // время моделирования
    int numExperiments; // количество экспериментов

    cout << "Введите интенсивность входного потока lambda (заявок/ед.времени): ";
    cin >> lambda;
    cout << "Введите интенсивность обслуживания mu (заявок/ед.времени): ";
    cin >> mu;
    cout << "Введите время моделирования: ";
    cin >> timeMax;
    cout << "Введите количество экспериментов: ";
    cin >> numExperiments;

    // Проверка корректности
    if (lambda <= 0 || mu <= 0 || timeMax <= 0 || numExperiments <= 0) {
        cerr << "Ошибка: все параметры должны быть положительными!" << endl;
        return 1;
    }

    // Создаем и запускаем симулятор
    Simulator sim(lambda, mu, timeMax);
    sim.runMultipleExperiments(numExperiments);

    // Вывод итоговых результатов
    cout << "\n========================================" << endl;
    cout << "ИТОГОВЫЕ РЕЗУЛЬТАТЫ" << endl;
    cout << "========================================" << endl;
    cout << fixed << setprecision(4);
    cout << "Средняя вероятность отказа (Pотк) = " << sim.getAvgRejectionProb() << endl;
    cout << "Средняя относительная пропускная способность (Q) = " << sim.getAvgThroughput() << endl;
    cout << "Среднее количество обслуженных заявок = " << sim.getAvgServed() << endl;
    cout << "Среднее количество потерянных заявок = " << sim.getAvgLost() << endl;
    cout << "Среднее общее количество заявок = " << sim.getAvgTotal() << endl;
    cout << "\nТеоретическое значение Pотк = " << lambda / (lambda + mu) << " (формула Эрланга для СМО с отказами)" << endl;
    cout << "========================================" << endl;

    return 0;
}