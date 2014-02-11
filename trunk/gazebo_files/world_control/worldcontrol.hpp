#ifndef WORLDCONTROL_H
#define WORLDCONTROL_H
#include <QWidget>
#include "countdown.hpp"
#include "score.hpp"
#include "transport.hpp"

class WorldControl : public QWidget
{
public:
  WorldControl(QWidget *parent = 0);
};

#endif
