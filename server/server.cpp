#include "server.h"

#include <QDebug>

Server::Server(QObject *parent)
    : QLocalServer(parent)
{
    if (!listen(QLatin1String("sform"))) {
        qDebug() << "Unable to start the sform server:"
                 << errorString();
        close();
        return;
    }

    connect(this, SIGNAL(newConnection()), this, SLOT(establishConnection()));
}

Server::~Server()
{
    close();
}

void Server::establishConnection()
{
    qDebug() << "establishConnection";

    QLocalSocket *clientConnection = nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));

    connect(clientConnection, SIGNAL(readyRead()),
            this, SLOT(readClientData()));

    clientConnection->waitForReadyRead(-1);
}

void Server::readClientData()
{
    qDebug() << "reading client data...";
    QLocalSocket *clientConnection = qobject_cast<QLocalSocket*>(sender());
    if (!clientConnection) {
        qDebug() << "client connection broken";
    }

    QDataStream in(clientConnection);
    char *data;
    in >> data;
    qDebug() << data;
}
