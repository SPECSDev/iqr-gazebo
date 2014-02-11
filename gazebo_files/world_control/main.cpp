#include <QApplication>

#include "worldcontrol.hpp"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  WorldControl *worldcontrol = new WorldControl();
  
  worldcontrol->show();
 
  return app.exec();
}
