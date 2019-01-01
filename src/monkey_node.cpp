#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

float linear_min;
float linear_max;
float angular_min;
float angular_max;
float random_linear;
float random_angular;

float random(float min, float max) {
    return  (max - min) * ((((float) rand()) / (float) RAND_MAX)) + min ;
}

void timerCallback(const ros::TimerEvent& event){
  random_linear  = random(linear_min, linear_max);
  random_angular = random(angular_min, angular_max);
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "monkey_node");
  ros::NodeHandle n;

  if (ros::param::get("~linear_min", linear_min)) {}
  else {
    ROS_WARN("Failed to get param 'linear_min' - default -0.5 m/s");
    linear_min = -0.5;
  }

  if (ros::param::get("~linear_max", linear_max)) {}
  else {
    ROS_WARN("Failed to get param 'linear_max' - default 0.5 m/s");
    linear_max = 0.5;
  }

  if (ros::param::get("~angular_min", angular_min)) {}
  else {
    ROS_WARN("Failed to get param 'angular_min' - default -1.0 rad/s");
    angular_min = -1.0;
  }

  if (ros::param::get("~angular_max", angular_max)) {}
  else {
    ROS_WARN("Failed to get param 'angular_max' - default 1.0 rad/s");
    angular_max = 1.0;
  }

  float refresh_duration;
  if (ros::param::get("~refresh_duration", refresh_duration)) {}
  else {
    ROS_WARN("Failed to get param 'refresh_duration' - default 1 sec");
    refresh_duration = 1.0;
  }

  float crossfade_coeff;
  if (ros::param::get("~crossfade_coeff", crossfade_coeff)) {}
  else {
    ROS_WARN("Failed to get param 'crossfade_coeff' - default 0.01");
    crossfade_coeff = 0.01;
  }

  float publish_rate;
  if (ros::param::get("~publish_rate", publish_rate)) {}
  else {
    ROS_WARN("Failed to get param 'publish_rate' - default 50Hz");
    publish_rate = 50.0;
  }

  ros::Rate r(publish_rate);
  ros::Timer timer = n.createTimer(ros::Duration(refresh_duration), timerCallback);
  ros::Publisher cmd_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 50);
  geometry_msgs::Twist cmd_vel;

  float filtered_linear;
  float filtered_angular;

  while(n.ok()){
    ros::spinOnce();

    float diff_linear  = random_linear - filtered_linear;
    filtered_linear   += diff_linear * crossfade_coeff;
    float diff_angular = random_angular - filtered_angular;
    filtered_angular  += diff_angular * crossfade_coeff;

    cmd_vel.linear.x  = filtered_linear;
    cmd_vel.angular.z = filtered_angular;
    cmd_pub.publish(cmd_vel);
    r.sleep();
  }
}
