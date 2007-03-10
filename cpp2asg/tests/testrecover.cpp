#include "testrecover.h"

#include "tosource.h"
#include "filepreprocessor.h"

#include <parser.h>
#include <parsesession.h>

TestRecover::TestRecover() {
  QDir d("/home/adam/dev/sforms/cpp2asg/tests/files");
  QFileInfoList infos = d.entryInfoList(QDir::Files | QDir::Readable, QDir::Name);
  foreach (QFileInfo f, infos) {
    m_files << f.absoluteFilePath();
  }
}

TestRecover::~TestRecover() {
}

void TestRecover::handleFile(const QString &fileName) {
  QFile file(fileName);
  Q_ASSERT(file.open(QIODevice::ReadOnly));

  QByteArray fileData = file.readAll();
  file.close();

  QString contents = QString::fromUtf8(fileData.constData());

  QString ppd;
  FilePreprocessor preprocessor;

  rpp::Environment *e = preprocessor.processString(fileName, contents, &ppd);

  ParseSession* session = new ParseSession();
  session->setContents(ppd.toUtf8());
  session->environment = e;

  Parser parser(&m_control);
  TranslationUnitAST* ast = parser.parse(session);
  ast->session = session;

  QUrl url = QUrl::fromLocalFile(fileName);

  QString name = QFileInfo(file).fileName();
  ToSource src(url);

  src.setDebug(name == "test.cpp");
  if (name == "test.cpp") qDebug() << "===== Preprocessed:" << session->contents() << endl;

  QString source = src.dump(ast, session);

  QTest::newRow(name.toAscii().constData()) << contents << QString(source.toUtf8());
}

void TestRecover::recoverSource_data() {
  QTest::addColumn<QString>("source");
  QTest::addColumn<QString>("recover");

  foreach (QString f, m_files) {
    handleFile(f);
  }
}

void TestRecover::recoverSource() {
  QFETCH(QString, source);
  QFETCH(QString, recover);

  QEXPECT_FAIL("first_line.h", "FIXME", Continue);
  QEXPECT_FAIL("comment.h", "FIXME", Continue);

  source.prepend("\n");
  recover.prepend("\n");

  QCOMPARE(source, recover);
}

 QTEST_APPLESS_MAIN(TestRecover)
