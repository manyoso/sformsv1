#ifndef FILEPREPROCESSOR_H
#define FILEPREPROCESSOR_H

#include <QStringList>

#include "preprocessor.h"

class FilePreprocessor : public rpp::Preprocessor {
public:
  FilePreprocessor();
  virtual ~FilePreprocessor();

  virtual rpp::Stream* sourceNeeded(QString& fileName, IncludeType type, int sourceLine);

private:
  QStringList _includeDirs;
};

#endif
