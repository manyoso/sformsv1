#include <QCoreApplication>

#include <QDebug>

class LCSLengths {
public:
    LCSLengths(int m, int n)
    {
        m_m = m;
        m_n = n;
        m_vector.resize(m_m * m_n);
    }
    ~LCSLengths() {}

    int value(int i, int j)
    {
        int index = i * m_n + j;
        if (index > m_vector.count() || i >= m_m || j >= m_n)
            return 0;
        else
            return m_vector.at(index);
    }
    void setValue(int i, int j, int value)
    {
        int index = i * m_n + j;
        if (index > m_vector.count() || i >= m_m || j >= m_n)
            return;

        m_vector.replace(index, value);
    }
    void print()
    {
        for (int j = 0; j <= m_n; ++j) {
            for (int i = 0; i <= m_m; ++i) {
              printf("%d ", value(i, j));
              if (i == m_m)
                  printf("\n");
            }
        }
    }

private:
    int m_m;
    int m_n;
    QVector<int> m_vector;
};

int lcs_lengthLines(const QByteArray &a, const QByteArray &b)
{
    QList<QByteArray> aLines = a.split('\n');
    QList<QByteArray> bLines = b.split('\n');
    const int aSize = aLines.size();
    const int bSize = bLines.size();

    LCSLengths l(aSize, bSize);

    for (int i = aSize - 1; i >= 0; i--) {
        for (int j = bSize - 1; j >= 0; j--) {
            if (aLines.at(i) == bLines.at(j)) {
                l.setValue(i, j, 1 + l.value(i+1, j+1));
            } else {
                l.setValue(i, j, qMax(l.value(i+1, j), l.value(i, j+1)));
            }
        }
    }

    //l.print();

    const int length = l.value(0, 0);

    QByteArray diff;
    int i = 0;
    int j = 0;
    while (i < aSize && j < bSize) {
        if (aLines.at(i) == bLines.at(j)) {
            diff.append(" " + aLines.at(i) + "\n");
            i++; j++;
        } else if (l.value(i+1, j) >= l.value(i, j+1)) {
            diff.append("-" + aLines.at(i) + "\n");
            i++;
        } else {
            diff.append("+" + bLines.at(j) + "\n");
            j++;
        }
    }

    qDebug() << diff;

    return length;
}

int lcs_length(const QByteArray &a, const QByteArray &b)
{
    const int aSize = a.size();
    const int bSize = b.size();

    LCSLengths l(aSize, bSize);

    for (int i = aSize - 1; i >= 0; i--) {
        for (int j = bSize - 1; j >= 0; j--) {
            if (a.at(i) == b.at(j)) {
                l.setValue(i, j, 1 + l.value(i+1, j+1));
            } else {
                l.setValue(i, j, qMax(l.value(i+1, j), l.value(i, j+1)));
            }
        }
    }

    //l.print();

    const int length = l.value(0, 0);

    QByteArray longestSubSequence;
    int i = 0;
    int j = 0;
    while (i < aSize && j < bSize) {
        if (a.at(i)==b.at(j)) {
            longestSubSequence.append(a.at(i));
            i++; j++;
        } else if (l.value(i+1, j) >= l.value(i, j+1)) {
            i++;
        } else {
            j++;
        }
    }

    qDebug() << "longestSubSequence" << longestSubSequence;

    return length;
}

int main(int, char **)
{
#if 0
    QString a("nematode knowledge");
    QString b("empty bottle");

    //emt ole is the solution...
    qDebug() << "length" << lcs_length(a.toLatin1(), b.toLatin1());
#else

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

    QString b("This is an important\n"
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

    qDebug() << "length" << lcs_lengthLines(a.toLatin1(), b.toLatin1());
#endif

    return 0;
}
