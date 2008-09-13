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
    QList<QByteArray> sforms = file.readAll().split('\n');
    foreach (QString sform, sforms) {
        QStringList info = sform.split(' ');
        if (info.count() != 2)
            continue;
        qint64 pid = info[0].toLong();
        QTime time = QTime::fromString(info[1]);

        if (qAbs(time.msecsTo(QTime::currentTime())) > 1000) {
            qDebug() << "killing process:" << pid;
            Q_ASSERT(kill(pid, SIGTERM) == 0);
        } else {
            spared << sform;
        }
    }

    file.resize(0); //truncates what is currently in the file

    QTextStream out(&file);
    out << spared.join(QChar('\n'));
}
