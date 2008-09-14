#ifndef SFORM_H
#define SFORM_H

#include <QtCore>
#include <QtNetwork>

class SForm : public QObject {
public:
    SForm(QObject *parent);
    virtual ~SForm();

private:
    void reproduce();

private:
    QLocalSocket *m_socket;
};

#endif
