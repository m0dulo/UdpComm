#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QLabel* LabSocketState;
    QUdpSocket* udpSocket;
    QString getLocalIP();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSocketStateChange(QAbstractSocket::SocketState socketState);
    void onSocketReadyRead();

    void on_actStart_triggered();
    void on_actStop_triggered();
    void on_actClear_triggered();
    void on_btnSend_clicked();
    void on_btnBroadcast_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
