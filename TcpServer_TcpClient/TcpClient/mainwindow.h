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
    void BIU();
    void on_startButton_clicked();
    void on_sendButton_clicked();

    void on_pushButton_2_clicked();

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    int flag=0;
    int Bflag=0;
    int Uflag=0;
    int Iflag=0;
    Ui::MainWindow *ui;
    QTcpSocket *tcp;
    QTextCharFormat *fs;
};
#endif // MAINWINDOW_H
