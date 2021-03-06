#!/bin/bash

export TURTLEBOT_GAZEBO_WORLD_FILE=$(rospack find world)/with_ball.world
export TURTLEBOT_GAZEBO_MAP_FILE=$(rospack find world)/map.yaml

# Launch the nodes
xterm  -e " roslaunch turtlebot_gazebo turtlebot_world.launch" &
sleep 7
xterm  -e " roslaunch turtlebot_gazebo amcl_demo.launch " &
sleep 6
xterm  -e " roslaunch turtlebot_rviz_launchers view_navigation.launch" &
sleep 6
xterm  -e " rosrun add_markers add_markers"
