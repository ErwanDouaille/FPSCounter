#include "benchmark.h"

Benchmark::Benchmark()
{
    m_nbLine = 0;
    m_columnSize = 0;
}

QString Benchmark::getMonitoringVersion()
{
    return m_monitoringVersion;
}

QString Benchmark::getGPU()
{
    return m_gpu;
}

int Benchmark::getColumnSize()
{
    return m_columnSize;
}

int Benchmark::getNbLine()
{
    return m_nbLine;
}

void Benchmark::incrementNbLine()
{
    m_nbLine++;
}

QList<QString> Benchmark::getColumName()
{
    return m_columnName;
}

QList<QString> Benchmark::getColumUnit()
{
    return m_columnUnit;
}

QList<double> Benchmark::getColumValue()
{
    return m_columnValue;
}

double Benchmark::getColumValue(int i)
{
    if (i > m_columnValue.size())
        return 0.0;
    return m_columnValue.at(i);
}

QString Benchmark::getColumUnit(int i)
{
    if (i > m_columnUnit.size())
        return "Error";
    return m_columnUnit.at(i);
}

QString Benchmark::getColumName(int i)
{
    if (i > m_columnName.size())
        return "Error";
    return m_columnName.at(i);
}

void Benchmark::addColumnName(QString name)
{
    m_columnName.push_back(name);
}

void Benchmark::addColumnUnit(QString name)
{
    m_columnUnit.push_back(name);
}

void Benchmark::addColumnValue(int i, double value)
{
    double intPastValue = m_columnValue.at(i);
    m_columnValue.replace(i, intPastValue + value);
}

void Benchmark::resetColumnValue()
{
    m_columnValue.clear();
    for(int i = 0; i < m_columnSize; i++)
        m_columnValue.push_back(0);
}

void Benchmark::setMonitoringVersion(QString name)
{
    m_monitoringVersion = name;
}

void Benchmark::setGPU(QString name)
{
    m_gpu = name;
}

void Benchmark::setColumnSize(int size)
{
    m_columnSize = size;
}
