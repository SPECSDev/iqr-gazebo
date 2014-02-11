 #ifndef COUNTDOWN_H
 #define COUNTDOWN_H

 #include <QLCDNumber>

 class Countdown : public QLCDNumber
 {
     Q_OBJECT

 public:
   Countdown(QWidget *parent = 0);
   QTime* timeCurrent;
   QTime*  timeStart;
   QTimer* timer;
 
 public slots:
   void reset();
   void start();
		
 private slots:
     void count();
   
 };

 #endif
