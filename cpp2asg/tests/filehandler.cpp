#include "filehandler.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStringList>

#include "duchain.h"
#include "topducontext.h"
#include "declarationbuilder.h"
#include "usebuilder.h"
#include "declaration.h"
#include "kdevdocumentrange.h"
#include "cppeditorintegrator.h"
#include "dumptypes.h"

#include "tokens.h"
#include "parsesession.h"
#include "symboltable.h"
#include "pp-environment.h"

#include "tosource.h"
#include "filepreprocessor.h"

FileHandler::FileHandler() {}

FileHandler::~FileHandler() {}

void FileHandler::handleFile(const QString &fileName) {

  QFile file(fileName);
  Q_ASSERT( file.open( QIODevice::ReadOnly ) );

  QByteArray fileData = file.readAll();
  file.close();
  QString contents = QString::fromUtf8( fileData.constData() );
  FilePreprocessor preprocessor;

  QString ppd;
  rpp::Environment *e = preprocessor.processString( fileName, contents, &ppd );
  ParseSession* session = new ParseSession();
  session->setContents(ppd.toUtf8());
  session->environment = e;

  parse(QUrl::fromLocalFile(fileName), session,
        static_cast<DumpAreas>(DumpDUChain /*| DumpType | DumpAST*/));
}

DUContext* FileHandler::parse(const QUrl &url, ParseSession *session, DumpAreas dump) {

  qDebug() << "===== Preprocessed:" << session->contents() << endl;

  Parser parser(&control);
  TranslationUnitAST* ast = parser.parse(session);
  ast->session = session;

//   if (dump & DumpAST) {
//     qDebug() << "===== AST:" << endl;
//     dumper.dump(ast, session);
//   }

  ToSource src(url);
  const QByteArray source = src.dump(ast, session).toAscii();
  qDebug() << "===== From AST:" << source.constData() << endl;

  qDebug() << "recovered" << (QString(session->contents()).trimmed() == QString(source).trimmed() ? "true" : "false") << endl;

  TopDUContext* top = 0;
//   DeclarationBuilder definitionBuilder(session);
//   top = definitionBuilder.buildDeclarations(url, ast);
// 
//   UseBuilder useBuilder(session);
//   useBuilder.buildUses(ast);

//   for (int i = 0; i < session->token_stream->size(); ++i) {
//     QString s = session->token_stream->token(i).symbol();
//     if (!s.isEmpty())
//     qDebug() << i << " " << s << endl;
//   }

//   if (dump & DumpDUChain) {
//     qDebug() << "===== DUChain:" << endl;
// 
//     QReadLocker lock(DUChain::lock());
//     dumper.dump(top);
//   }
// 
//   if (dump & DumpType) {
//     qDebug() << "===== Types:" << endl;
//     DumpTypes dt;
//     foreach (const AbstractType::Ptr& type, definitionBuilder.topTypes())
//     dt.dump(type.data());
//   }
// 
//   SymbolTable::self()->dumpStatistics();

  delete session;

  return top;

}
