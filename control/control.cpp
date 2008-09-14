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
    setWindowTitle(tr("SForm Control Center"));

    _actionStart = new QAction(tr("&Start"), this);
    connect(_actionStart, SIGNAL(triggered()), this, SLOT(start()));

    _actionStop = new QAction(tr("&Stop"), this);
    _actionStop->setEnabled(false);
    connect(_actionStop, SIGNAL(triggered()), this, SLOT(stop()));

    QWidget *centralWidget = new QWidget(this);

    QGroupBox *controlWidget = new QGroupBox(tr("SForm Controls"), centralWidget);

    QToolButton *start = new QToolButton(controlWidget);
    start->setDefaultAction(_actionStart);
    QToolButton *stop = new QToolButton(controlWidget);
    stop->setDefaultAction(_actionStop);

    QHBoxLayout *controlLayout = new QHBoxLayout(controlWidget);
    controlLayout->addWidget(start);
    controlLayout->addWidget(stop);
    controlWidget->setLayout(controlLayout);

    QHBoxLayout *centralLayout = new QHBoxLayout(centralWidget);
    centralLayout->addWidget(controlWidget);
    centralWidget->setLayout(centralLayout);
    setCentralWidget(centralWidget);
}

Control::~Control()
{
    stop();
}

void Control::start()
{
    startReaper();
    startGenesis();
    _actionStart->setEnabled(false);
    _actionStop->setEnabled(true);
}

void Control::stop()
{
    stopGenesis();
    stopReaper();
    _actionStart->setEnabled(true);
    _actionStop->setEnabled(false);}

void Control::startReaper()
{
    if (_reaper)
        return;

//     qDebug() << "startReaper";
    QString command = QCoreApplication::applicationDirPath() +
                      QDir::separator() + QLatin1String("reaper");
    _reaper = new QProcess(this);
    _reaper->setWorkingDirectory(QCoreApplication::applicationDirPath());
    _reaper->setProcessChannelMode(QProcess::ForwardedChannels);
    _reaper->start(command);
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

//     qDebug() << "stopReaper";
    _reaper->write("stop");
    _reaper->closeWriteChannel();
    _reaper->waitForFinished(2000); //give it a chance to exit gracefully
    _reaper->kill();
    delete _reaper;
    _reaper = 0;
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

//     qDebug() << "startGenesis";
    QString command = QCoreApplication::applicationDirPath() +
                      QDir::separator() + QLatin1String("genesis");
    _genesis = new QProcess(this);
    _genesis->setWorkingDirectory(QCoreApplication::applicationDirPath());
    _genesis->setProcessChannelMode(QProcess::ForwardedChannels);
    _genesis->start(command);
}

void Control::stopGenesis()
{
    if (!_genesis)
        return;

//     qDebug() << "stopGenesis";
    _genesis->write("stop");
    _genesis->closeWriteChannel();
    _genesis->waitForFinished(2000); //give it a chance to exit gracefully
    _genesis->kill();
    delete _genesis;
    _genesis = 0;
}
