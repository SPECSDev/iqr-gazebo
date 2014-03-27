#ifndef WORLDCONTROL_H
#define WORLDCONTROL_H
#include <QWidget>
#include "countdown.hpp"
#include "score.hpp"
#include "transport.hpp"
#include <QtGui>

class WorldControl : public QWidget
{
  Q_OBJECT;
public:
  WorldControl(bool autostart=false, QWidget *parent = 0);
private:
  QPushButton *start;
  QPushButton *stop;
  QPushButton *reset;
  QPushButton *quit;

public slots:
 void visibilityEnd();

private slots:
  void visibilityStart();
  void visibilityStop();
  void visibilityReset();
 
  
};

#endif
