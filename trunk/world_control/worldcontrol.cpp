

#include "worldcontrol.hpp"


WorldControl::WorldControl(bool autostart, QWidget *parent)
  : QWidget(parent)
{
  QLabel* labelTime = new QLabel("Time");
  labelTime->setFont(QFont("Arial", 18, QFont::Bold));
  labelTime->setAlignment(Qt::AlignCenter);
  labelTime->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  
  Countdown *countdown = new Countdown(this); 
  
  QLabel* labelScore = new QLabel("Score");
  labelScore->setFont(QFont("Arial", 18, QFont::Bold));
  labelScore->setAlignment(Qt::AlignCenter);
  labelScore->setFrameStyle(QFrame::Panel | QFrame::Sunken);
  
  Score *score = new Score(this); 
  
  start = new QPushButton(tr("Start"));
  start->setFont(QFont("Arial", 18, QFont::Bold));
  connect(start, SIGNAL(clicked()), countdown, SLOT(start()));
  
  stop = new QPushButton(tr("Stop"));
  stop->setFont(QFont("Arial", 18, QFont::Bold));
  connect(stop, SIGNAL(clicked()), countdown, SLOT(stop()));
  
  reset = new QPushButton(tr("Reset"));
  reset->setFont(QFont("Arial", 18, QFont::Bold));
  connect(reset, SIGNAL(clicked()), countdown, SLOT(reset()));
  connect(reset, SIGNAL(clicked()), score, SLOT(reset()));
 
  gazebo::Transport *transport = new gazebo::Transport();
  connect(reset, SIGNAL(clicked()), transport, SLOT(reset()));
  connect(start, SIGNAL(clicked()), transport, SLOT(start()));
  connect(stop, SIGNAL(clicked()), transport, SLOT(stop()));
  connect(transport, SIGNAL(plusPointA()), score, SLOT(plusPointA()));
  connect(transport, SIGNAL(plusPointB()), score, SLOT(plusPointB()));
  connect(transport, SIGNAL(minusPointA()), score, SLOT(minusPointA()));
  connect(transport, SIGNAL(minusPointB()), score, SLOT(minusPointB()));
  connect(countdown, SIGNAL(end()), transport, SLOT(reset()));
   
  quit = new QPushButton(tr("Quit"));
  quit->setFont(QFont("Arial", 18, QFont::Bold));
  connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));
  connect(qApp, SIGNAL(aboutToQuit()), transport, SLOT(close()));
  
  connect(start,SIGNAL(clicked()), this, SLOT(visibilityStart()));
  connect(stop,SIGNAL(clicked()), this, SLOT(visibilityStop()));
  connect(reset,SIGNAL(clicked()), this, SLOT(visibilityReset()));
  connect(countdown, SIGNAL(end()), this, SLOT(visibilityEnd()));

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(labelTime,1,1,1,4);
  layout->addWidget(countdown,2,1,3,4);  

  layout->addWidget(labelScore,5,1,1,4);
  layout->addWidget(score,6,1,3,4);    
  
  layout->addWidget(start,9,1);
  layout->addWidget(stop,9,2);
  layout->addWidget(reset,9,3);  
  layout->addWidget(quit,9,4);
  
    
  setLayout(layout);
  setMinimumSize(200,400);
  setWindowTitle("World Control");
  
  visibilityReset();


  if(autostart){
    countdown->start();
    transport->start();
    visibilityStart();
  }
  
}

void WorldControl::visibilityStart(){
  start->setEnabled(false);
  stop->setEnabled(true);
  reset->setEnabled(true);
}

void WorldControl::visibilityStop(){
  start->setEnabled(true);
  stop->setEnabled(false);
  reset->setEnabled(true);
}
void WorldControl::visibilityReset(){
  start->setEnabled(true);
  stop->setEnabled(false);
  reset->setEnabled(false);
}

void WorldControl::visibilityEnd(){
  start->setEnabled(false);
  stop->setEnabled(false);
  reset->setEnabled(true);
}
