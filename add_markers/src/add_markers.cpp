 
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include "nav_msgs/Odometry.h"
#include <tf/tf.h>
#include <math.h>

bool picked = false;
bool dropped = false;
float threshold = 0.2;
float targets[2][3] = {{-3.0, 0.28, 1.5707}, {0.08, 1.2, 1.5707}};

/*  <arg name="initial_pose_x" default="-1.29"/>
  <arg name="initial_pose_y" default="-1.8"/>*/

void callback(const nav_msgs::Odometry::ConstPtr& msg)
{
  float curr_x = 0;
  float curr_y = 0;
  float picked_x_diff = 0.0;
  float picked_y_diff = 0.0;
  float dropped_x_diff = 0.0;
  float dropped_y_diff = 0.0;
  float pick_diff = 0.0;
  float drop_diff = 0.0;

  curr_x = msg->pose.pose.position.x ;
  curr_y = msg->pose.pose.position.y ;
  picked_x_diff = std::abs(curr_x - targets[0][0]);
  picked_y_diff = std::abs(curr_y - targets[0][1]);
  pick_diff = sqrt(pow(picked_x_diff,2) + pow(picked_y_diff,2));
  dropped_x_diff = std::abs(curr_x - targets[1][0]);
  dropped_y_diff = std::abs(curr_y - targets[1][1]);
  drop_diff = sqrt(pow(dropped_x_diff,2) + pow(dropped_y_diff,2));

  if (picked_x_diff < threshold && picked_y_diff < threshold){
    picked = true;
    ROS_INFO("Object picked up!");
  }else if(dropped_y_diff < threshold && dropped_x_diff < threshold){
    dropped = true;
    ROS_INFO("Object dropped!");
  }else{

    ROS_INFO("curr_x: %f", curr_x);
    ROS_INFO("curr_y: %f", curr_y);
    
/*    ROS_INFO("pick_diff_x: %f", picked_x_diff);
    ROS_INFO("pick_diff_y: %f", picked_y_diff);
    ROS_INFO("drop_diff_x: %f", dropped_x_diff);
    ROS_INFO( "drop_diff_y: %f", dropped_y_diff);*/
  }


}


int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Subscriber obom_sub = n.subscribe("/odom", 1000, callback);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  visualization_msgs::Marker marker;

  marker.header.frame_id = "map";
  marker.header.stamp = ros::Time::now();
  marker.ns = "add_markers";
  marker.id = 0;

  marker.type = visualization_msgs::Marker::CUBE;

  marker.scale.x = 0.5;
  marker.scale.y = 0.5;
  marker.scale.z = 0.5;
  marker.color.r = 0.0f;
  marker.color.g = 0.0f;
  marker.color.b = 1.0f;
  marker.color.a = 1.0;
  marker.lifetime = ros::Duration();



  while(ros::ok()){

    if(!picked){
      marker.action = visualization_msgs::Marker::ADD;
      marker.pose.position.x = targets[0][0];
      marker.pose.position.y = targets[0][1];
      marker.pose.position.z = 0.5;
      marker.pose.orientation = tf::createQuaternionMsgFromYaw(targets[0][2]);
      marker_pub.publish(marker);
    }else{
      ros::Duration(5.0).sleep();
      marker.action = visualization_msgs::Marker::DELETE;
      marker_pub.publish(marker);
    }
    if(dropped){
      ROS_INFO("dropped");
      marker.action = visualization_msgs::Marker::ADD;
      marker.pose.position.x = targets[1][0];
      marker.pose.position.y = targets[1][1];
      marker.pose.position.z = 0.5;
      marker.pose.orientation = tf::createQuaternionMsgFromYaw(targets[1][2]);

      marker_pub.publish(marker);
      ros::Duration(5.0).sleep();

    }
    ros::spinOnce();
  }

  
  return 0;
}