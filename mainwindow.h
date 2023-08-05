#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateData();

private:
    Ui::MainWindow *ui;

    QLabel *m_LabelInfo[4][4];
    QListWidgetItem *imageItem[4][7];

    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    int hotkeyid;
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) __attribute__((thiscall));

private slots:
    void v1ButtonClicked();
    void v2ButtonClicked();
    void exitButtonClicked();
};

#endif // MAINWINDOW_H
