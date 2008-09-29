#include <QCoreApplication>

#include <qtdiff.h>

#include <QDebug>

int main(int, char **)
{
    QString a("This part of the\n"
              "document has stayed the\n"
              "same from version to\n"
              "version.  It shouldn't\n"
              "be shown if it doesn't\n"
              "change.  Otherwise, that\n"
              "would not be helping to\n"
              "compress the size of the\n"
              "changes.\n"
              "\n"
              "This paragraph contains\n"
              "text that is outdated.\n"
              "It will be deleted in the\n"
              "near future.\n"
              "\n"
              "It is important to spell\n"
              "check this dokument. On\n"
              "the other hand, a\n"
              "misspelled word isn't\n"
              "the end of the world.\n"
              "Nothing in the rest of\n"
              "this paragraph needs to\n"
              "be changed. Things can\n"
              "be added after it.\n");

    QString b(
              "This is an important\n"
              "notice! It should\n"
              "therefore be located at\n"
              "the beginning of this\n"
              "document!\n"
              "\n"
              "This part of the\n"
              "document has stayed the\n"
              "same from version to\n"
              "version.  It shouldn't\n"
              "be shown if it doesn't\n"
              "change.  Otherwise, that\n"
              "would not be helping to\n"
              "compress anything.\n"
              "\n"
              "It is important to spell\n"
              "check this document. On\n"
              "the other hand, a\n"
              "misspelled word isn't\n"
              "the end of the world.\n"
              "Nothing in the rest of\n"
              "this paragraph needs to\n"
              "be changed. Things can\n"
              "be added after it.\n"
              "\n"
              "This paragraph contains\n"
              "important new additions\n"
              "to this document.\n");

    qDebug() << "diff:\n" << QtDiff::diff(a.toLatin1(), b.toLatin1());

    return 0;
}
