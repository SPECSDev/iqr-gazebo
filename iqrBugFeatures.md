# Introduction #

The iqrBug consists in a compilation of files including the  iqr module called moduleGazeboBug, the gazebo plugins and the gazebo model files.


# Features #
The iqrBug offers a series of sensors and actuators. Here we describe their properties and proper use.

## General ##
The iqr module provides the communication between iqr and gazebo. All communication happens over groups of cells. An iqr system can comprise more than one iqrBug. To differentiate what iqrBug is controlled by what module use the **Bug name** parameter. The name needs to correspond to the name given to the bug in the gazebo world by default or in the  .world file.

The modulGazeboBug iqr module is a threaded module. The **Cycles/Second** parameter determines the speed the module is running. This is independent on the speed iqr is running. Make sure that the three process i.e. iqr, gazebo, iqrModule have similar cycles per second.

## Sensors ##
### Laser Range Finder ###
The laser range finder provides proximity information for almost the whole surrounding of the robot. The cells are from _left-back_ over _front_ to _right-back_.

### Laser Range Finder Gripper ###
The laser range finder gripper provides proximity information for the near proximity of the gripper. The cells are from _left_ to _right_.

### Camera ###
The camera provides different modes: RGB, BW, and HSV. Please tick the corresponding parameters in the module properties. No selection corresponds to the default RGB.

The resolution of the camera is defined by the size of the groups. The default max resolution is 32x24. Please make sure all groups assigned to the different values of the camera have the same size.

### GPS ###
The GPS sensor offers position in x (cell 1) ,y (cell 2) and rotation (cell 3). This information is also transformed to place cell and head direction cell information. The resolution of both is defined by the geometry of the associated groups. The head direction cell group needs to be of height 1.

Adjust the corresponding parameters to adjust the shape of the place and head direction cells.


### Microphone ###
The iqrBug can communicate over a virtual microphone and speaker system. The sound will decay linearly over distance up to  a max of 10 m. The iqrBug will not hear the sound it emits. Only iqrBugs sharing the same communication key will hear each other. You can set the **Communication Key** parameter in the module properties.
## Actuators ##
The iqrBug comprises motors for locomotion, a gripper and a speaker for emitting sounds.

### Motors ###
The speed of the motors are set over a motor map with a winner take all mechanism. Each cell of the motor group stands for a specific speed. The motor map is organized along translational and rotational speed. The center of the motor map corresponds to 0 translational speed and 0 rotational speed. Translational speed varies from top to bottom between **+Max Translational Speed** to **-Max Translational Speed** and rotational speed varies from left to right to **-Max Rotational Speed** to **+Max Rotational Speed**.

You can set the max speeds in the module properties.

### Gripper ###
The gripper state can be opening or closing. The state can be set over the cell assigned to the gripper. If the activity is > 0.5 than the state is closing and if it is <0.5 it is opening.

### Speaker ###
The speaker emits the sound i.e. the activity of the associated cell group. (See Microphone)