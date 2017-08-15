#ifndef DATACOMPUTERAVERAGE_H
#define DATACOMPUTERAVERAGE_H

#include "datacomputer.h"

class DataComputerAverage : public DataComputer
{
    Q_OBJECT
public:
    DataComputerAverage(QObject *parent = nullptr);

    virtual void setup();
    virtual double compute(Benchmark* bench, int i);
};

#endif // DATACOMPUTERAVERAGE_H
