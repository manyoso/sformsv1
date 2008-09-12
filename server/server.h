#ifndef SERVER_H
#define SERVER_H

#include <QLocalServer>
#include <QLocalSocket>

class Server : public QLocalServer {
    Q_OBJECT
public:
    Server(QObject *parent = 0L);
    virtual ~Server();

private Q_SLOTS:
    void establishConnection();
    void readClientData();

    void diff(char *data);
    void compileAssembly(char *data);
    void spawn(const QString &path);

private:
    int _blockSize;
};

#endif
