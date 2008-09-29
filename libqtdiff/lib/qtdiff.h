#ifndef QTDIFF_H
#define QTDIFF_H

class QByteArray;

class QtDiff {
public:
    static QByteArray diff(const QByteArray &a, const QByteArray &b);

private:
    QtDiff() {}
    ~QtDiff() {}
};

#endif
