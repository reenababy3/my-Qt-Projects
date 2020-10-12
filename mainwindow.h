#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSemaphore>
#include <processfile.h>
#include <qtextedit.h>
#include "highlightletters.h"
#include <qrubberband.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();
    void lock();
    void unlock();
    QRubberBand * rubberBand;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
signals:
    void fileSelected(QString fileName);
    void readyForData();

public slots:
    void loadData(QByteArray data);

private slots:
    void on_openButton_clicked();
    void on_startButton_clicked();
    void on_quitButton_clicked();

private:
     Letters_ColorPattern *assaign_color;
     void setupTextEditor();
     QPoint origin;

    enum UIState
    {
        NEED_FILE,
        CAN_PROCESS,
        PROCESSING,
        WAITING,
        ERROR
    };

    Ui::MainWindow *ui;
    ProcessFile *fileReader;
    void updateFromFileReaderState(ProcessFile::State state);
    void setState(UIState state);


    void closeEvent(QCloseEvent *event);


};
#endif // MAINWINDOW_H
