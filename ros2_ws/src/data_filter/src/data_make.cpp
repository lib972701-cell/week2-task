#include <chrono>
#include <memory>
#include <string>
#include <cmath>
#include <random>

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
  : Node("noise_publisher"), count_(0),amplitude_(100.0)
  {
    publisher_ = this->create_publisher<std_msgs::msg::Float64>("noise", 10);   
    timer_ = this->create_wall_timer(1ms, [this](){data_make();});//发布频率1000hz
  }

private:
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
  size_t count_;
  double amplitude_;
  std::mt19937 generator_{std::random_device{}()};  //伪随机数序列
  std::normal_distribution<double> noise_dist_{0.0, amplitude_ * 0.01};//高斯分布

  void data_make()
  {
    double cycle = 20;//正弦波频率
    double time1 = 0.001;
    double single_time = count_*time1;


    double sin_value = amplitude_ * sin(2*M_PI*cycle*single_time);
    double noise_data = sin_value + noise_dist_(generator_);

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
