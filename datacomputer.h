#ifndef DATACOMPUTER_H
#define DATACOMPUTER_H

#include <QObject>

class DataComputer : public QObject
{
    Q_OBJECT
public:
    explicit DataComputer(QObject *parent = nullptr);

signals:

public slots:
};

#endif // DATACOMPUTER_H