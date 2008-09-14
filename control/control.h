#ifndef CONTROL_H
#define CONTROL_H

#include <QMainWindow>

#include "ui_control.h"

class QTimer;
class QProcess;
class QStandardItemModel;

class Control : public QMainWindow {
    Q_OBJECT
public:
    Control(QWidget *parent = 0L);
    virtual ~Control();

public Q_SLOTS:
    void start();
    void stop();

private Q_SLOTS:
    void startReaper();
    void stopReaper();

    void startGenesis();
    void stopGenesis();

    void refreshProcesses();

private:
    QTimer *_processesRefresh;
    QStandardItemModel *_processesModel;
    QProcess *_reaper;
    QProcess *_genesis;
    Ui::Control ui;
};

#endif
