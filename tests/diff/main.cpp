#include <qtdiff.h>

#include <QtCore>
#include <iostream>

int main(int, char **)
{
    qDebug() << "Begin diff tests...";
    QLatin1String absoluteTestPath("/home/manyoso/dev/sforms/tests/diff");
    QStringList list = QDir(absoluteTestPath).entryList(QDir::AllDirs);
    foreach (QString dir, list) {
        QString absoluteDirPath = absoluteTestPath + QDir::separator() + dir + QDir::separator();
        QFile expected(absoluteDirPath + QLatin1String("expected"));
        if (!expected.exists() || !expected.open(QIODevice::ReadOnly | QIODevice::Text)) {
            continue;
        }

        QString original = absoluteDirPath + QLatin1String("original");
        QString modified = absoluteDirPath + QLatin1String("modified");

        QByteArray test = QByteArray::fromRawData(reinterpret_cast<const char*>(expected.map(0, expected.size())), expected.size());
        if (test == QtDiff::diff(original, modified)) {
            qDebug() << dir << "PASSED!";
        } else {
            qDebug() << dir << "FAILED!";
        }
    }
    qDebug() << "End diff tests!";

    return 0;
}
