#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtNetwork>
#include <QString>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#define CONVERT(x) QString::fromLocal8Bit((x))

QString MainWindow::getLocalIP() {
    QString hostName = QHostInfo::localHostName();
    QHostInfo hostInfo = QHostInfo::fromName(hostName);
    QString localIP = "";

    QList<QHostAddress> addList = hostInfo.addresses();
    if (!addList.isEmpty()) {
        for (QHostAddress aHost : addList) {
            if (QAbstractSocket::IPv4Protocol == aHost.protocol()) {
                localIP = aHost.toString();
                break;
            }
        }
    }
    return localIP;
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    LabSocketState = new QLabel(CONVERT("Socket状态: "));
    LabSocketState->setMinimumWidth(200);
    ui->statusBar->addWidget(LabSocketState);

    QString localIP = getLocalIP();
    this->setWindowTitle(this->windowTitle() + CONVERT(" 本机IP: ") + localIP);

    udpSocket = new QUdpSocket(this);

    connect(udpSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this, SLOT(onSocketStateChange(QAbstractSocket::SocketState)));
    onSocketStateChange(udpSocket->state());

    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
}


void MainWindow::onSocketStateChange(QAbstractSocket::SocketState socketState)
{
    switch(socketState)
    {
    case QAbstractSocket::UnconnectedState:
        LabSocketState->setText(CONVERT("scoket状态：UnconnectedState"));
        break;
    case QAbstractSocket::HostLookupState:
        LabSocketState->setText(CONVERT("scoket状态：HostLookupState"));
        break;
    case QAbstractSocket::ConnectingState:
        LabSocketState->setText(CONVERT("scoket状态：ConnectingState"));
        break;

    case QAbstractSocket::ConnectedState:
        LabSocketState->setText(CONVERT("scoket状态：ConnectedState"));
        break;

    case QAbstractSocket::BoundState:
        LabSocketState->setText(CONVERT("scoket状态：BoundState"));
        break;

    case QAbstractSocket::ClosingState:
        LabSocketState->setText(CONVERT("scoket状态：ClosingState"));
        break;

    case QAbstractSocket::ListeningState:
        LabSocketState->setText(CONVERT("scoket状态：ListeningState"));
    }
}

void MainWindow::onSocketReadyRead() {
    while(udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress peerAddr;
        quint16 peerPort;
        udpSocket->readDatagram(datagram.data(), datagram.size(), &peerAddr, &peerPort);
        QString str = datagram.data();
        QString peer = "[From " + peerAddr.toString() + ":" + QString::number(peerPort) + "]";
        ui->plainTextEdit->appendPlainText(peer + str);
    }
}

void MainWindow::on_actStart_triggered() {
    quint16 port = ui->spinBindPort->value();
    if (udpSocket->bind(port)) {
        ui->plainTextEdit->appendPlainText(CONVERT("**已成功绑定"));
        ui->plainTextEdit->appendPlainText(CONVERT("**绑定端口: ") + QString::number(udpSocket->localPort()));

        ui->actStart->setEnabled(false);
        ui->actStop->setEnabled(true);
    } else {
        ui->plainTextEdit->appendPlainText(CONVERT("**绑定失败"));
    }
}

void MainWindow::on_actStop_triggered() {
    udpSocket->abort();
    ui->actStart->setEnabled(true);
    ui->actStop->setEnabled(false);
    ui->plainTextEdit->appendPlainText(CONVERT("**已解除绑定"));
}

void MainWindow::on_actClear_triggered() {
    ui->plainTextEdit->clear();
}

void MainWindow::on_btnSend_clicked() {
    QString targetIP = ui->comboTargetIP->currentText();
    QHostAddress targetAddr(targetIP);
    quint16 targetPort = ui->spinTargetPort->value();
    QString msg = ui->editMsg->text();

    QByteArray str = msg.toUpper().toUtf8();
    udpSocket->writeDatagram(str, targetAddr, targetPort);

    ui->plainTextEdit->appendPlainText("[out] " + msg);
    ui->editMsg->clear();
    ui->editMsg->setFocus();
}

void MainWindow::on_btnBroadcast_clicked() {
    quint16 targetPort = ui->spinTargetPort->value();
    QString msg = ui->editMsg->text();
    QByteArray str = msg.toUpper().toUtf8();

    udpSocket->writeDatagram(str, QHostAddress::Broadcast, targetPort);

    ui->plainTextEdit->appendPlainText("[broadcast] " + msg);
    ui->editMsg->clear();
    ui->editMsg->setFocus();
}



MainWindow::~MainWindow()
{
    udpSocket->abort();
    delete udpSocket;
    delete ui;
}

