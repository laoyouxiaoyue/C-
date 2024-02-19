#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTcpServer>
#include <QTcpSocket>
#include <QMainWindow>
#include <vector>
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
    std::map<QTcpSocket*,QString>index;
    QString getUser();


private slots:
    void on_startButton_clicked();

    void on_sendButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer *server;

};
#endif // MAINWINDOW_H
