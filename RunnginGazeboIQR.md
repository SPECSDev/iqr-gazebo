# Running IQR and gazebo on a single machine #
Gazebo and iqr work as a server client aplication that can work on a single machine or  on multiple machines. To run gazebo on a single machine you best run gazebo and iqr in two different terminals.
Run gazebo in the first terminal:
```
cd $HOME/iqr-gazebo/SDIC_challenge
gazebo SDIC_arena.world

```

Run iqr in  the second terminal:
```
cd $HOME/iqr-gazebo/SDIC_challenge
iqr -f  gazeboBug.iqr

```

Press run in iqr. You should see the robot driving around the arena avoiding walls. Check out the sensory states of the robot using the space plot tool.


# Running IQR and gazebo on multiple machines #

Gazebo itself is a server client application. The command **gazebo** is a script that invokes both the server  application **gzserver** and the client application of **gzclient**.  You can try it out on a single machine by typing **gzserver** in one terminal and **gzclient** in another terminal. If you want to have a dedicated server on one machine (machine A) and a client on another machine (machine B) you need to tell the client on machine B  on what ip the server is reachable. This is done over the environment variable GAZEBO\_MASTER\_URI. You can use the script **set\_server\_ip.sh IPofServer** to set GAZEBO\_MASTER\_URI. If no IP is provided the GAZEBO\_MASTER\_URI variable is unset. To run **gzserver** and **gzclient** on different machines do as follow:

On machine A (the server):
```
cd $HOME/iqr-gazebo/SDIC_challenge
gzserver SDIC_arena.world
```

Check the output. It should look something like this:
```
Msg Waiting for master
Msg Connected to gazebo master @ http://127.0.0.1:11345
Msg Publicized address: 192.168.95.180
```

The Publicized address is the ip over which the server is reachable. In this case it is 192.168.95.180. In what follows please substitute 192.168.95.180 with the ip you got from gzserver.

In machine B (the client)
```
source $HOME/iqr-gazebo/scripts/set_server_ip.sh 192.168.95.180 
gzclient
```
You should now be able to see the graphical user interface with the robot arena.


iqr uses the same communication technology as gzserver and gzclient. As such iqr is just another client accessing the gazebo server. So to run iqr on machine B type:
```
source $HOME/iqr-gazebo/scripts/set_server_ip.sh 192.168.95.180 
cd $HOME/iqr-gazebo/SDIC_challenge
iqr -f  gazeboBug.iqr
```

Please note that the script set\_server\_ip.sh does set the environment varialbe in the .bashrc file. So it will be set on all the new terminals you open. Thus you only need to set the server ip once per machine.

If you want to again run both gazebo and iqr just on machine B you have to unset the GAZEBO\_MASTER\_URI variable. You can do this by invoking set\_server\_ip.sh without any arguments. You need to do this in all open terminals.