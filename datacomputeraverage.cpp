#include "datacomputeraverage.h"

DataComputerAverage::DataComputerAverage(QObject *parent) : DataComputer(parent)
{

}

double DataComputerAverage::compute(Benchmark* bench, int i)
{
    QList<double>* list = bench->getColumValue().at(i);
    double sum = std::accumulate(list->begin(), list->end(), 0.0);
    return (double)sum/(double)bench->getNbLine();
}
