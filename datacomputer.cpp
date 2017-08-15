#include "datacomputer.h"

DataComputer::DataComputer(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(updateView()), this->parent(), SLOT(updateView()));
}
