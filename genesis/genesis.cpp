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
#include <QSocketNotifier>
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
    signal(SIGKILL, crashHandler);    /* 9:   kill */
    signal(SIGBUS,  crashHandler);    /* 10:  bus error */
    signal(SIGSEGV, crashHandler);    /* 11:  segmentation violation */
    signal(SIGSYS,  crashHandler);    /* 12:  bad argument to system call */
    signal(SIGPIPE, crashHandler);    /* 13:  write on a pipe with no reader */
    signal(SIGTERM, crashHandler);    /* 15:  terminate */
    signal(SIGXCPU, crashHandler);    /* 24:  exceeded CPU time limit */
    signal(SIGXFSZ, crashHandler);    /* 25:  exceeded file size limit */

    connect(this, SIGNAL(newConnection()),
            this, SLOT(establishConnection()));

    _stdinNotifier = new QSocketNotifier(0, QSocketNotifier::Read, this);
    connect(_stdinNotifier, SIGNAL(activated(int)),
            this, SLOT(readStdin(int)));

    compileSeed();
}

Genesis::~Genesis()
{
    close();
}

void Genesis::establishConnection()
{
//     qDebug() << "establishConnection";
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

    char *raw;
    in >> raw;
    qint64 pid;
    in >> pid;

    qDebug() << "genesis pid:\t" << pid;

    QString assembly = raw;
    QString data = assembly;
    data.replace('\"', QString('\\') + QString('\"'));
    data.replace('\n', QString('\\') + QString('n'));
    QString quine = QString('q') +
                    QString('u') +
                    QString('i') +
                    QString('n') +
                    QString('e');
    assembly.replace(quine, data);

#if 1
    compileAssembly(assembly.toLatin1().constData());
#else
    diff(assembly.toLatin1().constData());
#endif
}

void Genesis::compileSeed()
{
    QFile file("sform.s");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    compileAssembly(file.readAll().constData());
}

void Genesis::compileAssembly(const char *data)
{
    const uint hash = qHash(data);
    const QString objectFile = QString("%1.o").arg(hash);
    const QString asCommand = QString("as --traditional-format -o %1").arg(objectFile);

//     qDebug() << "assembling:" << asCommand;

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

//     qDebug() << "linking:" << ldCommand;

    QProcess *linker = new QProcess(this);
    linker->setEnvironment(e);
    linker->start(ldCommand);
    Q_ASSERT(linker->waitForFinished(-1));
    delete linker;
    linker = 0;

//     qDebug() << "remove object file:" << objectFile;
    QFile::remove(QCoreApplication::applicationDirPath() +
                  QDir::separator() + objectFile);

    spawn(QString::number(hash));
}

void Genesis::diff(const char *data)
{
    QFile file("out.s");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << data;
    out.flush();
    file.flush();
}

void Genesis::spawn(const QString &file)
{
    QFile f("sform-process.list");
    if (!f.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Can not open sform process list!";
        return; //don't spawn unless we can get a lock on the file
    }

    QString spCommand = QCoreApplication::applicationDirPath() +
                        QDir::separator() + file;

//     qDebug() << "spawning:" << spCommand;

    qint64 pid;
    QStringList e = QProcess::systemEnvironment();
    QProcess *spawn = new QProcess(this);
    spawn->setEnvironment(e);
    Q_ASSERT(spawn->startDetached(spCommand, QStringList() /*args*/, QString() /*working directory*/, &pid));

    QTextStream out(&f);
    out << QString("%1 %2\n").arg(QString::number(pid)).arg(QTime::currentTime().toString());
    out.flush();
    f.flush();
    f.close();

    qDebug() << "spawn pid:\t" << pid;

    delete spawn;
    spawn = 0;

//     qDebug() << "remove executable:" << file;
    QFile::remove(spCommand);

    logSpawn(QStringList() << QString::number(pid));
}

void Genesis::logSpawn(const QStringList &spawn)
{
    QFile file("sform-spawn.list");
    Q_ASSERT(file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text));

    QTextStream out(&file);
    out << spawn.join(QChar('\n'));
    out << QChar('\n');
    out.flush();
    file.flush();
    file.close();
}

void Genesis::readStdin(int i)
{
    Q_UNUSED(i);
    QString command;
    QTextStream in(stdin);
    in >> command;
    if (command == "stop")
        QTimer::singleShot(0, QCoreApplication::instance(), SLOT(quit()));
}
