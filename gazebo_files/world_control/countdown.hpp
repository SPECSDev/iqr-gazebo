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
   void stop();
	      
 private slots:
   void count();
 signals:
   void end();
 };

 #endif
