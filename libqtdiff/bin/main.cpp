#include <qtdiff.h>

#include <QtCore>
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 3)
        return -1;

    QString original = argv[1];
    QString modified = argv[2];

    std::cout << QtDiff::diff(original, modified).constData();

    return 0;
}
