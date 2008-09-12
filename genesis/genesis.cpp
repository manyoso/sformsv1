#include "genesis.h"

#include <QDebug>

#include <limits.h>
#include <signal.h>

#include <QDir>
#include <QFile>
#include <QTime>
#include <QTimer>
#include <QProcess>
#include <QStringList>
#include <QLibraryInfo>
#include <QCoreApplication>

static void crashHandler(int sig)
{
    fprintf(stderr, "%s\n", strsignal(sig));
    QFile::remove("/tmp/sform");
    exit(128 + sig);
}

Genesis::Genesis(QObject *parent)
    : QLocalServer(parent), _blockSize(0)
{
    if (!listen(QLatin1String("sform"))) {
        qDebug() << "Unable to start the sform genesis:"
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

    _reaper = new QTimer(this);
    _reaper->start(1000); //every second
    connect(_reaper, SIGNAL(timeout()), this, SLOT(reap()));
}

Genesis::~Genesis()
{
    close();

    QHash<qint64, QTime>::const_iterator it = _processTable.constBegin();
    while (it != _processTable.constEnd()) {
        qDebug() << "killing process:" << it.key();
        Q_ASSERT(kill(it.key(), SIGTERM) == 0);
        ++it;
    }
}

void Genesis::establishConnection()
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

void Genesis::readClientData()
{
    QLocalSocket *clientConnection = qobject_cast<QLocalSocket*>(sender());
    Q_ASSERT(clientConnection);

    QDataStream in(clientConnection);
    if (_blockSize == 0) {
        in >> _blockSize;
    }

    if (clientConnection->bytesAvailable() < _blockSize)
        return;

    qDebug() << "readClientData";

    char *data;
    in >> data;

#if 1
    compileAssembly(data);
#else
    diff(data);
#endif
}

void Genesis::diff(char *data)
{
    QFile file("out.s");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << data;
}

void Genesis::compileAssembly(char *data)
{
    const uint hash = qHash(data);
    const QString objectFile = QString("%1.o").arg(hash);
    const QString asCommand = QString("as --traditional-format -o %1").arg(objectFile);

    qDebug() << "assembling:" << asCommand;

    QStringList e = QProcess::systemEnvironment();
    QProcess *assembler = new QProcess(this);
    assembler->setEnvironment(e);
    assembler->start(asCommand);
    assembler->write(data);
    assembler->closeWriteChannel();
    Q_ASSERT(assembler->waitForFinished(-1));
    delete assembler;
    assembler = 0;

    QString libs = QLibraryInfo::location(QLibraryInfo::LibrariesPath);
    QString ldCommand = QString("g++ -Wl,-rpath,%1 -L%2 -lQtGui -lQtNetwork"
                                " -lQtCore -lpthread -o %3 %4")
                                .arg(libs).arg(libs).arg(hash).arg(objectFile);

    qDebug() << "linking:" << ldCommand;

    QProcess *linker = new QProcess(this);
    linker->setEnvironment(e);
    linker->start(ldCommand);
    Q_ASSERT(linker->waitForFinished(-1));
    delete linker;
    linker = 0;

    qDebug() << "remove object file:" << objectFile;
    QFile::remove(QCoreApplication::applicationDirPath() +
                  QDir::separator() + objectFile);

    spawn(QString::number(hash));
}

void Genesis::spawn(const QString &file)
{
    QString spCommand = QCoreApplication::applicationDirPath() +
                        QDir::separator() + file;

    qDebug() << "spawning:" << spCommand;

    qint64 pid;
    QStringList e = QProcess::systemEnvironment();
    QProcess *spawn = new QProcess(this);
    spawn->setEnvironment(e);
    Q_ASSERT(spawn->startDetached(spCommand, QStringList() /*args*/, QString() /*working directory*/, &pid));

    _processTable.insert(pid, QTime::currentTime());

    qDebug() << "spawn success pid:" << pid;

    delete spawn;
    spawn = 0;

    qDebug() << "remove executable:" << file;
    QFile::remove(spCommand);
}

void Genesis::reap()
{
    if (_processTable.isEmpty())
        return;

    qDebug() << "reap";

    //FIXME make this semi-random?
    QMutableHashIterator<qint64, QTime> it(_processTable);
    while (it.hasNext()) {
        it.next();
        if (qAbs(it.value().msecsTo(QTime::currentTime())) > 1000) {
            qDebug() << "killing process:" << it.key();
            Q_ASSERT(kill(it.key(), SIGTERM) == 0);
            _processTable.remove(it.key());
        }
    }
}
