#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QString>
#include <QList>

#include <iostream>
#include <chrono>
#include <thread>

#include "benchmark.h"
#include "datacomputer.h"
#include "datacomputeraverage.h"
#include "datacomputermedian.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initView();
    void initComputer();


public slots:
    void displayBench();
    void resetView();
    void resetData();
    void updateView();
    void chooseDataToParse();
    void setCurrentDataComputer(int id);


signals:
    void parseChanged();
    void resetAll();
    void updateBenchmarkView();

private:
    Ui::MainWindow *ui;

    void parseData(QString fileName);
    void parseLine(QString line);

    DataComputer * m_currentDataComputer;
    QList<DataComputer*> m_dataComputerList;
    Benchmark * m_currentBenchmark;
    QList<Benchmark*> m_benchmarkList;


};

#endif // MAINWINDOW_H
