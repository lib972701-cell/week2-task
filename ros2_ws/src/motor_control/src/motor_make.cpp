#include <chrono>
#include <cmath>
#include <memory>
#include <queue>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"
#include "motor_control/motor_model.hpp"


#define MAX_NUM  5

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses a fancy C++11 lambda
* function to shorten the callback syntax, at the expense of making the
* code somewhat more difficult to understand at first glance. */

class motor_maker : public rclcpp::Node
{
public:
  motor_maker()
  : Node("motor_maker"), 
  torque(0),
  last_control_time_point_(std::chrono::steady_clock::now())
  {
    torque_publisher_ = this->create_publisher<std_msgs::msg::Float64>("motor_torque", 10);
    torque_subscription_ = this->create_subscription<std_msgs::msg::Float64>(  //创建订阅器
                                                              "control_torque",
                                                              10,
                                                              std::bind(
                                                                &motor_maker::torque_update,
                                                                this,
                                                                std::placeholders::_1));
                                                              
    timer_ = this->create_wall_timer(0.1ms, [this](){torque_callback();});
    
    angle_publisher_ = this->create_publisher<std_msgs::msg::Float64>("motor_angle", 10);
    velocity_publisher_ = this->create_publisher<std_msgs::msg::Float64>("motor_velocity", 10);
    
  }

private:
  void torque_update(const std_msgs::msg::Float64::SharedPtr torque_msg)   //扭矩接收回调
  {
      torque = torque_msg->data;//更新电机控制扭矩
  }

  //0.1ms定时器回调函数
  void torque_callback()
  {
    std_msgs::msg::Float64 angle_msg;
    std_msgs::msg::Float64 velocity_msg;
    std_msgs::msg::Float64 torque_msg;
    motor.set_torque(torque);
    motor.motor_update();
    angle_msg.data = motor.get_angle();
    velocity_msg.data = motor.get_velocity();
    torque_msg.data = motor.get_torque();
    //发布电机信息
    angle_publisher_ -> publish(angle_msg);
    velocity_publisher_ -> publish(velocity_msg);
    torque_publisher_ -> publish(torque_msg);       
  };

  motor_set motor{0.001,0,0.01,0.0001,0};

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr torque_publisher_;   //发布扭矩
  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr torque_subscription_;    //接收控制扭矩
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr angle_publisher_; //发布角度值
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr velocity_publisher_;//发布速度

  std::chrono::steady_clock::time_point last_control_time_point_;

  double torque;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<motor_maker>());
  rclcpp::shutdown();
  return 0;
}