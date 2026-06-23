#include "maze_publisher.hpp"
#include <iostream>

float counter = 0;
float North_wall_distance = 0;
float East_wall_distance = 0;
float South_wall_distance = 0;
float West_wall_distance = 0;
double current_position[3] = {0, 0, 0};
void MinimalPublisher::sensor_callback(const std_msgs::msg::Float32MultiArray::SharedPtr msg) {
    counter = 0;
    std::cout << "Received sensor data: ";
    for (const auto& value : msg->data) {
        std::cout << value << " ";
        if (counter == 0) {
            North_wall_distance = static_cast<float>(value);
        } else if (counter == 1) {
            East_wall_distance = static_cast<float>(value);
        } else if (counter == 2) {
            South_wall_distance = static_cast<float>(value);
        } else if (counter == 3) {
            West_wall_distance
             = static_cast<float>(value);
        }
        counter++;
    }

    std::cout << std::endl;
}

void MinimalPublisher::estimated_state_callback(const roscopter_msgs::msg::State::SharedPtr msg) {
    current_position[0] = static_cast<double>(msg->p_n);
    current_position[1] = static_cast<double>(msg->p_e);
    current_position[2] = static_cast<double>(msg->p_d);
}

MinimalPublisher::MinimalPublisher()
    : Node("maze_waypoint_publisher")
{
    rclcpp::QoS qos_transient_local_10_(10);
    qos_transient_local_10_.transient_local();
    
    estimated_state_subscriber = this->create_subscription<roscopter_msgs::msg::State>("estimated_state", 10,
         std::bind(&MinimalPublisher::estimated_state_callback, this, std::placeholders::_1));
    subscriber = this->create_subscription<std_msgs::msg::Float32MultiArray>("sensors/walls_sensor", 10,
         std::bind(&MinimalPublisher::sensor_callback, this, std::placeholders::_1));
    waypoint_publisher_ =
    this->create_publisher<roscopter_msgs::msg::Waypoint>(
        "waypoints", qos_transient_local_10_);
    publisher_ = this->create_publisher<std_msgs::msg::String>("preston", 10);
    timer_ = this->create_wall_timer(500ms, std::bind(&MinimalPublisher::timer_callback, this));
    std::cout << "Publisher node started" << std::endl;
}

void MinimalPublisher::timer_callback()
{

    auto message = std_msgs::msg::String();
    message.data = "Hello, world!";
    publisher_->publish(message);

    waypoint_publish();
}

void MinimalPublisher::waypoint_publish()
{
    std::cout << "Publishing waypoint" << std::endl;
  roscopter_msgs::msg::Waypoint new_waypoint = roscopter_msgs::msg::Waypoint(); //

  new_waypoint.type = 1;

  new_waypoint.w = {0,0,-5};

  new_waypoint.speed = 1;

  new_waypoint.psi = 0.0;

    if (!waypoint_publisher_) {
        std::cout << "Publisher is null!" << std::endl;
        return;
}     
  waypoint_publisher_->publish(new_waypoint);
}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MinimalPublisher>());
    rclcpp::shutdown();
    return 0;
}
