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
#if 1
    QString a("nematode knowledge");
    QString b("empty bottle");

    //emt ole is the solution...
#else
//     QString a("XMJYAUZ");
//     QString b("MZJAWXU");
//
//     //MJAU is the solution...

    QString a("abcdfghjqz");
    QString b("abcdefgijkrxyz");

    //abcdfgjz is the solution...
#endif

    qDebug() << "length" << lcs_length(a.toLatin1(), b.toLatin1());

    return 0;
}
