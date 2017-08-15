#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <QObject>
#include <QString>
#include <QList>

#include <iostream>

class Benchmark
{
public:
    Benchmark();

    QString getMonitoringVersion();
    QString getGPU();
    int getColumnSize();
    int getNbLine();
    QList<QString> getColumName();
    QList<QString> getColumUnit();
    QList<double> getColumValue();
    double getColumValue(int i);
    QString getColumUnit(int i);
    QString getColumName(int i);

    void incrementNbLine();
    void resetColumnValue();
    void setMonitoringVersion(QString name);
    void setGPU(QString name);
    void setColumnSize(int size);
    void addColumnName(QString name);
    void addColumnUnit(QString name);
    void addColumnValue(int i, double value);

protected:
    QString m_monitoringVersion;
    QString m_gpu;
    int m_columnSize;
    int m_nbLine;
    QList<QString> m_columnName;
    QList<QString> m_columnUnit;
    QList<double> m_columnValue;
};

#endif // BENCHMARK_H
