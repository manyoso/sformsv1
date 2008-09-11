#ifndef SFORM_H
#define SFORM_H

#include <QtCore>
#include <QtNetwork>

class SForm : public QObject {
public:
    SForm(QObject *parent = 0L);
    virtual ~SForm();

private:
    void reproduce();

private:
    uint m_numChildren;
    QLocalSocket *m_socket;
};

#endif
