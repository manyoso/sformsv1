#include "control.h"

#include <QDir>
#include <QDebug>
#include <QAction>
#include <QProcess>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QCoreApplication>

Control::Control(QWidget *parent)
    : QMainWindow(parent),
      _reaper(0),
      _genesis(0)
{
    setWindowTitle(tr("Sform Control Center"));

    _actionReaperStart = new QAction(tr("&Start"), this);
    connect(_actionReaperStart, SIGNAL(triggered()), this, SLOT(startReaper()));

    _actionReaperStop = new QAction(tr("&Stop"), this);
    connect(_actionReaperStop, SIGNAL(triggered()), this, SLOT(stopReaper()));

    _actionGenesisStart = new QAction(tr("&Start"), this);
    connect(_actionGenesisStart, SIGNAL(triggered()), this, SLOT(startGenesis()));

    _actionGenesisStop = new QAction(tr("&Stop"), this);
    connect(_actionGenesisStop, SIGNAL(triggered()), this, SLOT(stopGenesis()));

    QWidget *centralWidget = new QWidget(this);

    QGroupBox *reaperWidget = new QGroupBox(tr("Reaper Controls"), centralWidget);

    QToolButton *reaperStart = new QToolButton(reaperWidget);
    reaperStart->setDefaultAction(_actionReaperStart);
    QToolButton *reaperStop = new QToolButton(reaperWidget);
    reaperStop->setDefaultAction(_actionReaperStop);

    QHBoxLayout *reaperLayout = new QHBoxLayout(reaperWidget);
    reaperLayout->addWidget(reaperStart);
    reaperLayout->addWidget(reaperStop);
    reaperWidget->setLayout(reaperLayout);

    QGroupBox *genesisWidget = new QGroupBox(tr("Genesis Controls"), centralWidget);

    QToolButton *genesisStart = new QToolButton(genesisWidget);
    genesisStart->setDefaultAction(_actionGenesisStart);
    QToolButton *genesisStop = new QToolButton(genesisWidget);
    genesisStop->setDefaultAction(_actionGenesisStop);

    QHBoxLayout *genesisLayout = new QHBoxLayout(genesisWidget);
    genesisLayout->addWidget(genesisStart);
    genesisLayout->addWidget(genesisStop);
    genesisWidget->setLayout(genesisLayout);

    QHBoxLayout *centralLayout = new QHBoxLayout(centralWidget);
    centralLayout->addWidget(reaperWidget);
    centralLayout->addWidget(genesisWidget);
    centralWidget->setLayout(centralLayout);
    setCentralWidget(centralWidget);
}

Control::~Control()
{
    stopReaper();
    stopGenesis();
}

void Control::startReaper()
{
    if (!_reaper) {
        QString command = QCoreApplication::applicationDirPath() +
                          QDir::separator() + QLatin1String("reaper");
        _reaper = new QProcess(this);
        _reaper->setWorkingDirectory(QCoreApplication::applicationDirPath());
        _reaper->setProcessChannelMode(QProcess::ForwardedChannels);
        _reaper->start(command);
    }

    qDebug() << "startReaper";
}

void Control::stopReaper()
{
    if (!_reaper)
        return;

    qDebug() << "stopReaper";
    _reaper->write("stop");
    _reaper->closeWriteChannel();
    _reaper->waitForFinished(2000); //give it a chance to exit gracefully
    _reaper->kill();
    delete _reaper;
    _reaper = 0;
}

void Control::startGenesis()
{
    if (!_genesis) {
        QString command = QCoreApplication::applicationDirPath() +
                          QDir::separator() + QLatin1String("genesis");
        _genesis = new QProcess(this);
        _genesis->setWorkingDirectory(QCoreApplication::applicationDirPath());
        _genesis->setProcessChannelMode(QProcess::ForwardedChannels);
        _genesis->start(command);
    }

    qDebug() << "startGenesis";
}

void Control::stopGenesis()
{
    if (!_genesis)
        return;

    qDebug() << "stopGenesis";
    _genesis->write("stop");
    _genesis->closeWriteChannel();
    _genesis->waitForFinished(2000); //give it a chance to exit gracefully
    _genesis->kill();
    delete _genesis;
    _genesis = 0;
}
