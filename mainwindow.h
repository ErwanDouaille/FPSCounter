#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QString>
#include <QList>

#include <iostream>
#include <chrono>
#include <thread>

#include "benchmark.h"
#include "datacomputer.h"
#include "datacomputeraverage.h"
#include "datacomputermedian.h"
#include "hmlparser.h"

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

    HMLParser * getParser();


public slots:
    void displayBench();
    void resetView();
    void updateView();
    void chooseDataToParse();
    void setCurrentDataComputer(int id);

signals:
    void updateBenchmarkView();

private:
    Ui::MainWindow *ui;
    DataComputer * m_currentDataComputer;
    QList<DataComputer*> m_dataComputerList;
    HMLParser * m_parser;

};

#endif // MAINWINDOW_H
