#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <vector>

using namespace std;

/**
 * Класс, реализующий имитационное моделирование
 * одноканальной системы массового обслуживания (СМО) с отказами.
 */
class Simulator
{
private:
    double lambda;              // интенсивность входного потока
    double mu;                  // интенсивность обслуживания
    double timeMax;             // максимальное время моделирования

    // Результаты одного эксперимента
    double totalRequests;       // общее количество заявок
    double servedRequests;      // количество обслуженных заявок
    double lostRequests;        // количество потерянных заявок
    double rejectionProb;       // вероятность отказа Pотк

    // Накопленные результаты по всем экспериментам
    vector<double> rejectionProbs;
    vector<double> throughputs;
    vector<double> servedList;
    vector<double> lostList;
    vector<double> totalList;

    // Генерация экспоненциально распределенной случайной величины
    double generateExponential(double intensity);

    // Запуск одного эксперимента (событийное моделирование)
    void runSingleExperiment();

public:
    // Конструктор
    Simulator(double lambda, double mu, double timeMax);

    // Запуск серии экспериментов
    void runMultipleExperiments(int numExperiments);

    // Геттеры для итоговых средних значений
    double getAvgRejectionProb() const;
    double getAvgThroughput() const;
    double getAvgServed() const;
    double getAvgLost() const;
    double getAvgTotal() const;
};

#endif // SIMULATOR_H