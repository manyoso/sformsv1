#ifndef CONTROL_H
#define CONTROL_H

#include <QMainWindow>

class QProcess;

class Control : public QMainWindow {
    Q_OBJECT
public:
    Control(QWidget *parent = 0L);
    virtual ~Control();

private Q_SLOTS:
    void startReaper();
    void pauseReaper();
    void stopReaper();

    void startGenesis();
    void pauseGenesis();
    void stopGenesis();

private:
    QProcess *_reaper;
    QProcess *_genesis;

    QAction *_actionReaperStart;
    QAction *_actionReaperPause;
    QAction *_actionReaperStop;

    QAction *_actionGenesisStart;
    QAction *_actionGenesisPause;
    QAction *_actionGenesisStop;
};

#endif
