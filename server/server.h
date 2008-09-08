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

    void compileAssembly(char *data);

private:
    int _blockSize;
};

#endif
