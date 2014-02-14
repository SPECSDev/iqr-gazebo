#include <QtGui>

#include "countdown.hpp"
#include <iostream>


 Countdown::Countdown(QWidget *parent)
     : QLCDNumber(parent)
 {
     setSegmentStyle(Filled);

     timer = new QTimer(this);
     timeStart = new QTime(0,0,10);
     timeCurrent = new QTime(0,0,10);
     this->display(timeCurrent->toString());
     
     connect(timer, SIGNAL(timeout()), this, SLOT(count()));
     display(timeCurrent->toString());
    

   

    
 }

void Countdown::count()
{
  //std::cout<<"Countdown::Count"<<std::endl;
  timeCurrent->setHMS(0,timeCurrent->addSecs(-1).minute(),
		      timeCurrent->addSecs(-1).second());
  display(timeCurrent->toString());
  
  if(timeCurrent->minute()==0 && timeCurrent->second()==0){
    timer->stop();
    emit end();
  }
}

void Countdown::reset(){
  //std::cout<<"Countdown::Restart"<<std::endl;
  *timeCurrent = *timeStart; 
   timer->stop();
   display(timeCurrent->toString());

}

void Countdown::start(){
  //std::cout<<"Countdown::Start"<<std::endl;
  timer->start(1000);
  count();
}

void Countdown::stop(){
  timer->stop();
}
