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

    _actionReaperPause = new QAction(tr("&Pause"), this);
    connect(_actionReaperPause, SIGNAL(triggered()), this, SLOT(pauseReaper()));

    _actionReaperStop = new QAction(tr("&Stop"), this);
    connect(_actionReaperStop, SIGNAL(triggered()), this, SLOT(stopReaper()));

    _actionGenesisStart = new QAction(tr("&Start"), this);
    connect(_actionGenesisStart, SIGNAL(triggered()), this, SLOT(startGenesis()));

    _actionGenesisPause = new QAction(tr("&Pause"), this);
    connect(_actionGenesisPause, SIGNAL(triggered()), this, SLOT(pauseGenesis()));

    _actionGenesisStop = new QAction(tr("&Stop"), this);
    connect(_actionGenesisStop, SIGNAL(triggered()), this, SLOT(stopGenesis()));

    QWidget *centralWidget = new QWidget(this);

    QGroupBox *reaperWidget = new QGroupBox(tr("Reaper Controls"), centralWidget);

    QToolButton *reaperStart = new QToolButton(reaperWidget);
    reaperStart->setDefaultAction(_actionReaperStart);
    QToolButton *reaperPause = new QToolButton(reaperWidget);
    reaperPause->setDefaultAction(_actionReaperPause);
    QToolButton *reaperStop = new QToolButton(reaperWidget);
    reaperStop->setDefaultAction(_actionReaperStop);

    QHBoxLayout *reaperLayout = new QHBoxLayout(reaperWidget);
    reaperLayout->addWidget(reaperStart);
    reaperLayout->addWidget(reaperPause);
    reaperLayout->addWidget(reaperStop);
    reaperWidget->setLayout(reaperLayout);

    QGroupBox *genesisWidget = new QGroupBox(tr("Genesis Controls"), centralWidget);

    QToolButton *genesisStart = new QToolButton(genesisWidget);
    genesisStart->setDefaultAction(_actionGenesisStart);
    QToolButton *genesisPause = new QToolButton(genesisWidget);
    genesisPause->setDefaultAction(_actionGenesisPause);
    QToolButton *genesisStop = new QToolButton(genesisWidget);
    genesisStop->setDefaultAction(_actionGenesisStop);

    QHBoxLayout *genesisLayout = new QHBoxLayout(genesisWidget);
    genesisLayout->addWidget(genesisStart);
    genesisLayout->addWidget(genesisPause);
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
        _reaper->setProcessChannelMode(QProcess::ForwardedChannels);
        _reaper->start(command);
    }

    qDebug() << "startReaper";
}

void Control::pauseReaper()
{
    if (!_reaper)
        return;

    qDebug() << "pauseReaper";
    //FIXME call it to pause
}

void Control::stopReaper()
{
    if (!_reaper)
        return;

    qDebug() << "stopReaper";
    //FIXME call it to clean up
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
        _genesis->setProcessChannelMode(QProcess::ForwardedChannels);
        _genesis->start(command);
    }

    qDebug() << "startGenesis";
}

void Control::pauseGenesis()
{
    if (!_genesis)
        return;

    qDebug() << "pauseGenesis";
    //FIXME call it to pause
}

void Control::stopGenesis()
{
    if (!_genesis)
        return;

    qDebug() << "stopGenesis";
    //FIXME call it to clean up
    _genesis->kill();
    delete _genesis;
    _genesis = 0;
}
