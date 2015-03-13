# SDIC Robot Challenge #

Two teams of robots will compete in the same arena for limited resources. The arena is surrounded by walls and comprises, resources i.e. small spheres of different colours,  obstacles and two home corners for each team. The goal of the competition is to search for resources and bring them back to the home corner. Different resources will be associated to different number of points. [Groups](GroupsSDICchallenge.md) for the SDIC robot challenge 2014.

## Groups ##
| **Group 1 SABOTEUR Team** | **Group 2: TANSTAAFL** | **Group 3 Synapse Troopers** | **Group 4: Beetlejuice** | **Group 5: The Brainiacs** |
|:--------------------------|:-----------------------|:-----------------------------|:-------------------------|:---------------------------|
|Marwecki Sebastian|Pastor Sanchez Alvaro|Castillo Perez Heidi Beatriz|Demir Benan|Crowell Ciera|
|Li Zhao|Low Sock Ching|Matamoros Llopis Elisabet|Ramirez Gomez Argenis|Dieckhoff Anita|
|Vasile Valentina|Moreno Dionis Monica	|Berga Garreta David|Turquois Chloe|Bargues i Ribera Maria|
|Salantiu Luiza Maria|Marin Getino Guillermo|Hitscherich Rodriguez Gustavo|Ballmann Gianna| Padial Lopez Carlos Joaquin|

## Configuration ##
  * Robots Team A = red, Robots Team B = green
  * Home corner Team A = black, Home corner Team B = black
  * Obstacles grey
  * Resources  blue=2 point, yellow = -1 point, when brought to one of the home corners
  * Resources will be provided at two to four fixed locations
  * Resources will turn bad and change color from blue to yellow and will finally disappear
  * Grasped resources will not disappear
  * Resources will dissolve automatically when brought to the home corner


## Rules ##
  * The competition lasts 5 min.
  * The competition follow a double elimination format http://challonge.com/sdic_robot
  * The robots are controlled over IQR only.
  * During the competition you are allowed to change parameters and start stop the simulation.
  * During the competition you are not allowed to use the state manipulation panel or any other means of     inducing user defined or predefined activity.
  * Every robot is controlled over a separate iqr system.
  * Communication between the robots is only allowed over the provided communication system. No direct communication between groups of different robots.
  * No Cheating! Cheaters will be disqualified.



## Preparation ##

For preparing the Robot Challenge you can use the rooms 54.030 or 54.031.


## Projects ##
Each group is supposed to select a subsystem of their competition system for a more detailed design and analysis following the biomimetic approach. However also the overall system should follow a biomimetic approach.

Possible subsystems:
  * Goal based Navigation
  * Space  Value Learning
  * Communication
  * Avoidance Learning
  * Optimal Path Learning
  * Search and Grasp
  * Homeostasis - State Control

The more detailed design and analysis should contain:
  * Analysis of biological System > Constraints
  * Research Question
  * Design Principles
  * Validation Constraint
  * Results
  * Conclusion

## Presentations ##
Before the competition the different groups will present their system in a 5 min presentation. The emphasis of the presentation should be on the subsystem you selected for the more detailed biomimetic design and analysis.

## Report ##
Same as for the presentation the report should focus on the subsystem for which you have followed the more detailed biomimetic design and analysis. However the whole system has to be described in the report. You can downlaoad a template for the projcet report here ([doc](https://iqr-gazebo.googlecode.com/svn/wiki/ProjectReportTemplate.doc)).




Please save the file in pdf format and with a meaningful name including your group name and some keywords describing your project.
```
Example: Rambots_PlaceCellNavigation_SDIC2015.pdf
```