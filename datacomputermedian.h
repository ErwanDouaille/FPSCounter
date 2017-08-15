#ifndef DATACOMPUTERMEDIAN_H
#define DATACOMPUTERMEDIAN_H

#include "datacomputer.h"
#include <math.h>

class DataComputerMedian : public DataComputer
{
    Q_OBJECT
public:
    DataComputerMedian(QObject *parent = nullptr);

    virtual double compute(Benchmark* bench, int i);
};

#endif // DATACOMPUTERMEDIAN_H
