#include "datacomputermedian.h"

DataComputerMedian::DataComputerMedian(QObject *parent) : DataComputer(parent)
{

}

double DataComputerMedian::compute(Benchmark* bench, int i)
{
    QList<double>* list = bench->getColumValue().at(i);
    qSort(*list);
    int id = list->size()/2 + list->size()%2;
    return list->at(id);
}
