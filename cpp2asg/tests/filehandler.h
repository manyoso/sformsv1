#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <QUrl>
#include <QString>
#include <QByteArray>

#include "cursor.h"

#include "parser.h"
#include "control.h"

#include "typerepository.h"
#include "identifier.h"
#include "dumpchain.h"

class Declaration;
class TopDUContext;

class FileHandler : public QObject {
  Q_OBJECT
public:
  enum DumpArea {
    DumpNone = 0,
    DumpAST = 1,
    DumpDUChain = 2,
    DumpType = 4
  };
  Q_DECLARE_FLAGS(DumpAreas, DumpArea)

  FileHandler();
  virtual ~FileHandler();

  void handleFile(const QString &fileName);

private:
  DUContext* parse(const QUrl &url,
                   ParseSession *session,
                   DumpAreas dump = static_cast<DumpAreas>(DumpAST | DumpDUChain | DumpType));
  void release(DUContext* top);

  // Parser
  Control control;
  DumpChain dumper;

  // Declaration - use chain
  Declaration* noDef;
  TopDUContext* topContext;

  AbstractType::Ptr typeVoid;
  AbstractType::Ptr typeInt;

  bool testFileParseOnly;
};

#endif
