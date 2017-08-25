#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_parser = new HMLParser();
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
    connect(ui->refreshButton, SIGNAL(clicked(bool)), m_parser, SLOT(resetData()));
    connect(ui->refreshButton, SIGNAL(clicked(bool)), this, SLOT(resetView()));
    connect(ui->refreshButton, SIGNAL(clicked(bool)), this, SLOT(chooseDataToParse()));
    connect(m_parser, SIGNAL(resetAll()), m_parser, SLOT(resetData()));
    connect(m_parser, SIGNAL(resetAll()), this, SLOT(resetView()));
    connect(m_parser, SIGNAL(parseChanged()), this, SLOT(updateView()));

    if(m_dataComputerList.isEmpty())
        return;
    for(DataComputer* dataComputer : m_dataComputerList)
    {
        ui->comboBoxDataComputer->addItem(dataComputer->className());
    }
    ui->comboBoxDataComputer->activated(0);
}

HMLParser * MainWindow::getParser()
{
    return m_parser;
}

void MainWindow::displayBench()
{
    int id = ui->comboBoxBenchmark->currentIndex();
    if (id < 0)
        return;
    QString result ;
    QList<Benchmark*> benchmarkList = m_parser->getBenchmarkList();
    Benchmark* bench = benchmarkList.at(id);
    result += "Version : " + bench->getMonitoringVersion() + "\n";
    result += "Date    : " + bench->getDate() + "\n";
    result += "Gpu     : " + bench->getGPU() + "\n";
    result += "Nb data : " + QString::number(bench->getNbLine()) + "\n";
    for (int i = 0; i < bench->getColumnSize(); i++)
    {
        double value = m_currentDataComputer->compute(bench, i);
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
    ui->comboBoxBenchmark->clear();
    ui->textBrowser->clear();
}

void MainWindow::chooseDataToParse()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open RivaTuner hardware monitoring log file"), "", tr("Bench Data (*.hml)"));
    m_parser->parseData(fileName);
}

void MainWindow::updateView()
{
    QList<Benchmark*> benchmarkList =  m_parser->getBenchmarkList();
    if(benchmarkList.isEmpty())
        return;
    for(Benchmark* bench : benchmarkList)
        ui->comboBoxBenchmark->addItem(bench->getDate() + bench->getGPU());
    ui->comboBoxBenchmark->activated(0);
}

void MainWindow::setCurrentDataComputer(int id)
{
    if (id > m_dataComputerList.size())
        return;
    m_currentDataComputer = m_dataComputerList.at(id);

    emit displayBench();
}
