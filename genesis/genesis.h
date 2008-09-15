#ifndef GENESIS_H
#define GENESIS_H

#include <QLocalServer>
#include <QLocalSocket>

class QTime;
class QTimer;
class QSocketNotifier;

class Genesis : public QLocalServer {
    Q_OBJECT
public:
    Genesis(QObject *parent = 0L);
    virtual ~Genesis();

private Q_SLOTS:
    void establishConnection();
    void readClientData();

    void compileSeed();
    void compileAssembly(const char *data, qint64 parent);
    void diff(const char *data, qint64 parent);
    void spawn(const QString &file, qint64 parent);
    void readStdin(int);

private:
    void logTree(qint64 parent, qint64 child);
    void logSpawn(const QStringList &spawn);

private:
    int _blockSize;
    QSocketNotifier *_stdinNotifier;
};

#endif
