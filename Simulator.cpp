#include "Simulator.h"
#include <random>
#include <cmath>
#include <iostream>

using namespace std;

// Глобальный генератор случайных чисел (один на всю программу)
static random_device rd;
static mt19937 gen(rd());

double Simulator::generateExponential(double intensity)
{
    if (intensity <= 0) return 1e9; // если интенсивность 0 - бесконечное время
    uniform_real_distribution<> dis(0.0, 1.0);
    double u = dis(gen);
    // Инверсионный метод для экспоненциального распределения
    return -log(1.0 - u) / intensity;
}

Simulator::Simulator(double lambda, double mu, double timeMax)
    : lambda(lambda), mu(mu), timeMax(timeMax)
{
    // Инициализация
    totalRequests = 0;
    servedRequests = 0;
    lostRequests = 0;
    rejectionProb = 0.0;
}

void Simulator::runSingleExperiment()
{
    // СБРОС счетчиков для нового эксперимента
    totalRequests = 0;
    servedRequests = 0;
    lostRequests = 0;

    double currentTime = 0.0;           // текущее модельное время
    double timeNextArrival = generateExponential(lambda);   // время следующей заявки
    double timeNextDeparture = 1e9;      // время завершения обслуживания (бесконечность, канал свободен)
    bool isChannelBusy = false;          // занят ли канал

    // Событийное моделирование
    while (currentTime < timeMax)
    {
        // Определяем ближайшее событие
        double nextEventTime = min(timeNextArrival, timeNextDeparture);

        // Если следующее событие выходит за timeMax, завершаем цикл
        if (nextEventTime > timeMax)
            break;

        currentTime = nextEventTime;

        // Обработка события: поступление заявки
        if (currentTime >= timeNextArrival - 1e-9 && timeNextArrival <= timeNextDeparture + 1e-9)
        {
            totalRequests++;    // поступила новая заявка

            if (!isChannelBusy)
            {
                // Канал свободен - начинаем обслуживание
                isChannelBusy = true;
                timeNextDeparture = currentTime + generateExponential(mu);
                servedRequests++;
            }
            else
            {
                // Канал занят - заявка получает отказ
                lostRequests++;
            }

            // Генерируем время следующего поступления
            timeNextArrival = currentTime + generateExponential(lambda);
        }

        // Обработка события: завершение обслуживания
        if (currentTime >= timeNextDeparture - 1e-9 && timeNextDeparture <= timeNextArrival + 1e-9)
        {
            isChannelBusy = false;
            timeNextDeparture = 1e9; // канал освободился
        }
    }

    // Вычисляем вероятность отказа для этого эксперимента
    if (totalRequests > 0)
        rejectionProb = lostRequests / totalRequests;
    else
        rejectionProb = 0.0;

    // Сохраняем результаты
    rejectionProbs.push_back(rejectionProb);
    throughputs.push_back(1.0 - rejectionProb);
    servedList.push_back(servedRequests);
    lostList.push_back(lostRequests);
    totalList.push_back(totalRequests);
}

void Simulator::runMultipleExperiments(int numExperiments)
{
    // Очистка предыдущих результатов
    rejectionProbs.clear();
    throughputs.clear();
    servedList.clear();
    lostList.clear();
    totalList.clear();

    cout << "\nЗапуск " << numExperiments << " экспериментов..." << endl;
    cout << "----------------------------------------" << endl;

    for (int i = 0; i < numExperiments; i++)
    {
        runSingleExperiment();
        cout << "Эксперимент " << i + 1
            << ": Pотк = " << rejectionProb
            << ", Обслужено = " << servedRequests
            << ", Потеряно = " << lostRequests
            << ", Всего = " << totalRequests << endl;
    }
    cout << "----------------------------------------" << endl;
}

double Simulator::getAvgRejectionProb() const
{
    if (rejectionProbs.empty()) return 0.0;
    double sum = 0.0;
    for (double val : rejectionProbs)
        sum += val;
    return sum / rejectionProbs.size();
}

double Simulator::getAvgThroughput() const
{
    if (throughputs.empty()) return 0.0;
    double sum = 0.0;
    for (double val : throughputs)
        sum += val;
    return sum / throughputs.size();
}

double Simulator::getAvgServed() const
{
    if (servedList.empty()) return 0.0;
    double sum = 0.0;
    for (double val : servedList)
        sum += val;
    return sum / servedList.size();
}

double Simulator::getAvgLost() const
{
    if (lostList.empty()) return 0.0;
    double sum = 0.0;
    for (double val : lostList)
        sum += val;
    return sum / lostList.size();
}

double Simulator::getAvgTotal() const
{
    if (totalList.empty()) return 0.0;
    double sum = 0.0;
    for (double val : totalList)
        sum += val;
    return sum / totalList.size();
}