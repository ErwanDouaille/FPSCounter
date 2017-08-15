#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    parseData();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::parseData()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open RivaTuner hardware monitoring log file"), "C:\Users\edoudaille\Desktop", tr("Bench Data (*.hml)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(0, "Error", file.errorString());
        return;
    }

    std::cout << fileName.toStdString() << std::endl;
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        parseLine(line);
    }

    for(Benchmark* bench : m_benchmarkList)
    {
        std::cout << "Version : " << bench->getMonitoringVersion().toStdString() << std::endl;
        std::cout << "Gpu     : " << bench->getGPU().toStdString() << std::endl;
        std::cout << "Nb data : " << bench->getNbLine() << std::endl;
        for (int i = 0; i < bench->getColumnSize(); i++)
        {
            std::cout << bench->getColumName().at(i).toStdString() << " : "
               << bench->getColumValue().at(i)/bench->getNbLine() << " "
               << bench->getColumUnit().at(i).toStdString()
               << std::endl;
        }
    }


    file.close();
}

void MainWindow::parseLine(QString line)
{
    QStringList list = line.split(",");
    QString lineID = list.at(0);

    if (lineID.compare("00") == 0)
    {
        m_currentBenchmark = new Benchmark();
        m_benchmarkList.push_back(m_currentBenchmark);
        m_currentBenchmark->setMonitoringVersion(list.at(2));
    }

    if (lineID.compare("01") == 0)
    {
        if(!m_currentBenchmark)
            return;
        m_currentBenchmark->setGPU(list.at(2));
    }

    if (lineID.compare("02") == 0)
    {
        if(!m_currentBenchmark)
            return;
        m_currentBenchmark->setColumnSize(list.size() - 2);
        m_currentBenchmark->resetColumnValue();
        for(int i = 2; i < list.size(); i++)
            m_currentBenchmark->addColumnName(list.at(i));
    }

    if (lineID.compare("03") == 0)
    {
        if(!m_currentBenchmark)
            return;
        m_currentBenchmark->addColumnUnit(list.at(3));
    }

    if (lineID.compare("80") == 0)
    {
        if(!m_currentBenchmark)
            return;
        for(int i = 2; i < list.size(); i++)
        {
            QString qStringValue = list.at(i);
            double value = qStringValue.toDouble();
            m_currentBenchmark->addColumnValue(i-2, value);
        }
        m_currentBenchmark->incrementNbLine();
    }



}
