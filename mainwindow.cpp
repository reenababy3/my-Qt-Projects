#include <assert.h>
#include <iostream>

#include <QDebug>
#include <QObject>
#include <QThread>
#include <QFileDialog>
#include <QFont>
#include <QFontDatabase>
#include <processfile.h>
#include <QCloseEvent>
#include <QMoveEvent>

#include "MainWindow.h"
#include "ui_MainWindow.h"


const unsigned int DATA_DEPTH = 5;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    fileReader(new ProcessFile(DATA_DEPTH))
{

    qRegisterMetaType<ProcessFile::State>("ProcessFile::State");

    QThread *readerThread = new QThread(this);
    readerThread->setObjectName("ReaderThread");

    connect(readerThread, &QThread::finished, fileReader,  &QObject::deleteLater);

    fileReader->moveToThread(readerThread);
    readerThread->start();


    ui->setupUi(this);

    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    ui->data_viewEdit->setCurrentFont(fixedFont);


    setState(MainWindow::WAITING);

    connect(fileReader, &ProcessFile::stateChanged,this,&MainWindow::updateFromFileReaderState    );
    updateFromFileReaderState(fileReader->state());

    connect(fileReader, &ProcessFile::dataRead,this,&MainWindow::loadData    );

    connect(this, &MainWindow::readyForData,fileReader, &ProcessFile::startReading);

    connect(this,&MainWindow::fileSelected,fileReader, &ProcessFile::fileSelected);

}

void MainWindow::setState(MainWindow::UIState state)
{
    bool openEnabled  = state == NEED_FILE || state == CAN_PROCESS;
    bool startEnabled = state == CAN_PROCESS;

    ui->openButton->setEnabled(openEnabled);
    //ui->startButton->setEnabled(startEnabled);
}


void MainWindow::loadData(QByteArray data)
{
    const int hexLength = 10;
    QString hexString = data.data();
    QString renderString;
    renderString.reserve(hexString.size() * 1.5);

    for (int i = 0; i < hexString.length(); i += hexLength)
    {
        renderString.append(hexString.mid(i, hexLength) + " ");
    }


    //this is to give different colors to the texts
    // when this line added to the program ,it will load the textfile  very slowly
    assaign_color = new Letters_ColorPattern(ui->data_viewEdit->document());

    ui->data_viewEdit->append(renderString);

    //calling the threadClass
    fileReader->processedData();
}

void MainWindow::updateFromFileReaderState(ProcessFile::State state)
{
    ProcessFile::ReadingState rState = state.first;
    MainWindow::UIState uiState;

    switch (rState)
    {
        case ProcessFile::IDLE:
            uiState = MainWindow::NEED_FILE;
            break;

        case ProcessFile::READY:
            uiState = MainWindow::CAN_PROCESS;
            break;

        case ProcessFile::STARTED:
            uiState = MainWindow::PROCESSING;
            break;

        case ProcessFile::ERROR:
            uiState = MainWindow::ERROR;
            break;

        default:
            assert(0);
            break;
    }

    setState(uiState);
}

void MainWindow::on_startButton_clicked()
{
    setState(MainWindow::WAITING);
    emit readyForData();
}

void MainWindow::on_quitButton_clicked()
{
    close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();

    fileReader->thread()->quit();
    fileReader->thread()->wait();

    qApp->quit();
}


void MainWindow::on_openButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File");
    if (!fileName.isNull()) {
        emit fileSelected(fileName);
    }
    on_startButton_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    origin = event->pos();
        if (!rubberBand)
            rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        rubberBand->setGeometry(QRect(origin, QSize()));
        rubberBand->show();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    rubberBand->hide();

}
