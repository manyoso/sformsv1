#include "server.h"

#include <QDebug>

#include <limits.h>
#include <signal.h>

#include <QFile>

static void crashHandler(int sig)
{
    fprintf(stderr, "%s\n", strsignal(sig));
    QFile::remove("/tmp/sform");
    exit(128 + sig);
}

Server::Server(QObject *parent)
    : QLocalServer(parent), _blockSize(0)
{
    if (!listen(QLatin1String("sform"))) {
        qDebug() << "Unable to start the sform server:"
                 << errorString();
        exit(1);
    }

    signal(SIGINT,  crashHandler);    /* 2:   interrupt */
    signal(SIGILL,  crashHandler);    /* 4:   illegal instruction (not reset when caught) */
    signal(SIGTRAP, crashHandler);    /* 5:   trace trap (not reset when caught) */
    signal(SIGFPE,  crashHandler);    /* 8:   floating point exception */
    signal(SIGBUS,  crashHandler);    /* 10:  bus error */
    signal(SIGSEGV, crashHandler);    /* 11:  segmentation violation */
    signal(SIGSYS,  crashHandler);    /* 12:  bad argument to system call */
    signal(SIGPIPE, crashHandler);    /* 13:  write on a pipe with no reader */
    signal(SIGTERM, crashHandler);    /* 15:  terminate */
    signal(SIGXCPU, crashHandler);    /* 24:  exceeded CPU time limit */
    signal(SIGXFSZ, crashHandler);    /* 25:  exceeded file size limit */

    connect(this, SIGNAL(newConnection()),
            this, SLOT(establishConnection()));
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

    _blockSize = 0;
    clientConnection->waitForReadyRead(-1);
}

void Server::readClientData()
{
    qDebug() << "readClientData";
    QLocalSocket *clientConnection = qobject_cast<QLocalSocket*>(sender());
    Q_ASSERT(clientConnection);

    QDataStream in(clientConnection);
    if (_blockSize == 0) {
        in >> _blockSize;
    }

    if (clientConnection->bytesAvailable() < _blockSize)
        return;

    char *data;
    in >> data;
}
