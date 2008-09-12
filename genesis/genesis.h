#ifndef GENESIS_H
#define GENESIS_H

#include <QLocalServer>
#include <QLocalSocket>

class QTime;
class QTimer;

class Genesis : public QLocalServer {
    Q_OBJECT
public:
    Genesis(QObject *parent = 0L);
    virtual ~Genesis();

private Q_SLOTS:
    void establishConnection();
    void readClientData();

    void diff(char *data);
    void compileAssembly(char *data);
    void spawn(const QString &file);
    void reap();

private:
    int _blockSize;
    QTimer *_reaper;
    QHash<qint64, QTime> _processTable;
};

#endif
