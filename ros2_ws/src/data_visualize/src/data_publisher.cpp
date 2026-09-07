#include <chrono>
#include <memory>
#include <string>
#include <cmath>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses a fancy C++11 lambda
* function to shorten the callback syntax, at the expense of making the
* code somewhat more difficult to understand at first glance. */

class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher()
  : Node("data_publisher"), count_(0)
  {
    sin_publisher_ = this->create_publisher<std_msgs::msg::Float32>("sin", 10);
    square_publisher_ = this->create_publisher<std_msgs::msg::Float32>("square", 10);
    timer_ = this->create_wall_timer(0.5ms, [this](){sin_publish();});
  }

private:
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr sin_publisher_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr square_publisher_;
  size_t count_;

  void sin_publish()
  {
    int sin_cycle = 10;
    int square_cycle = 1;
    float time = 0.001;  //采样时间间隔
    float single_time = time*count_;  //单次采样时间
    float sin_value = sin(2 * M_PI * single_time * sin_cycle);  //正弦波
    int square_value = sin(2*M_PI*single_time*square_cycle) > 0 ? 1 : -1;   //方波

    count_++;
    std_msgs::msg::Float32 sin_msg;
    sin_msg.data = sin_value;
    std_msgs::msg::Float32 square_msg;
    square_msg.data = square_value;

    sin_publisher_->publish(sin_msg);
    square_publisher_->publish(square_msg);
  }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}