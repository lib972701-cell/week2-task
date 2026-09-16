#include <chrono>
#include <cmath>
#include <memory>
#include <queue>

#include "velocity_pid.hpp"

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"
#include "motor_control/motor_model.hpp"


#define MAX_NUM  5

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses a fancy C++11 lambda
* function to shorten the callback syntax, at the expense of making the
* code somewhat more difficult to understand at first glance. */

class velocity_pid : public rclcpp::Node
{
public:
  velocity_pid()
  : Node("velocity_pid"), 
  last_control_time_point_1(std::chrono::steady_clock::now()),  
  last_control_time_point_2(std::chrono::steady_clock::now())
  {
    torque_publisher_ = this->create_publisher<std_msgs::msg::Float64>("control_torque", 10);
    ve_subscription_ = this->create_subscription<std_msgs::msg::Float64>("motor_velocity", 10,
                            std::bind(&velocity_pid::ve_subscriber_callback, this, std::placeholders::_1));
    // angle_subscription_ = this->create_subscription<std_msgs::msg::Float64>("motor_angle", 10,
    //                         std::bind(&velocity_pid::angle_subscriber_callback, this, std::placeholders::_1));
    
  }

private:
//角度环pid
  // void angle_subscriber_callback(const std_msgs::msg::Float64::SharedPtr msg)
  // {
  //   auto delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(
  //           std::chrono::steady_clock::now() - last_control_time_point_1);
  //           angle = msg->data;
  //           angle_pid.dt = (1.0f*delta_time.count())/1000.0;
  //           if(delta_time.count() >= angle_pid.getsample())
  //           {
  //             velo_pid.set_target(angle_pid.pid_process(angle, angle_pid.dt));
  //             last_control_time_point_1 = std::chrono::steady_clock::now();
  //           }
  // }

  //内环速度环pid
  void ve_subscriber_callback(const std_msgs::msg::Float64::SharedPtr msg)
  {
    std_msgs::msg::Float64 torque_msg;
    auto delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - last_control_time_point_2);
            velocity = msg->data;
            velo_pid.dt = (1.0f*delta_time.count())/1000.0;
            if(delta_time.count() >= velo_pid.getsample())
            {                
                torque_msg.data = velo_pid.pid_process(velocity,velo_pid.dt);
                torque_publisher_->publish(torque_msg);
                last_control_time_point_2 = std::chrono::steady_clock::now();
            }
  };

  pid velo_pid{2,0.0f,400.0f,0.05,0.007,0.0001};
  pid angle_pid{5,0.0f,(2*M_PI)/3,4.0,0.00,0.00};
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr torque_publisher_;   //发布扭矩
  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr ve_subscription_;    //接收实时速度
  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr angle_subscription_; //接收角度值
  std::chrono::steady_clock::time_point last_control_time_point_1;
  std::chrono::steady_clock::time_point last_control_time_point_2;
  double velocity;
  double angle;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<velocity_pid>());
  rclcpp::shutdown();
  return 0;
}