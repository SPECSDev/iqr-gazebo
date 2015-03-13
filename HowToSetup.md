# Introduction #

For iqr-gazebo to work you need to install both iqr and gazebo and then compile the iqr module as well as the gazebo plugins. This instructions are for Ubuntu OS. Please use the analog commands for different linux distributions


# How To Step by Step #

### Install General Packages ###
Install general packages used to compile and run iqr-gazebo
```
sudo apt-get install gdebi subversion build-essential cmake libqt4-dev 

```

If you have already installed the latest version of gazebo and iqr continue at step **Setting environment variables**

### Install IQR ###
Download the binary packages for ubuntu form http://sourceforge.net/projects/iqr/files/iqr/ and install using:
```
sudo gdebi iqr-2.4.1.Ubuntu-xxx-xxx.deb

```
You have to introduce this command in the directory where you have downloaded the files. Please replace the xxx-xxx with the file name you have downloaded corresponding to your os.


Download the developer files from http://sourceforge.net/projects/iqr/files/iqr-devel/2.0.0/ and install using:
```
sudo dpkg -i iqr-dev_2.0-0.ubuntu_debian.i386.deb

```

### Install Gazebo ###

Install Gazebo following the instruction on: http://gazebosim.org/tutorials/?tut=install. Please note you need at least gazebo version 2.2 or higher.

### Get the iqr-gazebo Files ###
Check out the files using svn to your home directory.
```
cd $HOME
svn checkout http://iqr-gazebo.googlecode.com/svn/trunk/ iqr-gazebo

```

In case you do not have access to the svn repository you can download the latest source code using wget. Attention this overwrites the current iqr-gazebo folder:
```
cd $HOME
rm -rf iqr-gazebo
wget  -r  -np  -nH --cut-dirs=1 --reject "index.htm*" http://iqr-gazebo.googlecode.com/svn/trunk/
mv trunk iqr-gazebo
```




## Install iqr-gazebo ##

To build the necessary iqr moudles and gazebo plugins please run the following script:
```
cd $HOME/iqr-gazebo
source ./update_compile.sh
```

If the script fails please follow the instruction at the end of the document. You can use the same script for updating and compiling the iqr-gazebo project from the svn repository.
You are all setup. Check out how to run iqr and gazebo [here](RunnginGazeboIQR.md). In case thinks go wrong please check the [Q&A wiki](QuestionAndAnswers.md)