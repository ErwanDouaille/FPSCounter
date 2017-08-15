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
    QList<QList<double>*> getColumValue();
    QList<double>* getColumValue(int i);
    QString getColumUnit(int i);
    QString getColumName(int i);
    QString getDate();

    void incrementNbLine();
    void resetColumnValue();
    void setMonitoringVersion(QString name);
    void setGPU(QString name);
    void setDate(QString date);
    void setColumnSize(int size);
    void addColumnName(QString name);
    void addColumnUnit(QString name);
    void addColumnValue(int i, double value);

protected:
    QString m_monitoringVersion;
    QString m_gpu;
    QString m_date;
    int m_columnSize;
    int m_nbLine;
    QList<QString> m_columnName;
    QList<QString> m_columnUnit;
    QList<QList<double>*> m_columnValue;
};

#endif // BENCHMARK_H
