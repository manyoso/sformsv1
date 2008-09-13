#include "control.h"

#include <QDir>
#include <QDebug>
#include <QAction>
#include <QProcess>
#include <QGroupBox>
#include <QMessageBox>
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
    _actionReaperStop->setEnabled(false);
    connect(_actionReaperStop, SIGNAL(triggered()), this, SLOT(stopReaper()));

    _actionGenesisStart = new QAction(tr("&Start"), this);
    connect(_actionGenesisStart, SIGNAL(triggered()), this, SLOT(startGenesis()));

    _actionGenesisStop = new QAction(tr("&Stop"), this);
    _actionGenesisStop->setEnabled(false);
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
    if (_reaper)
        return;

    qDebug() << "startReaper";
    QString command = QCoreApplication::applicationDirPath() +
                      QDir::separator() + QLatin1String("reaper");
    _reaper = new QProcess(this);
    _reaper->setWorkingDirectory(QCoreApplication::applicationDirPath());
    _reaper->setProcessChannelMode(QProcess::ForwardedChannels);
    _reaper->start(command);

    _actionReaperStart->setEnabled(false);
    _actionReaperStop->setEnabled(true);
}

void Control::stopReaper()
{
    if (!_reaper)
        return;

    if (_genesis) {
        QMessageBox::StandardButton button = QMessageBox::question(this,
            tr("Stop Reaper?"),
            tr("<b>Warning:</b> Genesis is still running! <p>Stopping Reaper"
                " with Genesis still running can lead to runaway SForm reproduction"
                " and process creation. This can crash the computer.</p>"
                " <p>Really stop Reaper anyway?</p>"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (button != QMessageBox::Yes) {
            return;
        }
    }

    qDebug() << "stopReaper";
    _reaper->write("stop");
    _reaper->closeWriteChannel();
    _reaper->waitForFinished(2000); //give it a chance to exit gracefully
    _reaper->kill();
    delete _reaper;
    _reaper = 0;

    _actionReaperStart->setEnabled(true);
    _actionReaperStop->setEnabled(false);
}

void Control::startGenesis()
{
    if (_genesis)
        return;

    if (!_reaper) {
        QMessageBox::StandardButton button = QMessageBox::question(this,
            tr("Start Genesis?"),
            tr("<b>Warning:</b> Reaper is not running! <p>Starting Genesis"
                " without Reaper running can lead to runaway SForm reproduction"
                " and process creation. This can crash the computer.</p>"
                " <p>Really start Genesis anyway?</p>"),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if (button != QMessageBox::Yes) {
            return;
        }
    }

    qDebug() << "startGenesis";
    QString command = QCoreApplication::applicationDirPath() +
                      QDir::separator() + QLatin1String("genesis");
    _genesis = new QProcess(this);
    _genesis->setWorkingDirectory(QCoreApplication::applicationDirPath());
    _genesis->setProcessChannelMode(QProcess::ForwardedChannels);
    _genesis->start(command);

    _actionGenesisStart->setEnabled(false);
    _actionGenesisStop->setEnabled(true);
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

    _actionGenesisStart->setEnabled(true);
    _actionGenesisStop->setEnabled(false);
}
