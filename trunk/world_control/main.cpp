#include <QApplication>

#include "worldcontrol.hpp"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  QStringList args = app.arguments();
  
  bool autostart=false;
  QStringListIterator argsIterator(args);
  argsIterator.next(); //ignore command name

  while (argsIterator.hasNext()){
    std::cout<<argsIterator.peekNext().toStdString()<<std::endl;

    if(argsIterator.next()!="-r"){
      std::cerr<< "Invalid argument! Use -r to start worldcontrol at startup"<<std::endl;
      return 1;
    }    
    else{
      autostart = true;
    }
  }    
  
  WorldControl *worldcontrol = new WorldControl(autostart);
  
  worldcontrol->show();
 
  return app.exec();
}
