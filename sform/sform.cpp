#include "sform.h"

#include <iostream>

class CodeResource : public QResource {
public:
    CodeResource() {
        setFileName(":/code");
    }
    virtual ~CodeResource() {}

    QStringList files() const {
        return children();
    }
};

static const CodeResource _resource = CodeResource();

QStringList Code::files(FileTypes t) {

    QStringList filter;
    QStringList files = _resource.files();
    files.replaceInStrings(QRegExp("^(.*)$"), _resource.absoluteFilePath() + "/\\1");

    if (t & Source) {
        filter += files.filter(".cpp");
    }

    if (t & Header) {
        filter += files.filter(".h");
    }

    if (t & Project) {
        filter += files.filter(".pro");
        filter += files.filter(".pri");
    }

    if (t & Resource) {
        filter += files.filter(".qrc");
    }

    return filter;
}

SForm::SForm(QObject *parent)
    : QObject(parent),
    _numChildren(0) {

    reproduce();
}

SForm::~SForm() {}

void SForm::reproduce() {
    //1. Copy all resources out to disk in new directory

    QDir current(QCoreApplication::applicationDirPath());
    Q_ASSERT(current.exists());

    const QString name = QString::number(getpid());
    current.mkdir(name);

    QDir childrensDir(current.absolutePath() + QDir::separator() + name);
    Q_ASSERT(childrensDir.exists());

    const QString childName = QString::number(_numChildren++);
    childrensDir.mkdir(childName);

    qDebug() << name << "reproducing child" << childName;

    QDir childDir(childrensDir.absolutePath() + QDir::separator() + childName);
    Q_ASSERT(childDir.exists());

    QStringList files = Code::files();
    QStringList::ConstIterator it = files.begin();
    for (; it != files.end(); ++it) {
        QFile f((*it));
        Q_ASSERT(f.exists());
        Q_ASSERT(f.copy(childDir.absolutePath() + QDir::separator() + QFileInfo(f).fileName()));
    }

    //2. Compile them into new executable

    QStringList e = QProcess::systemEnvironment();

    QProcess *qmake = new QProcess(this);
    qmake->setEnvironment(e);
    qmake->setWorkingDirectory(childDir.absolutePath());
//     qmake->setProcessChannelMode(QProcess::ForwardedChannels);
    qmake->start("qmake");
    Q_ASSERT(qmake->waitForFinished(-1));
    delete qmake;
    qmake = 0;

    QProcess *make = new QProcess(this);
    make->setEnvironment(e);
    make->setWorkingDirectory(childDir.absolutePath());
//     make->setProcessChannelMode(QProcess::ForwardedChannels);
    make->start("make");
    Q_ASSERT(make->waitForFinished(-1));
    delete make;
    make = 0;

    //3. Ask server to execute them? or execute ourself and fork?
    QProcess *child = new QProcess(this);
    child->setEnvironment(e);
    child->setWorkingDirectory(childDir.absolutePath());
    child->setProcessChannelMode(QProcess::ForwardedChannels);
    child->start("./sform");

    reproduce();
}

int main(int argc, char **argv) {
    QCoreApplication *a = new QCoreApplication(argc, argv);
    QCoreApplication::setOrganizationDomain("treat.org");
    QCoreApplication::setApplicationName("sform");

    SForm sform;

    int rc = a->exec();

    return rc;
}
