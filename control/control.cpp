#include "control.h"

#include <QDir>
#include <QDebug>
#include <QTimer>
#include <QAction>
#include <QProcess>
#include <QGroupBox>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QCoreApplication>
#include <QStandardItemModel>

Control::Control(QWidget *parent)
    : QMainWindow(parent),
      _reaper(0),
      _genesis(0)
{
    ui.setupUi(this);

    setWindowTitle(tr("SForm Control Center"));

    _modelRefresh = new QTimer(this);
    connect(_modelRefresh, SIGNAL(timeout()),
            this, SLOT(refreshModels()));

    _processesModel = new QStandardItemModel(this);
    ui.processesTableView->setModel(_processesModel);

    _inheritanceModel = new QStandardItemModel(this);
    ui.inheritanceTreeView->setModel(_inheritanceModel);

    connect(ui.actionStart, SIGNAL(triggered()), this, SLOT(start()));
    connect(ui.actionStop, SIGNAL(triggered()), this, SLOT(stop()));
    ui.actionStop->setEnabled(false);
}

Control::~Control()
{
    stop();
}

void Control::start()
{
    startReaper();
    startGenesis();
    ui.actionStart->setEnabled(false);
    ui.actionStop->setEnabled(true);
    _modelRefresh->start(1000);
}

void Control::stop()
{
    stopGenesis();
    stopReaper();
    ui.actionStart->setEnabled(true);
    ui.actionStop->setEnabled(false);
    _modelRefresh->stop();
}

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

void Control::refreshModels()
{
    refreshProcessesModel();
    refreshInheritanceModel();
}

void Control::refreshProcessesModel()
{
    QString filePath = QCoreApplication::applicationDirPath() +
                       QDir::separator() + QLatin1String("sform-process.list");
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Can not open process list!";
        return;
    }

    QStringList header;
    header << QString("PID");
    header << QString("TIME");

    _processesModel->clear();
    _processesModel->setHorizontalHeaderLabels(header);

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QList<QByteArray> info = line.split(' ');
        if (info.count() != 2)
            continue;
        QList<QStandardItem *> row;
        QStandardItem *pid = new QStandardItem(QString(info[0]).trimmed());
        QStandardItem *time = new QStandardItem(QString(info[1]).trimmed());
        row << pid;
        row << time;
        _processesModel->appendRow(row);
    }
}

void Control::refreshInheritanceModel()
{
    QString filePath = QCoreApplication::applicationDirPath() +
                       QDir::separator() + QLatin1String("sform-tree.list");
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Can not open tree list!";
        return;
    }

    _inheritanceModel->clear();

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QList<QByteArray> info = line.split(' ');
        if (info.count() != 2)
            continue;

        QString parentPid = info[0].trimmed();
        QString childPid = info[1].trimmed();

        QStandardItem *parent = 0;
        if (parentPid.toInt() == -1) {
            parent = _inheritanceModel->invisibleRootItem();
        } else {
            QList<QStandardItem*> items = _inheritanceModel->findItems(parentPid, Qt::MatchExactly | Qt::MatchRecursive);
            if (!items.isEmpty())
                parent = items.first();
        }

        if (parent)
            parent->appendRow(new QStandardItem(childPid));
    }
}
