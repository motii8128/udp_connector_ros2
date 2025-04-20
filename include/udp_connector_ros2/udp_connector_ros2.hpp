#ifndef UDP_CONNECTOR_ROS2_HPP_
#define UDP_CONNECTOR_ROS2_HPP_

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include <chrono>

#include "udp_utils.hpp"

namespace udp_connector_ros2
{
    class UdpConnectorROS2: public rclcpp::Node
    {
        public:
        explicit UdpConnectorROS2(const rclcpp::NodeOptions& options = rclcpp::NodeOptions());

        void timer_callback();

        private:
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;

        std::shared_ptr<UdpHandler> udp_;

        uint16_t port_, dest_port_;
        std::string addr_, dest_addr_, tx_packet_;

        bool enable_debug_;
    };
}

#endif