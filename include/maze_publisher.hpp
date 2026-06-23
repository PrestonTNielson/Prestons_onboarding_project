#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <std_msgs/msg/float32_multi_array.hpp>
#include <roscopter_msgs/msg/state.hpp>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "roscopter_msgs/msg/waypoint.hpp"

using namespace std::chrono_literals;


class MinimalPublisher : public rclcpp::Node
{
public:
  MinimalPublisher();

private:
  void timer_callback(); 
  void waypoint_publish();
  void sensor_callback(const std_msgs::msg::Float32MultiArray::SharedPtr msg);
  void estimated_state_callback(const roscopter_msgs::msg::State::SharedPtr msg);

  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::Publisher<roscopter_msgs::msg::Waypoint>::SharedPtr waypoint_publisher_;
  rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr subscriber;
  rclcpp::Subscription<roscopter_msgs::msg::State>::SharedPtr estimated_state_subscriber;
};



