#include "reaper.h"

#include <QFile>
#include <QTime>
#include <QTimer>
#include <QDebug>
#include <QStringList>

#include <signal.h>

Reaper::Reaper(QObject *parent)
    : QObject(parent)
{
    _reaper = new QTimer(this);
    _reaper->start(1000); //every second
    connect(_reaper, SIGNAL(timeout()), this, SLOT(reap()));
}

Reaper::~Reaper()
{
}

void Reaper::reap()
{
    QFile file("sform-process.list");
    if (!file.exists())
        return;

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return;

    qDebug() << "reap";

    QStringList spared;
    QStringList killed;
    QList<QByteArray> sforms = file.readAll().split('\n');
    foreach (QString sform, sforms) {
        QStringList info = sform.split(' ');
        if (info.count() != 2)
            continue;

        qint64 pid = info[0].toLong();
        QTime time = QTime::fromString(info[1]);

        if (qAbs(time.msecsTo(QTime::currentTime())) > 1000) {
            qDebug() << "killing process:" << pid;
            kill(pid, SIGKILL);
            killed << QString::number(pid);
        } else {
            spared << sform;
        }
    }

    file.resize(0); //truncates what is currently in the file
    file.flush();

    QTextStream out(&file);
    out << spared.join(QChar('\n'));
    out << QChar('\n');
    out.flush();
    file.flush();
    file.close();

    logKill(killed);
}

void Reaper::logKill(const QStringList &killed)
{
    if (killed.isEmpty())
        return;

    QFile file("sform-killed.list");
    Q_ASSERT(file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text));

    QTextStream out(&file);
    out << killed.join(QChar('\n'));
    out << QChar('\n');
    out.flush();
    file.flush();
    file.close();
}