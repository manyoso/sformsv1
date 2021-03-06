#ifndef QTDIFF_H
#define QTDIFF_H

class QString;
class QByteArray;

class QtDiff {
public:
    static QByteArray diff(const QString &a, const QString &b);
    static QByteArray diff(const QByteArray &a, const QByteArray &b);

private:
    QtDiff() {}
    ~QtDiff() {}
};

#endif
