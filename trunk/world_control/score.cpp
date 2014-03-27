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
  str.sprintf("%02d:%02d",pointsA,pointsB);
  setDigitCount(str.length());
  this->display(str);
}

void Score::reset(){
  //std::cout<<"Score::Restart"<<std::endl;
  pointsA=pointsB=0;
  setDigitCount(5);
  setDisplay();
}

void Score::plusPointA(){
  //std::cout<<"Score::addPointA"<<std::endl;
  pointsA++;
  setDisplay();
}

void Score::plusPointB(){
  //std::cout<<"Score::addPointB"<<std::endl;
  pointsB++;
  setDisplay();
}

void Score::minusPointA(){
  //std::cout<<"Score::addPointA"<<std::endl;
  pointsA--;
  setDisplay();
}

void Score::minusPointB(){
  //std::cout<<"Score::addPointB"<<std::endl;
  pointsB--;
  setDisplay();
}
