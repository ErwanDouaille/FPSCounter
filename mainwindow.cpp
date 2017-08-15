#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    initView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initView()
{
    ui->setupUi(this);
    resetView();
    connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(displayBench(int)));
    connect(ui->refreshButton, SIGNAL(clicked(bool)), this, SLOT(chooseDataToParse()));
    connect(this, SIGNAL(parseChanged()), this, SLOT(updateView()));
}

void MainWindow::displayBench(int id)
{
    QString result ;
    Benchmark* bench = m_benchmarkList.at(id);
    result += "Version : " + bench->getMonitoringVersion() + "\n";
    result += "Date    : " + bench->getDate() + "\n";
    result += "Gpu     : " + bench->getGPU() + "\n";
    result += "Nb data : " + QString::number(bench->getNbLine()) + "\n";
    for (int i = 0; i < bench->getColumnSize(); i++)
    {

        double sum = 0.0;
        QList<double>* list = bench->getColumValue().at(i);
//        std::for_each(list.rbegin(), list.rend(), [&](double n) { std::cout << n << std::endl;sum += n; });
        sum = std::accumulate(list->begin(), list->end(), 0.0);
        double value = (double)sum/(double)bench->getNbLine();
        result += bench->getColumName().at(i) + " : "
                  + QString::number(value) + " "
                  + bench->getColumUnit().at(i)
                  +  "\n";
    }
    ui->textBrowser->clear();
    ui->textBrowser->setText(result);

}

void MainWindow::resetView()
{
    ui->comboBox->clear();
    ui->textBrowser->clear();
}

void MainWindow::chooseDataToParse()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open RivaTuner hardware monitoring log file"), "C:\Users\edoudaille\Desktop", tr("Bench Data (*.hml)"));
    parseData(fileName);
    emit parseChanged();
}

void MainWindow::updateView()
{
    if(m_benchmarkList.isEmpty())
        return;
    for(Benchmark* bench : m_benchmarkList)
        ui->comboBox->addItem(bench->getDate() + bench->getGPU());
    ui->comboBox->setCurrentIndex(0);
}

void MainWindow::parseData(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(0, "Error", file.errorString());
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        parseLine(line);
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
        m_currentBenchmark->setDate(list.at(1));
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
