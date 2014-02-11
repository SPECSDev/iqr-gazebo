#include <QtGui>

#include "score.hpp"
#include <iostream>


 Score::Score(QWidget *parent)
     : QLCDNumber(parent)
 {
     setSegmentStyle(Filled);
     
     QPalette *palette = new QPalette();
     palette->setColor(QPalette::WindowText,Qt::red);
     setPalette(*palette);
     
     pointsA=0;
     pointsB=0;
     setDisplay();
     
 }

void Score::setDisplay(){
  QString str;
  this->display(str.sprintf("%02d:%02d",pointsA,pointsB));
}

void Score::reset(){
  //std::cout<<"Score::Restart"<<std::endl;
  pointsA=pointsB=0;
  setDisplay();
}

void Score::addPointA(){
  //std::cout<<"Score::addPointA"<<std::endl;
  pointsA++;
  setDisplay();
}

void Score::addPointB(){
  //std::cout<<"Score::addPointB"<<std::endl;
  pointsB++;
  setDisplay();
}
