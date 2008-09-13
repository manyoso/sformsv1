#ifndef REAPER_H
#define REAPER_H

#include <QObject>

class QTime;
class QTimer;

class Reaper : public QObject {
    Q_OBJECT
public:
    Reaper(QObject *parent = 0L);
    virtual ~Reaper();

private Q_SLOTS:
    void reap();

private:
    QTimer *_reaper;
};

#endif
