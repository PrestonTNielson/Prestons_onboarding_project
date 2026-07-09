#include "maze_publisher.hpp"
#include <iostream>

float counter = 0;
float North_wall_distance = 0;
float East_wall_distance = 0;
float South_wall_distance = 0;
float West_wall_distance = 0;
direction Where_I_Came_From = SOUTH;
double current_position[3] = {0, 0, 0};
copter_state current_state = INIT;
roscopter_msgs::msg::Waypoint new_waypoint = roscopter_msgs::msg::Waypoint();
void MinimalPublisher::sensor_callback(const std_msgs::msg::Float32MultiArray::SharedPtr msg) {
    counter = 0;
   // std::cout << "Received sensor data: ";
    for (const auto& value : msg->data) {
       // std::cout << value << " ";
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

   // std::cout << std::endl;
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

roscopter_msgs::msg::Waypoint calculate_next_waypoint() {
    roscopter_msgs::msg::Waypoint next_waypoint;
    // next_waypoint.w = {0, 0, -5};
    // next_waypoint.type = 1;
    // next_waypoint.speed = 1;
    // next_waypoint.psi = 0.0;
    // if (North_wall_distance < 10000 && North_wall_distance > East_wall_distance && North_wall_distance > West_wall_distance && Where_I_Came_From != NORTH) {
    //     next_waypoint.w = {current_position[0] + North_wall_distance - 1, current_position[1], -10};
    //     next_waypoint.type = 1;
    //     next_waypoint.speed = 1;
    //     next_waypoint.psi = 0.0;
    //     Where_I_Came_From = SOUTH;
    // } else if (East_wall_distance < 10000 && East_wall_distance > North_wall_distance && East_wall_distance > South_wall_distance && Where_I_Came_From != EAST) {
    //     next_waypoint.w = {current_position[0], current_position[1] + East_wall_distance - 1, -10};
    //     next_waypoint.type = 1;
    //     next_waypoint.speed = 1;
    //     next_waypoint.psi = 0.0;
    //     Where_I_Came_From = WEST;
    // } else if (South_wall_distance < 10000 && South_wall_distance > East_wall_distance && South_wall_distance > West_wall_distance && Where_I_Came_From != SOUTH) {
    //     next_waypoint.w = {current_position[0] - South_wall_distance + 1, current_position[1], -10};
    //     next_waypoint.type = 1;
    //     next_waypoint.speed = 1;
    //     next_waypoint.psi = 0.0;
    //     Where_I_Came_From = NORTH;
    // } else if (West_wall_distance < 10000 && West_wall_distance > North_wall_distance && West_wall_distance > South_wall_distance && Where_I_Came_From != WEST) {
    //     next_waypoint.w = {current_position[0], current_position[1] - West_wall_distance + 1, -10};
    //     next_waypoint.type = 1;
    //     next_waypoint.speed = 1;
    //     next_waypoint.psi = 0.0;
    //     Where_I_Came_From = EAST;
    if (Where_I_Came_From == SOUTH) {
        if (North_wall_distance < 10000 && North_wall_distance > East_wall_distance && North_wall_distance > West_wall_distance) {
            next_waypoint.w = {current_position[0] + North_wall_distance - 2, current_position[1], -7};
            next_waypoint.type = 1;
            next_waypoint.speed = 3;
            next_waypoint.psi = 0.0;
            Where_I_Came_From = SOUTH;
        } else if (East_wall_distance < 10000 && East_wall_distance > North_wall_distance && East_wall_distance > West_wall_distance) {
            next_waypoint.w = {current_position[0], current_position[1] + East_wall_distance - 2, -7};
            next_waypoint.type = 1;
            next_waypoint.speed = 3;
            next_waypoint.psi = 0.0;
            Where_I_Came_From = WEST;
        } else if (West_wall_distance < 10000 && West_wall_distance > North_wall_distance && West_wall_distance > East_wall_distance) {
            next_waypoint.w = {current_position[0], current_position[1] - West_wall_distance + 2, -7};
            next_waypoint.type = 1;
            next_waypoint.speed = 3;
            next_waypoint.psi = 0.0;
            Where_I_Came_From = EAST;
        }

    } else if (Where_I_Came_From == WEST) {
        if (North_wall_distance < 10000 && North_wall_distance > East_wall_distance && North_wall_distance > South_wall_distance) {
            next_waypoint.w = {current_position[0] + North_wall_distance - 2, current_position[1], -7};
            next_waypoint.type = 1;
            next_waypoint.speed = 3;
            next_waypoint.psi = 0.0;
            Where_I_Came_From = SOUTH;
        } else if (East_wall_distance < 10000 && East_wall_distance > North_wall_distance && East_wall_distance > South_wall_distance) {
            next_waypoint.w = {current_position[0], current_position[1] + East_wall_distance - 2, -7};
            next_waypoint.type = 1;
            next_waypoint.speed = 3;
            next_waypoint.psi = 0.0;
            Where_I_Came_From = WEST;
        } else if (South_wall_distance < 10000 && South_wall_distance > East_wall_distance && South_wall_distance > North_wall_distance) {
            next_waypoint.w = {current_position[0] - South_wall_distance + 2, current_position[1], -7};
            next_waypoint.type = 1;
            next_waypoint.speed = 3;
            next_waypoint.psi = 0.0;
            Where_I_Came_From = NORTH;
        }
    } else if (Where_I_Came_From == NORTH) {
        if (East_wall_distance < 10000 && East_wall_distance > North_wall_distance && East_wall_distance > South_wall_distance) {
            next_waypoint.w = {current_position[0], current_position[1] + East_wall_distance - 2, -7};
            next_waypoint.type = 1;
            next_waypoint.speed = 3;
            next_waypoint.psi = 0.0;
            Where_I_Came_From = WEST;
        } else if (South_wall_distance < 10000 && South_wall_distance > East_wall_distance && South_wall_distance > West_wall_distance) {
            next_waypoint.w = {current_position[0] - South_wall_distance + 2, current_position[1], -7};
            next_waypoint.type = 1;
            next_waypoint.speed = 3;
            next_waypoint.psi = 0.0;
            Where_I_Came_From = NORTH;
        } else if (West_wall_distance < 10000 && West_wall_distance > North_wall_distance && West_wall_distance > South_wall_distance) {
            next_waypoint.w = {current_position[0], current_position[1] - West_wall_distance + 2, -7};
            next_waypoint.type = 1;
            next_waypoint.speed = 3;
            next_waypoint.psi = 0.0;
            Where_I_Came_From = EAST;
        }
    } else if (Where_I_Came_From == EAST) {
        if (North_wall_distance < 10000 && North_wall_distance > East_wall_distance && North_wall_distance > South_wall_distance) {
            next_waypoint.w = {current_position[0] + North_wall_distance - 2, current_position[1], -7};
            next_waypoint.type = 1;
            next_waypoint.speed = 3;
            next_waypoint.psi = 0.0;
            Where_I_Came_From = SOUTH;
        } else if (South_wall_distance < 10000 && South_wall_distance > East_wall_distance && South_wall_distance > West_wall_distance) {
            next_waypoint.w = {current_position[0] - South_wall_distance + 2, current_position[1], -7};
            next_waypoint.type = 1;
            next_waypoint.speed = 3;
            next_waypoint.psi = 0.0;
            Where_I_Came_From = NORTH;
        } else if (West_wall_distance < 10000 && West_wall_distance > North_wall_distance && West_wall_distance > South_wall_distance) {
            next_waypoint.w = {current_position[0], current_position[1] - West_wall_distance + 2, -7};
            next_waypoint.type = 1;
            next_waypoint.speed = 3;
            next_waypoint.psi = 0.0;
            Where_I_Came_From = EAST;
        }


    } else {
        std::cout << "No valid direction to move!" << std::endl;
        std::cout << "North_wall_distance: " << North_wall_distance << std::endl;
        std::cout << "East_wall_distance: " << East_wall_distance << std::endl;
        std::cout << "South_wall_distance: " << South_wall_distance << std::endl;
        std::cout << "West_wall_distance: " << West_wall_distance << std::endl;
        std::cout << "Where_I_Came_From: " << Where_I_Came_From << std::endl;

    }
    return next_waypoint;
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
    float distance_to_next_waypoint;
      //sm action logic
    switch (current_state) {
    
    case INIT:
        break;

    case WAIT:
        break;

    case NEXT_WAYPOINT:
        new_waypoint = calculate_next_waypoint();
        waypoint_publisher_->publish(new_waypoint);
        std::cout << "Published waypoint: (" << new_waypoint.w[0] << ", " << new_waypoint.w[1] << ", " << new_waypoint.w[2] << ")" << std::endl;
        break;
    
  }
  //sm transition logic
    switch (current_state) {
    
    case INIT:
        current_state = NEXT_WAYPOINT;
        break;
    
    case WAIT:
        distance_to_next_waypoint = std::sqrt(std::pow(current_position[0] - new_waypoint.w[0], 2) +
            std::pow(current_position[1] - new_waypoint.w[1], 2) +
            std::pow(current_position[2] - new_waypoint.w[2], 2));
        if (distance_to_next_waypoint < 1.5) {
            current_state = NEXT_WAYPOINT;
        }
        std::cout << "Current position: (" << current_position[0] << ", " << current_position[1] << ", " << current_position[2] << ")" << std::endl;
        std::cout << "Next waypoint: (" << new_waypoint.w[0] << ", " << new_waypoint.w[1] << ", " << new_waypoint.w[2] << ")" << std::endl;
        std::cout << "Distance to next waypoint: " << distance_to_next_waypoint << std::endl;
        break;
    
    case NEXT_WAYPOINT:
        std::cout << "Transitioning to WAIT state" << std::endl;
        current_state = WAIT;
        break;
  }



    if (!waypoint_publisher_) {
        std::cout << "Publisher is null!" << std::endl;
        return;
}     
    new_waypoint.hold_indefinitely = false;
}

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MinimalPublisher>());
    rclcpp::shutdown();
    return 0;
}
