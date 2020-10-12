#ifndef PROCESSFILE_H
#define PROCESSFILE_H

#include <QObject>
#include <QPair>
#include <QThread>
#include <QMutex>
#include <QSemaphore>
class ProcessFile : public QObject
{
    Q_OBJECT

public:
    enum ReadingState
    {
        IDLE,
        READY,
        STARTED,
        ERROR
    };

    typedef QPair<ReadingState, QString> State;

    explicit ProcessFile(unsigned int depth, QObject *parent = 0);
    ~ProcessFile();

    State state();
    void processedData();

signals:
    void dataRead(QByteArray data);
    void stateChanged(ProcessFile::State newState);

public slots:
    void fileSelected(QString fileName);
    void startReading();

private:
    QMutex _stateLock;
    State _state;

    QSemaphore _dataCounter;

    bool changeState(ReadingState oldState, State newState);
};

#endif // PROCESSFILE_H
