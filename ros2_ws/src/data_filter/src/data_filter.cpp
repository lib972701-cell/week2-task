#include <memory>
#include <algorithm>
#include <queue>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"


#define MAX_NUM  5

class datafilter : public rclcpp::Node
{
public:
  datafilter()
  : Node("data_filter")
  {
    middle_publisher_ = this->create_publisher<std_msgs::msg::Float64>("middle", 10);
    low_publisher_ = this->create_publisher<std_msgs::msg::Float64>("low", 10);

    subscription_ = this->create_subscription<std_msgs::msg::Float64>(
      "noise",10,
      [this](const std_msgs::msg::Float64::SharedPtr message){
        std_msgs::msg::Float64 out_msg;
        receive_data_1 =  message->data;
        receive_data_2 = message->data;

        middle_process(receive_data_1);
        lowfilter_process(receive_data_2);
      }
    );
  }

private:
  rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr subscription_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr middle_publisher_;
  rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr low_publisher_;

  std::queue<double> middle_q;
  double receive_data_1{0.0f};
  double receive_data_2{0.0f};
  double lastdata{0.0f};

  void middle_process(double data)
  {
    double res;
    double temp_data[MAX_NUM];
    std_msgs::msg::Float64 mid_res;
    middle_q.push(data);
    if(middle_q.size() < MAX_NUM)
    { 
      return;
    }
    if(middle_q.size() > MAX_NUM)
    {
      middle_q.pop();
    }
    for(int i=0;i<MAX_NUM;i++)
    {
      temp_data[i] = middle_q.front();
      middle_q.pop();
      middle_q.push(temp_data[i]);
    }
    std::sort(temp_data,temp_data+MAX_NUM);
    res = temp_data[MAX_NUM/2]; 
    mid_res.data = res;

    middle_publisher_ -> publish(mid_res);
  }

  void lowfilter_process(double data)
  {
    double nowdata;
    double res;
    double a = 0.6;   //低通滤波系数

    std_msgs::msg::Float64 low_msg;
    nowdata = data;
    res = nowdata*(1-a) + lastdata*a;
    lastdata = res;

    low_msg.data = res;
    low_publisher_ -> publish(low_msg);
  }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<datafilter>());
  rclcpp::shutdown();
  return 0;
}