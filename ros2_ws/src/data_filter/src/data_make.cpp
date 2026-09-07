#include <chrono>
#include <memory>
#include <string>
#include <cmath>
#include <cstdlib>
#include <time.h>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"

using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses a fancy C++11 lambda
* function to shorten the callback syntax, at the expense of making the
* code somewhat more difficult to understand at first glance. */

class Noisepublisher : public rclcpp::Node
{
public:
  Noisepublisher()
  : Node("noise_publisher"), count_(0)
  {
    publisher_ = this->create_publisher<std_msgs::msg::Float64>("noise", 10);   
    timer_ = this->create_wall_timer(10ms, [this](){data_make();});
    srand((unsigned)time(NULL));
  }

private:
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
  size_t count_;

  void data_make()
  {
    int cycle = 1;
    double time1 = 0.01;
    double single_time = count_*time1;


    double sin_value = 100*sin(2*M_PI*cycle*single_time);
    double noise_data = sin_value + 10.0*(rand()%10-5);

    std_msgs::msg::Float64 noise_msg;
    noise_msg.data = noise_data;
    publisher_ -> publish(noise_msg);
    count_++;
  }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Noisepublisher>());
  rclcpp::shutdown();
  return 0;
}