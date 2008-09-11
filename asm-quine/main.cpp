#include <QCoreApplication>

#include <QFile>
#include <QDebug>

int main(int argc, char **argv)
{
    if (argc < 1)
        return 0;

    QFile file(argv[1]);

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
        return 0;

    QString assembly = file.readAll();

    file.close();

    QString data = assembly;
    data.replace('\"', QString('\\') + QString('\"'));
    data.replace('\n', QString('\\') + QString('n'));
    QString quine = QString('q') +
                    QString('u') +
                    QString('i') +
                    QString('n') +
                    QString('e');
    assembly.replace(quine, data);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return 0;

    QTextStream out(&file);
    out << assembly;

    return 0;
}
