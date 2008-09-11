#include "sform.h"

#include <iostream>

SForm::SForm(QObject *parent)
    : QObject(parent),
    m_numChildren(0)
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
        qDebug() << "Connection to sform server failed:" << m_socket->error();
        ::sleep(1);
        reproduce();
    }

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    QString assembly = "quine";
    QString data = assembly;
    data.replace('\"', QString('\\') + QString('\"'));
    data.replace('\n', QString('\\') + QString('n'));
    QString quine = QString('q') +
                    QString('u') +
                    QString('i') +
                    QString('n') +
                    QString('e');
    assembly.replace(quine, data);

    QByteArray bytes = assembly.toLatin1();
    out << bytes.size();
    out << bytes.data();

    m_socket->write(block);
    m_socket->flush();
    m_socket->disconnectFromServer();
}

int main(int argc, char **argv)
{
    QCoreApplication *a = new QCoreApplication(argc, argv);
    QCoreApplication::setOrganizationDomain("treat.org");
    QCoreApplication::setApplicationName("sform");

    SForm sform;

    int rc = a->exec();

    return rc;
}
