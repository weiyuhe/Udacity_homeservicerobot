#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/tf.h>

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  // Initialize the simple_navigation_goals node
  ros::init(argc, argv, "pick_objects");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;

  // set up the frame parameters
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();
  float targets[2][3] = {{-3.0, 0.28, 1.5707}, {0.08, 1.2, 1.5707}};
  bool reached = false;


  for(int i = 0; i<2 ;i++){

    // Define a position and orientation for the robot to reach
    goal.target_pose.pose.position.x = targets[i][0];
    goal.target_pose.pose.position.y = targets[i][1];
    goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(targets[i][2]);

    // Send the goal position and orientation for the robot to reach
    ROS_INFO("Sending goal");
    ac.sendGoal(goal);

    // Wait an infinite time for the results
    ac.waitForResult();

    // Check if the robot reached its goal
    if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED && i == 1 ){
      
      ROS_INFO("Hooray, the robot has reached the drop zone.");
      
    }else if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED && i == 0){
      ROS_INFO("Hooray, the robot has reached the pick-up zone.");

    }else{
      ROS_INFO("The base failed to move forward 1 meter for some reason");
    }

    ros::Duration(5.0).sleep();
  }
  


  return 0;
}