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
}

void Server::establishConnection() {
    qDebug() << "establishConnection";
}
