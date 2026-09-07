#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"

class data_subcriber : public rclcpp::Node
{
public:
  data_subcriber()
  : Node("data_subscriber")
  {
    out_publisher_ = this->create_publisher<std_msgs::msg::Float32>("result", 10);

    sin_subscription_ = this->create_subscription<std_msgs::msg::Float32>(
      "sin", 10,
      [this](const std_msgs::msg::Float32::SharedPtr message) {
        latest_sin_ = message->data;
        has_sin_ = true;
      });

    square_subscription_ = this->create_subscription<std_msgs::msg::Float32>(
      "square",10,
      [this](const std_msgs::msg::Float32::SharedPtr message){
        std_msgs::msg::Float32 out_msg;
        
        if(has_sin_ && message->data*latest_sin_ > 0)
        {
          out_msg.data = latest_sin_;
          out_publisher_->publish(out_msg);
        }
        else {
          out_msg.data = 0.0f;
          out_publisher_->publish(out_msg);
        }
      }
    );
  }

private:
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr sin_subscription_;
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr square_subscription_;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr out_publisher_;
  float latest_sin_{0.0f};
  bool has_sin_{false};
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<data_subcriber>());
  rclcpp::shutdown();
  return 0;
}