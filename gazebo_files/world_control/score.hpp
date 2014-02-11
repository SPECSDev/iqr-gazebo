#ifndef SCORE_H
#define SCORE_H

#include <QLCDNumber>

class Score : public QLCDNumber
{
  Q_OBJECT
  
public:
  Score(QWidget *parent = 0);
  
private:
  int pointsA;
  int pointsB;
  void setDisplay();

		   
public slots:
  void reset();
  void addPointA();
  void addPointB();
  
};

 #endif
