cd $HOME
#svn checkout http://iqr-gazebo.googlecode.com/svn/trunk/ iqr-gazebo
rm -rf iqr-gazebo
wget  -r  -np  -nH --cut-dirs=1 --reject "index.htm*" http://iqr-gazebo.googlecode.com/svn/trunk/
mv trunk iqr-gazebo
cd iqr-gazebo
export GAZEBO_MODEL_PATH=$HOME/iqr-gazebo/gazebo_files/models
export GAZEBO_PLUGIN_PATH=$HOME/iqr-gazebo/gazebo_files/plugins
#export GAZEBO_MASTER_URI=10.200.188.1:11345
. ./update_compile.sh
#. ./set_server_ip.sh 10.200.188.1