#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    initComputer();
    initView();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initComputer()
{
    m_dataComputerList.push_back(new DataComputerAverage(this));
    m_dataComputerList.push_back(new DataComputerMedian(this));
}

void MainWindow::initView()
{
    ui->setupUi(this);
    resetView();

    connect(ui->comboBoxDataComputer, SIGNAL(activated(int)), this, SLOT(setCurrentDataComputer(int)));
    connect(ui->comboBoxBenchmark, SIGNAL(activated(int)), this, SLOT(displayBench()));
    connect(ui->refreshButton, SIGNAL(clicked(bool)), this, SLOT(resetData()));
    connect(ui->refreshButton, SIGNAL(clicked(bool)), this, SLOT(resetView()));
    connect(ui->refreshButton, SIGNAL(clicked(bool)), this, SLOT(chooseDataToParse()));
    connect(this, SIGNAL(parseChanged()), this, SLOT(updateView()));
    connect(this, SIGNAL(resetAll()), this, SLOT(resetData()));
    connect(this, SIGNAL(resetAll()), this, SLOT(resetView()));


    if(m_dataComputerList.isEmpty())
        return;
    for(DataComputer* dataComputer : m_dataComputerList)
    {
        ui->comboBoxDataComputer->addItem(dataComputer->className());
    }
    ui->comboBoxDataComputer->activated(0);
}

void MainWindow::displayBench()
{
    int id = ui->comboBoxBenchmark->currentIndex();
    if (id < 0)
        return;
    QString result ;
    Benchmark* bench = m_benchmarkList.at(id);
    result += "Version : " + bench->getMonitoringVersion() + "\n";
    result += "Date    : " + bench->getDate() + "\n";
    result += "Gpu     : " + bench->getGPU() + "\n";
    result += "Nb data : " + QString::number(bench->getNbLine()) + "\n";
    for (int i = 0; i < bench->getColumnSize(); i++)
    {
        double value = m_currentDataComputer->compute(m_currentBenchmark, i);
        result += bench->getColumName().at(i) + " : "
                  + QString::number(value) + " "
                  + bench->getColumUnit().at(i)
                  +  "\n";
    }
    ui->textBrowser->clear();
    ui->textBrowser->setText(result);

}

void MainWindow::resetData()
{
    m_currentBenchmark = NULL;
    m_benchmarkList.clear();
}

void MainWindow::resetView()
{
    ui->comboBoxBenchmark->clear();
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
        ui->comboBoxBenchmark->addItem(bench->getDate() + bench->getGPU());
    ui->comboBoxBenchmark->activated(0);
}

void MainWindow::parseData(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(0, "Error", file.errorString());
        return;
    }

    QTextStream in(&file);
    bool error = false;
    while (!in.atEnd() && !error)
    {
        QString line = in.readLine();
        try
        {
            parseLine(line);
        } catch (std::exception exception)
        {
            QMessageBox::warning(0, "Error", exception.what());
            error = true;
            emit resetAll();
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
        m_currentBenchmark->setDate(list.at(1));
    }

    if (lineID.compare("01") == 0)
    {
        if(!m_currentBenchmark)
             throw std::exception("Error while parsing hlm file, 00 data instruction is missing");
        m_currentBenchmark->setGPU(list.at(2));
    }

    if (lineID.compare("02") == 0)
    {
        if(!m_currentBenchmark)
            throw std::exception("Error while parsing hlm file, 00 data instruction is missing");
        m_currentBenchmark->setColumnSize(list.size() - 2);
        m_currentBenchmark->resetColumnValue();
        for(int i = 2; i < list.size(); i++)
            m_currentBenchmark->addColumnName(list.at(i));
    }

    if (lineID.compare("03") == 0)
    {
        if(!m_currentBenchmark)
            throw std::exception("Error while parsing hlm file, 00 data instruction is missing");
        m_currentBenchmark->addColumnUnit(list.at(3));
    }

    if (lineID.compare("80") == 0)
    {
        if(!m_currentBenchmark)
            return;
        //At this point we ensure than every list has the same size
        if (m_currentBenchmark->getColumName().size() != m_currentBenchmark->getColumnSize())
            throw std::exception("Error while parsing hlm file, 02 data instruction is missing");
        if (m_currentBenchmark->getColumUnit().size() != m_currentBenchmark->getColumnSize())
            throw std::exception("Error while parsing hlm file, 03 data instruction is missing");

        for(int i = 2; i < list.size(); i++)
        {
            QString qStringValue = list.at(i);
            double value = qStringValue.toDouble();
            m_currentBenchmark->addColumnValue(i-2, value);
        }
        m_currentBenchmark->incrementNbLine();
    }



}

void MainWindow::setCurrentDataComputer(int id)
{
    if (id > m_dataComputerList.size())
        return;
    m_currentDataComputer = m_dataComputerList.at(id);

    emit displayBench();
}
