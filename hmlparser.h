#ifndef HMLPARSER_H
#define HMLPARSER_H

#include <QObject>
#include <QMessageBox>
#include <QTextStream>
#include <QList>
#include <QString>

#include "benchmark.h"

class HMLParser : public QObject
{
    Q_OBJECT
public:
    explicit HMLParser(QObject *parent = nullptr);

    void parseData(QString fileName);
    void parseLine(QString line);
    QList<Benchmark*> getBenchmarkList();

private:
    Benchmark * m_currentBenchmark;
    QList<Benchmark*> m_benchmarkList;

signals:
    void parseChanged();
    void resetAll();

public slots:
    void resetData();
};

#endif // HMLPARSER_H
