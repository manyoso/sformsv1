#ifndef SFORM_H
#define SFORM_H

#include <QObject>

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
