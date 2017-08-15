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

public slots:
    void displayBench(int id);
    void resetView();
    void updateView();
    void chooseDataToParse();

signals:
    void parseChanged();

private:
    Ui::MainWindow *ui;

    void parseData(QString fileName);
    void parseLine(QString line);

    Benchmark * m_currentBenchmark;
    QList<Benchmark*> m_benchmarkList;


};

#endif // MAINWINDOW_H
