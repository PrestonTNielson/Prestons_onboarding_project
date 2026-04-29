#include "maze_publisher.hpp"


MinimalPublisher::MinimalPublisher()
    : Node("maze_waypoint_publisher")
{
    publisher_ = this->create_publisher<std_msgs::msg::String>("preston", 10);
    timer_ = this->create_wall_timer(500ms, std::bind(&MinimalPublisher::timer_callback, this));
}

void MinimalPublisher::timer_callback()
{
    auto message = std_msgs::msg::String();
    message.data = "Hello, world!";
    publisher_->publish(message);
}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MinimalPublisher>());
    rclcpp::shutdown();
    return 0;
}
