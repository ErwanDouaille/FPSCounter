#ifndef DATACOMPUTER_H
#define DATACOMPUTER_H

#include <QObject>
#include "benchmark.h"

class DataComputer : public QObject
{
    Q_OBJECT
public:
    explicit DataComputer(QObject *parent = nullptr);

    virtual double compute(Benchmark* bench, int i) =0;
    virtual QString className() {  return QString(metaObject()->className()); }


signals:
    void updateView();

public slots:
};

#endif // DATACOMPUTER_H
