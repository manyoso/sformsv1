#include "sform.h"

#include <iostream>

SForm::SForm(QObject *parent)
    : QObject(parent)
{
    m_socket = new QLocalSocket(this);
    reproduce();
}

SForm::~SForm()
{
}

void SForm::reproduce()
{
    m_socket->abort();
    m_socket->connectToServer(QLatin1String("sform"));
    if (!m_socket->waitForConnected(-1)) {
        ::sleep(1);
        reproduce();
    }

    qint64 pid = QCoreApplication::applicationPid();
    qDebug() << "reproduce pid:\t" << pid;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    QString assembly = "quine";
    QByteArray bytes = assembly.toLatin1();
    out << bytes.size();
    out << bytes.data();
    out << QCoreApplication::applicationPid();
    m_socket->write(block);
    m_socket->flush();
    m_socket->disconnectFromServer();

    ::sleep(1);
    reproduce(); //recurse
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setOrganizationDomain("treat.org");
    QCoreApplication::setApplicationName("sform");

    SForm sform(&app);

    int rc = app.exec();
    return rc;
}
