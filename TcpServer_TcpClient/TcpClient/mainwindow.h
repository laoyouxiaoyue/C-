#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTcpSocket>
#include <QMainWindow>
#include <QTextCursor>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTextCursor cursor;
private slots:
    void on_startButton_clicked();

    void on_sendButton_clicked();

    void on_pushButton_2_clicked();

private:
    int flag=0;
    Ui::MainWindow *ui;
    QTcpSocket *tcp;
};
#endif // MAINWINDOW_H
