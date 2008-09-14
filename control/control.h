#ifndef CONTROL_H
#define CONTROL_H

#include <QMainWindow>

class QProcess;

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

private:
    QProcess *_reaper;
    QProcess *_genesis;

    QAction *_actionStart;
    QAction *_actionStop;
};

#endif
