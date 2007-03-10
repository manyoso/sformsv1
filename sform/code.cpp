#include "code.h"

#include <QResource>

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
