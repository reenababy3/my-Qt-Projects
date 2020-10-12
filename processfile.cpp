#include <assert.h>

#include <QDebug>
#include <QFile>
#include <QThread>

#include "processfile.h"

ProcessFile::ProcessFile(unsigned int depth, QObject *parent) :
    QObject(parent),
    _stateLock(),
    _state(IDLE, QString()),
    _dataCounter(depth)
{

}

void ProcessFile::fileSelected(QString fileName)
{
    changeState(IDLE, State(READY, fileName));
}

void ProcessFile::startReading()
{
    static const unsigned DATA_SIZE = 256;

    unsigned debugCounter = 0;

    QString fileName = state().second;

    if (changeState(READY, State(STARTED, QString())))
    {
        {
            QFile inFile(fileName);

            if (!inFile.open(QIODevice::ReadOnly))
            {
                changeState(STARTED, State(ERROR, QString()));
                return;
            }

            while(!inFile.atEnd())
            {
                QByteArray qa = inFile.read(DATA_SIZE);
                _dataCounter.acquire();
                qDebug() << "emitting signal dataRead(): "
                         << debugCounter
                         << " / "
                         << _dataCounter.available();

                emit dataRead(qa);
                debugCounter++;
            }
        }

        changeState(STARTED, State(IDLE, QString()));
    }
}

void ProcessFile::processedData()
{
    qDebug() << "here calling processedData()";
    _dataCounter.release();
    qDebug() << "this is released data count: " << _dataCounter.available();
}

ProcessFile::~ProcessFile()
{
}

ProcessFile::State ProcessFile::state()
{
    QMutexLocker locker(&_stateLock);
    return _state;
}

bool ProcessFile::changeState(ReadingState oldRState, State newState)
{
    ReadingState newRState = newState.first;
    QString fileName = newState.second;
    bool changed = false;
    bool valid;

    assert((newRState == READY) == (!fileName.isNull()));
    {
        QMutexLocker locker(&_stateLock);
        valid = oldRState == _state.first;

        if (valid)
        {
            changed = newState != _state;
            _state = newState;
        }
    }

    if (changed)
    {
        emit stateChanged(newState);
    }

    return valid;
}
