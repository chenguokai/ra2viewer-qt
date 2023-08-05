#include "mainwindow.h"

#include <QApplication>
#include <QTimer>
#include <iostream>
#include <pthread.h>
#include <Qt>

void createConf() {
    FILE *fp = std::fopen("./conf.txt", "w");
    std::fprintf(fp, "400 # modify this to adjust where the frame is\n");
    std::fprintf(fp, "700 # modify this to adjust the frame width\n");
    std::fprintf(fp, "300 # modify this to adjust the frame height\n");
    fclose(fp);
}

int position, width, height;
char confBuf[100];
int confSize = 100;
#include <stdio.h>
#include <stdlib.h>
void loadConf() {
    FILE *fp = std::fopen("./conf.txt", "r");
    if (fp == NULL) {
        createConf();
        fp = std::fopen("./conf.txt", "r");
    }
    fgets(confBuf, confSize, fp);
    std::sscanf(confBuf, "%d", &position);
    fgets(confBuf, confSize, fp);
    sscanf(confBuf, "%d", &width);
    fgets(confBuf, confSize, fp);
    sscanf(confBuf, "%d", &height);
    printf("pos %d width %d height %d", position, width, height);
}

class MyTimer : public QObject
{
    Q_OBJECT
public slots:
    void MyTimerSlot() {
        std::cout << "Timer...\n";
        updateWindow();
        //mainwin->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
        //mainwin->raise();
        //mainwin->activateWindow();
        mainwin->resize(width, height);
        mainwin->show();
        //mainwin->
    }


public:
    MainWindow *mainwin;
    MyTimer() {
        // create a timer
        timer = new QTimer(this);

        // setup signal and slot
        connect(timer, SIGNAL(timeout()),
                this, SLOT(MyTimerSlot()));

        // msec
        timer->start(1000);
        std::cout<<"timer started\n";
    }
    void setMainWindow(MainWindow *a) {
        mainwin = a;
    }
    void updateWindow() {
        mainwin->updateData();
    }
    QTimer *timer;
};
#include "main.moc"
#include <QDesktopWidget>
void* ra2_main(void* arg);
int main(int argc, char *argv[])
{
    loadConf();
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    MyTimer timer;
    timer.setMainWindow(&w);
    pthread_t ptid;

    // Creating a new thread
    pthread_create(&ptid, NULL, &ra2_main, NULL);

    QDesktopWidget *desktop = QApplication::desktop();
    int screenWidth;
    int screenHeight;

    int x, y;
    screenWidth = desktop->width();
    screenHeight = desktop->height();

    w.move(0, screenHeight - position); // first: left to right second: top to buttom
    a.exec();


    return 0;
}
