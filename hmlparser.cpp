#include "hmlparser.h"

HMLParser::HMLParser(QObject *parent) : QObject(parent)
{

}

void HMLParser::parseData(QString fileName)
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
    emit parseChanged();
}

void HMLParser::parseLine(QString line)
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
            throw std::runtime_error("Error while parsing hlm file, 00 data instruction is missing");
        m_currentBenchmark->setGPU(list.at(2));
    }

    if (lineID.compare("02") == 0)
    {
        if(!m_currentBenchmark)
            throw std::runtime_error("Error while parsing hlm file, 00 data instruction is missing");
        m_currentBenchmark->setColumnSize(list.size() - 2);
        m_currentBenchmark->resetColumnValue();
        for(int i = 2; i < list.size(); i++)
            m_currentBenchmark->addColumnName(list.at(i));
    }

    if (lineID.compare("03") == 0)
    {
        if(!m_currentBenchmark)
            throw std::runtime_error("Error while parsing hlm file, 00 data instruction is missing");
        m_currentBenchmark->addColumnUnit(list.at(3));
    }

    if (lineID.compare("80") == 0)
    {
        if(!m_currentBenchmark)
            return;
        //At this point we ensure than every list has the same size
        if (m_currentBenchmark->getColumName().size() != m_currentBenchmark->getColumnSize())
            throw std::runtime_error("Error while parsing hlm file, 02 data instruction is missing");
        if (m_currentBenchmark->getColumUnit().size() != m_currentBenchmark->getColumnSize())
            throw std::runtime_error("Error while parsing hlm file, 03 data instruction is missing");

        for(int i = 2; i < list.size(); i++)
        {
            QString qStringValue = list.at(i);
            double value = qStringValue.toDouble();
            m_currentBenchmark->addColumnValue(i-2, value);
        }
        m_currentBenchmark->incrementNbLine();
    }
}

void HMLParser::resetData()
{
    m_currentBenchmark = nullptr;
    m_benchmarkList.clear();
}

QList<Benchmark*> HMLParser::getBenchmarkList()
{
    return m_benchmarkList;
}
