#ifndef SFORM_H
#define SFORM_H

#include <QtCore>

class Code {
public:
    enum FileType {
        Source = 1,
        Header = 2,
        Project = 4,
        Resource = 8,
        All = Source | Header | Project | Resource,
    };

  Q_DECLARE_FLAGS(FileTypes, FileType)

  static QStringList files(FileTypes t = All );

private:
    Code() {}
    virtual ~Code() {}
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Code::FileTypes)

class SForm : public QObject {
    Q_OBJECT
public:
    SForm(QObject *parent = 0L);
    virtual ~SForm();

private slots:
    void reproduce();

private:
    uint _numChildren;
};

#endif
