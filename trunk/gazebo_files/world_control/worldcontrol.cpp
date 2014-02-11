#include <QtGui>

#include "worldcontrol.hpp"


WorldControl::WorldControl(QWidget *parent)
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
  
 
  

  QPushButton *quit = new QPushButton(tr("Quit"));
  quit->setFont(QFont("Arial", 18, QFont::Bold));
  connect(quit, SIGNAL(clicked()), qApp, SLOT(quit()));

  QPushButton *start = new QPushButton(tr("Start"));
  start->setFont(QFont("Arial", 18, QFont::Bold));
  connect(start, SIGNAL(clicked()), countdown, SLOT(start()));
  

  QPushButton *reset = new QPushButton(tr("Reset"));
  reset->setFont(QFont("Arial", 18, QFont::Bold));
  connect(reset, SIGNAL(clicked()), countdown, SLOT(reset()));
  connect(reset, SIGNAL(clicked()), score, SLOT(reset()));
 
  gazebo::Transport *transport = new gazebo::Transport();
  connect(reset, SIGNAL(clicked()), transport, SLOT(reset()));
  connect(start, SIGNAL(clicked()), transport, SLOT(start()));
  connect(transport, SIGNAL(addPointA()), score, SLOT(addPointA()));
  connect(transport, SIGNAL(addPointB()), score, SLOT(addPointB()));
  

  /*
  QPushButton *addA = new QPushButton(tr("+A"));
  addA->setFont(QFont("Arial", 18, QFont::Bold));
  //connect(addA, SIGNAL(clicked()), score, SLOT(addPointA()));
  connect(addA, SIGNAL(clicked()), transport, SLOT(reset()));
    
  QPushButton *addB = new QPushButton(tr("+B"));
  addB->setFont(QFont("Arial", 18, QFont::Bold));
  connect(addB, SIGNAL(clicked()), score, SLOT(addPointB()));
  */

  

  
  QGridLayout *layout = new QGridLayout;
  layout->addWidget(labelTime,1,1,1,3);
  layout->addWidget(countdown,2,1,3,3);  

  layout->addWidget(labelScore,5,1,1,3);
  layout->addWidget(score,6,1,3,3);    
  
  layout->addWidget(start,9,1);  
  layout->addWidget(reset,9,2);  
  layout->addWidget(quit,9,3);
  
  /*
  layout->addWidget(addA,10,1);
  layout->addWidget(addB,10,3);
  */
 
  
  
  setLayout(layout);
  setMinimumSize(200,400);
  setWindowTitle("World Control");

}
