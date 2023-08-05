#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QListWidget.h>
#include <Qt>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QtGui>
#include <QLabel>
#include <QTimer>
#include <iostream>
#include "ra2.h"
#include <QString>

#include <windows.h>
// #include <QListWidgetItem.h>

extern int width, height;

char countryBuf[100];

char *getCountry(int i) {
    if (strlen(countryName[i]) > 20) return countryBuf;
    if (strcmp(countryName[i], allieCountries[0]) == 0) {
        strcpy(countryBuf, "美");
    } else if (strcmp(countryName[i], allieCountries[1]) == 0) {
        strcpy(countryBuf, "韩");
    } else if (strcmp(countryName[i], allieCountries[2]) == 0) {
        strcpy(countryBuf, "法");
    } else if (strcmp(countryName[i], allieCountries[3]) == 0) {
        strcpy(countryBuf, "德");
    } else if (strcmp(countryName[i], allieCountries[4]) == 0) {
        strcpy(countryBuf, "英");
    } else if (strcmp(countryName[i], sovietCountries[0]) == 0) {
        strcpy(countryBuf, "利");
    } else if (strcmp(countryName[i], sovietCountries[1]) == 0) {
        strcpy(countryBuf, "伊");
    } else if (strcmp(countryName[i], sovietCountries[2]) == 0) {
        strcpy(countryBuf, "古");
    } else if (strcmp(countryName[i], sovietCountries[3]) == 0) {
        strcpy(countryBuf, "苏");
    } else {
        strcpy(countryBuf, countryName[i]);
    }

    return countryBuf;
}

std::wstring getId(int i) {
    std::wstring ret = UserName[i];
    return ret;
}

char balanceBuf[100];

char *getBalance(int i) {
    sprintf(balanceBuf, "%d", balance[i]);
    return balanceBuf;
}

char powerBuf[100];
char *getPower(int i) {
    sprintf(powerBuf, "%d\n%d", powerDrain[i], powerOutput[i]);
    return powerBuf;
}

bool getSide(int idx) {
    for (int i = 0; i < 5; i++) {
        if (strcmp(countryName[idx], allieCountries[i]) == 0) {
            return true;
        }
    }
    return false;
}

char unitBuf[100];

char *getUnit(int i, int unit, bool isAlly) {
    switch (unit) {
    case 0:
        if (isAlly) {
            sprintf(unitBuf, "%d", ALLIMinerCount[i]);
        } else {
            sprintf(unitBuf, "%d", SOVMinerCount[i]);
        }
        break;
        case 1:
        if (isAlly) {
            sprintf(unitBuf, "%d", ALLIWarFactoryCount[i]);
        } else {
            sprintf(unitBuf, "%d", SOVWarFactoryCount[i]);
        }
        break;
        case 2:
        if (isAlly) {
            sprintf(unitBuf, "%d", ALLITANKcount[i]);
        } else {
            sprintf(unitBuf, "%d", SOVTANKcount[i]);
        }
        break;
        case 3:
        if (isAlly) {
            sprintf(unitBuf, "%d", ALLIDOGcount[i]);
        } else {
            sprintf(unitBuf, "%d", SOVDOGcount[i]);
        }
        break;
        case 4:
        if (isAlly) {
            sprintf(unitBuf, "%d", ALLIFlycount[i]);
        } else {
            sprintf(unitBuf, "%d", SOVSpidercount[i]);
        }
        break;
        case 5:
        if (isAlly) {
            sprintf(unitBuf, "%d", ALLIIFVcount[i]);
        } else {
            sprintf(unitBuf, "%d", SOVIFVcount[i]);
        }
        break;
        case 6:
        if (isAlly) {
            sprintf(unitBuf, "%d", GIcount[i]);
        } else {
            sprintf(unitBuf, "%d", SOVCONSCRIPTcount[i]);
        }
        break;
        case 7:
        if (isAlly) {
            sprintf(unitBuf, "%d", ALLIBOATcount[i]);
        } else {
            sprintf(unitBuf, "%d", SOVBOATcount[i]);
        }
        break;
        case 8:
        if (isAlly) {
            sprintf(unitBuf, "%d", ALLIFISHcount[i]);
        } else {
            sprintf(unitBuf, "%d", SOVFISHcount[i]);
        }
        break;
        case 9:
        if (isAlly) {
            sprintf(unitBuf, "%d", ALLICARRIERcount[i]);
        } else {
            sprintf(unitBuf, "%d", SOVCARRIERcount[i]);
        }
        break;
    }


    return unitBuf;
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
        last_mouse_position = event->globalPos();
}
void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (!event->buttons().testFlag(Qt::LeftButton))
        return;
    const QPoint position = pos() + event->globalPos() - last_mouse_position; //the position of mainfrmae + (current_mouse_position - last_mouse_position)
    move(position.x(), position.y());
    last_mouse_position = event->globalPos();
}

void MainWindow::v1ButtonClicked() {
    exit(0);
}

void MainWindow::v2ButtonClicked() {
    exit(0);
}

void MainWindow::exitButtonClicked() {
    UnregisterHotKey(HWND(this->winId()), hotkeyid);
    exit(0);
}

void MainWindow::enterEvent(QEvent *) {
    //setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    //show();
}

void MainWindow::leaveEvent(QEvent *) {
    //setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    //show();
}

bool powerOn(int i) {
    return powerDrain[i] < powerOutput[i];
}

void MainWindow::updateData() {
    int totValid = 0;
    // this->setWindowTitle("Updated");
    for (int i = 0; i < MAXPLAYERS; i++) {
        if (totValid > 3) continue;
        if (validPlayer[i]) {
            m_LabelInfo[totValid][0]->setText(getCountry(i));

            QPalette palette0 = m_LabelInfo[totValid][0]->palette();
            switch (color[i]) {
            case Yellow:
                palette0.setColor(m_LabelInfo[totValid][0]->backgroundRole(), Qt::yellow);
                break;
            case Red:
                palette0.setColor(m_LabelInfo[totValid][0]->backgroundRole(), Qt::red);
                break;
            case Orange:
                palette0.setColor(m_LabelInfo[totValid][0]->backgroundRole(), QColor(255, 165, 0));
                break;
            case Pink:
                palette0.setColor(m_LabelInfo[totValid][0]->backgroundRole(), QColor(255, 192, 203));
                break;
            case Purple:
                palette0.setColor(m_LabelInfo[totValid][0]->backgroundRole(), QColor(128, 0, 128));
                break;
            case Blue:
                palette0.setColor(m_LabelInfo[totValid][0]->backgroundRole(), Qt::blue);
                break;
            case Green:
                palette0.setColor(m_LabelInfo[totValid][0]->backgroundRole(), Qt::green);
                break;
            case Sky:
                palette0.setColor(m_LabelInfo[totValid][0]->backgroundRole(), Qt::cyan);
                break;
            }



            m_LabelInfo[totValid][0]->setPalette(palette0);

            m_LabelInfo[totValid][1]->setText(QString::fromStdWString(getId(i)));
            m_LabelInfo[totValid][2]->setText(getBalance(i));
            m_LabelInfo[totValid][3]->setText(getPower(i));
            QPalette palette = m_LabelInfo[totValid][3]->palette();
            if (powerOn(i))
                palette.setColor(m_LabelInfo[totValid][3]->foregroundRole(), Qt::green);
            else
                palette.setColor(m_LabelInfo[totValid][3]->foregroundRole(), Qt::red);
            m_LabelInfo[totValid][3]->setPalette(palette);
            for (int j = 0; j < 10; j++) {
                imageItem[totValid][j]->setText(getUnit(i, j, getSide(i)));


                switch (j) {
                case 0:
                    if (getSide(i))
                        imageItem[totValid][j]->setIcon(QIcon("./allyminer.png"));
                    else
                        imageItem[totValid][j]->setIcon(QIcon("./sovminer.png"));
                    break;
                case 1:
                    if (getSide(i))
                        imageItem[totValid][j]->setIcon(QIcon("./allywarfactory.png"));
                    else
                        imageItem[totValid][j]->setIcon(QIcon("./sovwarfactory.png"));
                    break;
                case 2:
                    if (getSide(i))
                        imageItem[totValid][j]->setIcon(QIcon("./allytank.png"));
                    else
                        imageItem[totValid][j]->setIcon(QIcon("./sovtank.png"));
                    break;
                case 3:
                    if (getSide(i))
                        imageItem[totValid][j]->setIcon(QIcon("./allydog.png"));
                    else
                        imageItem[totValid][j]->setIcon(QIcon("./sovdog.png"));
                    break;
                case 4:
                    if (getSide(i))
                        imageItem[totValid][j]->setIcon(QIcon("./fly.png"));
                    else
                        imageItem[totValid][j]->setIcon(QIcon("./sovfly.png"));
                    break;
                case 5:
                    if (getSide(i))
                        imageItem[totValid][j]->setIcon(QIcon("./ifv.png"));
                    else
                        imageItem[totValid][j]->setIcon(QIcon("./sovifv.png"));
                    break;
                case 6:
                    if (getSide(i))
                        imageItem[totValid][j]->setIcon(QIcon("./allysoldier.png"));
                    else
                        imageItem[totValid][j]->setIcon(QIcon("./sovsoldier.png"));
                    break;
                case 7:
                    if (getSide(i))
                        imageItem[totValid][j]->setIcon(QIcon("./allyboat.png"));
                    else
                        imageItem[totValid][j]->setIcon(QIcon("./sovboat.png"));
                    break;
                case 8:
                    if (getSide(i))
                        imageItem[totValid][j]->setIcon(QIcon("./allyfish.png"));
                    else
                        imageItem[totValid][j]->setIcon(QIcon("./sovfish.png"));
                    break;
                case 9:
                    if (getSide(i))
                        imageItem[totValid][j]->setIcon(QIcon("./allycv.png"));
                    else
                        imageItem[totValid][j]->setIcon(QIcon("./sovcv.png"));
                    break;
                }
            }
            ++totValid;

        }
    }
    this->show();
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result) {
    //setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    //show();
    if (eventType == "windows_generic_MSG") {

        MSG *pMsg = reinterpret_cast<MSG*>(message);
        if (pMsg->message == WM_HOTKEY) {
            std::cout<<"hotkeyhit\n";
            if (pMsg->wParam == hotkeyid) {
                setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
                //setWindowFlags(Qt::WindowStaysOnTopHint);
                show();
                //UnregisterHotKey(HWND(this->winId()), hotkeyid);
                //exit(0);
            }
        }
    }
    return QMainWindow::nativeEvent(eventType, message, result);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    this->setWindowTitle("RA2Viewer");




    QVBoxLayout *vLayout = new QVBoxLayout;

    QHBoxLayout *modeLayout = new QHBoxLayout;

    QListWidget *m_ImageList[4];



    for (int i = 0; i < 4; i++) {
        m_ImageList[i] = new QListWidget;
        m_ImageList[i]->setViewMode(QListWidget::IconMode);//显示模式

        m_ImageList[i]->setFlow(QListView::LeftToRight);

        m_ImageList[i]->setIconSize(QSize(40, 40));//设置图片大小

        m_ImageList[i]->setSpacing(5);//间距

        m_ImageList[i]->setResizeMode(QListView::Adjust);//适应布局调整

        m_ImageList[i]->setMovement(QListView::Static);//不能移动
        m_ImageList[i]->setFixedHeight(60);
        for (int j = 0; j < 4; j++) {
            m_LabelInfo[i][j] = new QLabel;
            m_LabelInfo[i][j]->setFixedHeight(40);
            switch (j) {
            case 0:
                m_LabelInfo[i][j]->setFixedWidth(30);
                break;
            case 1: case 2:
                m_LabelInfo[i][j]->setFixedWidth(60);
                break;
            case 4:
                m_LabelInfo[i][j]->setFixedWidth(40);
                break;
            }


        }
        QPalette palette = m_LabelInfo[i][0]->palette();
        palette.setColor(m_LabelInfo[i][0]->backgroundRole(), Qt::yellow);
        palette.setColor(m_LabelInfo[i][0]->foregroundRole(), Qt::black);

        m_LabelInfo[i][0]->setText("法");
        m_LabelInfo[i][0]->setFont(QFont("Arial", 20));
        m_LabelInfo[i][0]->setAutoFillBackground(true);
        m_LabelInfo[i][0]->setPalette(palette);
        m_LabelInfo[i][1]->setText("TestID");
        m_LabelInfo[i][2]->setText("10000");
        m_LabelInfo[i][3]->setText("1000\n2000");

    }




    for (int players = 0; players < 4; players++) {
        for (int i = 0; i < 10; i++) {
            imageItem[players][i] = new QListWidgetItem;

            imageItem[players][i]->setText("BEIJING");
            imageItem[players][i]->setSizeHint(QSize(50, 50));

            m_ImageList[players]->addItem(imageItem[players][i]);

            switch (i) {
            case 0:
                imageItem[players][i]->setIcon(QIcon("./allyminer.png"));
                break;
            case 1:
                imageItem[players][i]->setIcon(QIcon("./allywarfactory.png"));
                break;
            case 2:
                imageItem[players][i]->setIcon(QIcon("./allytank.png"));
                break;
            case 3:
                imageItem[players][i]->setIcon(QIcon("./allydog.png"));
                break;
            case 4:
                imageItem[players][i]->setIcon(QIcon("./fly.png"));
                break;
            case 5:
                imageItem[players][i]->setIcon(QIcon("./ifv.png"));
                break;
            case 6:
                imageItem[players][i]->setIcon(QIcon("./allysoldier.png"));
                break;
            case 7:
                imageItem[players][i]->setIcon(QIcon("./allysoldier.png"));
                break;
            case 8:
                imageItem[players][i]->setIcon(QIcon("./allysoldier.png"));
                break;
            case 9:
                imageItem[players][i]->setIcon(QIcon("./allysoldier.png"));
                break;
            }
        }
    }

    QPushButton *v2Button = new QPushButton("2v2");
    QPushButton *v1Button = new QPushButton("1v1");
    QPushButton *exitButton = new QPushButton("Exit");
    connect ( v2Button, SIGNAL( clicked() ), this, SLOT( v2ButtonClicked() ) );
    connect ( v1Button, SIGNAL( clicked() ), this, SLOT( v1ButtonClicked() ) );
    connect ( exitButton, SIGNAL( clicked() ), this, SLOT( exitButtonClicked() ) );


    QHBoxLayout *players[4];
    for (int i = 0; i < 4; i++) {
        players[i] = new QHBoxLayout;

        for (int j = 0; j < 4; j++)
            players[i]->addWidget(m_LabelInfo[i][j]);

        players[i]->addWidget(m_ImageList[i]);
    }

    for (int i = 0; i < 4; i++)
        vLayout->addLayout(players[i]);
    vLayout->addLayout(modeLayout);
    modeLayout->addWidget(v1Button);
    modeLayout->addWidget(v2Button);
    modeLayout->addWidget(exitButton);

    auto wid = new QWidget();
    setCentralWidget(wid);

    wid->setLayout(vLayout);
    //MainWindow::setLayout(vLayout);
    resize(700, 300);




    RegisterHotKey(HWND(this->winId()), hotkeyid, MOD_CONTROL | MOD_ALT, 'W');


}


MainWindow::~MainWindow()
{
    delete ui;
}

