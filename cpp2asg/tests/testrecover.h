#ifndef TESTRECOVER_H
#define TESTRECOVER_H

#include <QtTest/QtTest>

#include "control.h"

class TestRecover : public QObject {
  Q_OBJECT
public:
  TestRecover();
  ~TestRecover();

private slots:
  void recoverSource_data();
  void recoverSource();

private:
  void handleFile(const QString &fileName);
  QStringList m_files;
  Control m_control;
};

#endif
