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

    QString assembly = "#include \"sform.h\"\n\n#include <iostream>\n\nSForm::SForm(QObject *parent)\n    : QObject(parent),\n    m_numChildren(0)\n{\n    m_socket = new QLocalSocket(this);\n    reproduce();\n}\n\nSForm::~SForm()\n{\n}\n\nvoid SForm::reproduce()\n{\n    m_socket->abort();\n    m_socket->connectToServer(QLatin1String(\"sform\"));\n    if (!m_socket->waitForConnected(-1)) {\n        qDebug() << \"Connection to sform server failed:\" << m_socket->error();\n        ::sleep(1);\n        reproduce();\n    }\n\n    QByteArray block;\n    QDataStream out(&block, QIODevice::WriteOnly);\n\n    QString assembly = \"quine\";\n    QString data = assembly;\n    data.replace('\\"', QString('\\') + QString('\\"'));\n    data.replace('\n', QString('\\') + QString('n'));\n    QString quine = QString('q') +\n                    QString('u') +\n                    QString('i') +\n                    QString('n') +\n                    QString('e');\n    assembly.replace(quine, data);\n\n    QByteArray bytes = assembly.toLatin1();\n    out << bytes.size();\n    out << bytes.data();\n\n    m_socket->write(block);\n    m_socket->flush();\n    m_socket->disconnectFromServer();\n}\n\nint main(int argc, char **argv)\n{\n    QCoreApplication app(argc, argv);\n    QCoreApplication::setOrganizationDomain(\"treat.org\");\n    QCoreApplication::setApplicationName(\"sform\");\n\n    SForm sform(&app);\n\n    int rc = app.exec();\n    return rc;\n}\n";
    QString data = assembly;
    data.replace('\"', QString('\\') + QString('\"'));
    data.replace('\n', QString('\\') + QString('n'));
    QString #include \"sform.h\"\n\n#include <iostream>\n\nSForm::SForm(QObject *parent)\n    : QObject(parent),\n    m_numChildren(0)\n{\n    m_socket = new QLocalSocket(this);\n    reproduce();\n}\n\nSForm::~SForm()\n{\n}\n\nvoid SForm::reproduce()\n{\n    m_socket->abort();\n    m_socket->connectToServer(QLatin1String(\"sform\"));\n    if (!m_socket->waitForConnected(-1)) {\n        qDebug() << \"Connection to sform server failed:\" << m_socket->error();\n        ::sleep(1);\n        reproduce();\n    }\n\n    QByteArray block;\n    QDataStream out(&block, QIODevice::WriteOnly);\n\n    QString assembly = \"quine\";\n    QString data = assembly;\n    data.replace('\\"', QString('\\') + QString('\\"'));\n    data.replace('\n', QString('\\') + QString('n'));\n    QString quine = QString('q') +\n                    QString('u') +\n                    QString('i') +\n                    QString('n') +\n                    QString('e');\n    assembly.replace(quine, data);\n\n    QByteArray bytes = assembly.toLatin1();\n    out << bytes.size();\n    out << bytes.data();\n\n    m_socket->write(block);\n    m_socket->flush();\n    m_socket->disconnectFromServer();\n}\n\nint main(int argc, char **argv)\n{\n    QCoreApplication app(argc, argv);\n    QCoreApplication::setOrganizationDomain(\"treat.org\");\n    QCoreApplication::setApplicationName(\"sform\");\n\n    SForm sform(&app);\n\n    int rc = app.exec();\n    return rc;\n}\n = QString('q') +
                    QString('u') +
                    QString('i') +
                    QString('n') +
                    QString('e');
    assembly.replace(#include \"sform.h\"\n\n#include <iostream>\n\nSForm::SForm(QObject *parent)\n    : QObject(parent),\n    m_numChildren(0)\n{\n    m_socket = new QLocalSocket(this);\n    reproduce();\n}\n\nSForm::~SForm()\n{\n}\n\nvoid SForm::reproduce()\n{\n    m_socket->abort();\n    m_socket->connectToServer(QLatin1String(\"sform\"));\n    if (!m_socket->waitForConnected(-1)) {\n        qDebug() << \"Connection to sform server failed:\" << m_socket->error();\n        ::sleep(1);\n        reproduce();\n    }\n\n    QByteArray block;\n    QDataStream out(&block, QIODevice::WriteOnly);\n\n    QString assembly = \"quine\";\n    QString data = assembly;\n    data.replace('\\"', QString('\\') + QString('\\"'));\n    data.replace('\n', QString('\\') + QString('n'));\n    QString quine = QString('q') +\n                    QString('u') +\n                    QString('i') +\n                    QString('n') +\n                    QString('e');\n    assembly.replace(quine, data);\n\n    QByteArray bytes = assembly.toLatin1();\n    out << bytes.size();\n    out << bytes.data();\n\n    m_socket->write(block);\n    m_socket->flush();\n    m_socket->disconnectFromServer();\n}\n\nint main(int argc, char **argv)\n{\n    QCoreApplication app(argc, argv);\n    QCoreApplication::setOrganizationDomain(\"treat.org\");\n    QCoreApplication::setApplicationName(\"sform\");\n\n    SForm sform(&app);\n\n    int rc = app.exec();\n    return rc;\n}\n, data);

    QByteArray bytes = assembly.toLatin1();
    out << bytes.size();
    out << bytes.data();

    m_socket->write(block);
    m_socket->flush();
    m_socket->disconnectFromServer();
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
