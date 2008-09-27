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

struct HunkRange {
    int plusL;
    int plusS;
    int minusL;
    int minusS;
};

struct DiffLine {
    enum Type { Context, Minus, Plus, Hunk };
    Type type;
    int indexA;
    int indexB;
    QByteArray line;
};

QByteArray diff(const QByteArray &a, const QByteArray &b)
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

    QList<DiffLine> lines;
    int i = 0;
    int j = 0;
    while (i < aSize || j < bSize) {
        DiffLine line;
        bool exhaustedA = i >= aSize;
        bool exhaustedB = j >= bSize;
        QByteArray a = !exhaustedA ? aLines.at(i) : QByteArray();
        QByteArray b = !exhaustedB ? bLines.at(j) : QByteArray();
        if (!exhaustedA && !exhaustedB && a == b) {
            line.type = DiffLine::Context;
            line.indexA = i;
            line.indexB = j;
            line.line = " " + a;
            i++; j++;
        } else if (!exhaustedA && l.value(i+1, j) >= l.value(i, j+1)) {
            line.type = DiffLine::Minus;
            line.indexA = i;
            line.indexB = -1;
            line.line = "-" + a;
            i++;
        } else if (!exhaustedB) {
            line.type = DiffLine::Plus;
            line.indexA = -1;
            line.indexB = j;
            line.line = "+" + b;
            j++;
        }
        lines << line;
    }

    DiffLine hunk;
    hunk.type = DiffLine::Hunk;
    hunk.indexA = -1;
    hunk.indexB = -1;
    hunk.line = QString("@@ -%1,%2 +%3,%4 @@").toLatin1();
    lines.insert(0, hunk);

    int contextCount = 0;
    for (int i = 0; i < lines.count(); ++i) {
        DiffLine line = lines.at(i);

        if (line.type == DiffLine::Context)
            contextCount++;
        else
            contextCount = 0;

        if (contextCount == 6)
            lines.insert(i - 2, hunk);

        if (contextCount > 6)
            lines.removeAt(i - 2);
    }

    QList<HunkRange> ranges;
    int minusStart = 0;
    int minusRange = 0;
    int plusStart = 0;
    int plusRange = 0;
    bool firstMinus = false;
    bool firstPlus = false;
    for (int i = 1; i < lines.count(); ++i) {
        DiffLine line = lines.at(i);

        switch (line.type)
        {
        case DiffLine::Hunk:
            HunkRange range;
            range.minusL = minusStart + 1;
            range.minusS = minusRange;
            range.plusL = plusStart + 1;
            range.plusS = plusRange;
            ranges << range;
            minusStart = 0;
            minusRange = 0;
            plusStart = 0;
            plusRange = 0;
            firstMinus = true;
            firstPlus = true;
            break;
        case DiffLine::Context:
            plusRange++;
            minusRange++;
            if (firstMinus) {
                minusStart = line.indexA;
                firstMinus = false;
            }
            if (firstPlus) {
                plusStart = line.indexB;
                firstPlus = false;
            }
            break;
        case DiffLine::Minus:
            minusRange++;
            if (firstMinus) {
                minusStart = line.indexA;
                firstMinus = false;
            }
            break;
        case DiffLine::Plus:
            plusRange++;
            if (firstPlus) {
                plusStart = line.indexB;
                firstPlus = false;
            }
            break;
        default:
            break;
        }
    }

    HunkRange range;
    range.minusL = minusStart + 1;
    range.minusS = minusRange;
    range.plusL = plusStart + 1;
    range.plusS = plusRange;
    ranges << range;

    QByteArray diff;
    for (int i = 0; i < lines.count(); ++i) {
        DiffLine line = lines.at(i);

        if (line.type == DiffLine::Hunk) {
            HunkRange range = ranges.takeFirst();
            QString hunk = line.line;
            hunk = hunk.arg(range.minusL).arg(range.minusS).arg(range.plusL).arg(range.plusS);
            line.line = hunk.toLatin1();
        }

/*        qDebug() << line.type
                 << line.indexA
                 << line.indexB
                 << line.line;*/
        diff.append(line.line + "\n");
    }

    return diff;
}

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
              "be added after it.");

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
              "to this document.");

    qDebug() << "diff:\n" << diff(a.toLatin1(), b.toLatin1());

    return 0;
}
