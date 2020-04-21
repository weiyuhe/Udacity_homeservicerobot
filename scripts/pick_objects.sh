#!/bin/bash

export TURTLEBOT_GAZEBO_WORLD_FILE=$(rospack find world)/with_ball.world
export TURTLEBOT_GAZEBO_MAP_FILE=$(rospack find world)/map.yaml
xterm  -e " roslaunch turtlebot_gazebo turtlebot_world.launch" &
sleep 5
xterm  -e " roslaunch turtlebot_gazebo amcl_demo.launch " &
sleep 5
xterm  -e "source devel/setup.bash; roslaunch turtlebot_rviz_launchers view_navigation.launch" &
sleep 5
xterm  -e "source devel/setup.bash; rosrun pick_objects pick_objects"