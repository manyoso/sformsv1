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

private:
    void logSpawn(const QStringList &spawn);

private:
    int _blockSize;
};

#endif
