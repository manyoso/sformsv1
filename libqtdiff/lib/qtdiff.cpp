#include "qtdiff.h"

#include <QFile>
#include <QDebug>
#include <QByteArray>

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

QByteArray QtDiff::diff(const QString &a, const QString &b)
{
    QFile original(a);
    QFile modified(b);
    if (!original.exists() || !original.open(QIODevice::ReadOnly | QIODevice::Text))
        return QByteArray();
    if (!modified.exists() || !modified.open(QIODevice::ReadOnly | QIODevice::Text))
        return QByteArray();

    return diff(QByteArray::fromRawData(reinterpret_cast<const char*>(original.map(0, original.size())), original.size()),
                QByteArray::fromRawData(reinterpret_cast<const char*>(modified.map(0, modified.size())), modified.size()));
}

QByteArray QtDiff::diff(const QByteArray &a, const QByteArray &b)
{
    QList<QByteArray> aLines = a.split('\n');
    QList<QByteArray> bLines = b.split('\n');

    while (!aLines.isEmpty() && !bLines.isEmpty() && aLines.first() == bLines.first()) {
        aLines.removeFirst();
        bLines.removeFirst();
    }

    while (!aLines.isEmpty() && !bLines.isEmpty() && aLines.last() == bLines.last()) {
        aLines.removeLast();
        bLines.removeLast();
    }

    const int aSize = aLines.size();
    const int bSize = bLines.size();

    LCSLengths lengths(aSize, bSize);

    for (int i = aSize - 1; i >= 0; i--) {
        for (int j = bSize - 1; j >= 0; j--) {
            if (aLines.at(i) == bLines.at(j)) {
                lengths.setValue(i, j, 1 + lengths.value(i+1, j+1));
            } else {
                lengths.setValue(i, j, qMax(lengths.value(i+1, j), lengths.value(i, j+1)));
            }
        }
    }

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
        } else if (!exhaustedA && lengths.value(i+1, j) >= lengths.value(i, j+1)) {
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
    for (int i = 1; i < lines.count(); ++i) {
        DiffLine line = lines.at(i);

        switch (line.type)
        {
        case DiffLine::Hunk:
            HunkRange range;
            range.minusL = minusStart;
            range.minusS = minusRange;
            range.plusL = plusStart;
            range.plusS = plusRange;
            ranges << range;
            minusStart = 0;
            minusRange = 0;
            plusStart = 0;
            plusRange = 0;
            break;
        case DiffLine::Context:
            plusRange++;
            minusRange++;
            if (!minusStart) minusStart = line.indexA + 1;
            if (!plusStart) plusStart = line.indexB + 1;
            break;
        case DiffLine::Minus:
            minusRange++;
            if (!minusStart) minusStart = line.indexA + 1;
            break;
        case DiffLine::Plus:
            plusRange++;
            if (!plusStart) plusStart = line.indexB + 1;
            break;
        default:
            break;
        }
    }

    HunkRange range;
    range.minusL = minusStart;
    range.minusS = minusRange;
    range.plusL = plusStart;
    range.plusS = plusRange;
    ranges << range;

    QByteArray diff;

    diff.append(QLatin1String("--- original\n"));
    diff.append(QLatin1String("+++ modified\n"));

    for (int i = 0; i < lines.count(); ++i) {
        DiffLine line = lines.at(i);

        if (line.type == DiffLine::Hunk) {
            HunkRange range = ranges.takeFirst();
            QString hunk = line.line;
            hunk = hunk.arg(range.minusL).arg(range.minusS).arg(range.plusL).arg(range.plusS);
            line.line = hunk.toLatin1();
        }

        diff.append(line.line + '\n');
    }

    return diff;
}
