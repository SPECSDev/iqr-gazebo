## Error messages while compiling ##
### Error (iqr-devel): ###
While compiling the iqr\_module I get the following error:
```
 fatal error: Common/Item/threadModule.hpp: No such file or directory
```
**Possible Solution**
The compiler can not find the iqr header files. Please download the iqr-devel package from here: http://sourceforge.net/projects/iqr/files/iqr-devel/2.0.0/ and install using:
```
sudo dpkg -i iqr-dev_2.0-0.ubuntu_debian.i386.deb
```

### Error (iqr root): ###
While compiling the iqr\_module I get the following error:
```
 CMake Error at cmake_install.cmake:50 (FILE):
  file cannot create directory: /home/armin/iqr/lib/Modules.  Maybe need
  administrative privileges.
```
**Possible Solution**
The installer does not have write privileges for the $HOME/iqr folder. Please grant the user write privileges for the folder or remove the folder.
```
sudo rm -rf $HOME/iqr
. $HOME/iqr-gazebo/update_compile.sh
```

### Error (libqt4-dev): ###
While compiling the iqr\_module I get the following error:
```
  fatal error: QMutex: No such file or directory
 #include <QMutex>
```
**Possible Solution**
The installer can not find the qt header files. Please install the libqt4-dev package using.
```
sudo apt-get install libqt4-dev
```


## Error messages on run time ##
### Error (gzserver) ###
When running the iqr system, iqr stalls and stops at around 68% with the following output in the console:
```
INIT Gazebo Transport !
Msg Waiting for master..............
```

**Possible Soulution**
iqr can not find the gazebo server. Please run the gazebo server first as explained [here](RunningGazeboIQR.md)


### Error (bug name) ###
When running the iqr system, iqr stalls and stops at around 68% with the following output in the console:
```
Publishing to: /gazebo/default/TeamA_Bug_01/vel_cmd ....
```

**Possible Soulution**
iqr can not find the robot for communication. Please make sure the arena you are running on the gazebo server contains a bug robot and that the name corresponds to the parameter given in the iqr module under **Bug Name**


## In case things still go wrong... ##
Build the iqr modules and gazebo plugins without the script.

### Setting environment variables ###
Append the following lines to your .profile file in the home directory using gedit.
```
export GAZEBO_MODEL_PATH=$HOME/iqr-gazebo/gazebo_files/models
export GAZEBO_PLUGIN_PATH=$HOME/iqr-gazebo/gazebo_files/plugins
```
or use the following commands to add them from the console line
```
echo 'export GAZEBO_MODEL_PATH=$HOME/iqr-gazebo/gazebo_files/models' >> $HOME/.profile
echo 'export GAZEBO_PLUGIN_PATH=$HOME/iqr-gazebo/gazebo_files/plugins' >> $HOME/.profile

```

Please **logout and login again** to make sure the file is sourced.



### Build the iqr module ###
Compile the moduleGazeboBug module.
```
cd $HOME/iqr-gazebo/iqr_module
mkdir build
cd build
cmake ..
make install

```

### Build the gazebo plugins ###
Compile the gazebo vel\_plugin.
```
cd $HOME/iqr-gazebo/gazebo_files/vel_plugin
mkdir build
cd build
cmake ..
make install

```

Compile the gazebo target\_plugin.
```
cd $HOME/iqr-gazebo/gazebo_files/target_plugin
mkdir build
cd build
cmake ..
make install

```