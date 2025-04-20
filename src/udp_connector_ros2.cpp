#include "udp_connector_ros2/udp_connector_ros2.hpp"

namespace udp_connector_ros2
{
    UdpConnectorROS2::UdpConnectorROS2(const rclcpp::NodeOptions& options) : Node("UdpConnectorROS2", options)
    {
        port_ = this->declare_parameter("port", 64201);
        dest_port_ = this->declare_parameter("dest_port", 64202);
        addr_ = this->declare_parameter("addr", "127.0.0.1");
        dest_addr_ = this->declare_parameter("dest_addr", "127.0.0.1");
        tx_packet_ = this->declare_parameter("send_message", "Hello from ROS2");
        enable_debug_ = this->declare_parameter("enable_debug", false);

        udp_ = std::make_shared<UdpHandler>(port_, addr_);

        UDP_STATE state = udp_->open();
        if(state == UDP_STATE::ERR_CREATE_SOCKET)
        {
            RCLCPP_ERROR(this->get_logger(), "Failed to create socket.");
        }
        else if(state == UDP_STATE::ERR_BIND_SOCKET)
        {
            RCLCPP_ERROR(this->get_logger(), "Failed to bind address to socket.");
        }
        else if(state == UDP_STATE::OK_UDP)
        {
            RCLCPP_INFO(this->get_logger(), "Open UDP on %s:%d .", addr_.c_str(), port_);
        }

        publisher_ = this->create_publisher<std_msgs::msg::String>("/receive_data", rclcpp::SystemDefaultsQoS());
        timer_ = this->create_wall_timer(std::chrono::milliseconds(1), std::bind(&UdpConnectorROS2::timer_callback, this));

        RCLCPP_INFO(this->get_logger(), "Start %s.", this->get_name());
    }

    void UdpConnectorROS2::timer_callback()
    {
        UDP_STATE send_state = udp_->send(tx_packet_, dest_addr_, dest_port_);
        if(send_state == UDP_STATE::OK_UDP)
        {
            if(enable_debug_)RCLCPP_INFO(this->get_logger(), "Send packet to %s:%d", dest_addr_.c_str(), dest_port_);
        }
        else if(send_state == UDP_STATE::ERR_UDP)
        {
            if(enable_debug_)RCLCPP_ERROR(this->get_logger(), "Failed to send packet to %s:%d", dest_addr_.c_str(), dest_port_);
        }

        UDP_STATE recv_state = udp_->receive();
        if(recv_state == UDP_STATE::OK_UDP)
        {
            if(enable_debug_)RCLCPP_INFO(this->get_logger(), "Recv packet.");

            auto msg = std_msgs::msg::String();
            msg.data = udp_->get_packet();

            if(enable_debug_)RCLCPP_INFO(this->get_logger(), "Publish [%s] .", msg.data.c_str());

            publisher_->publish(msg);
        }
        else if(recv_state == UDP_STATE::ERR_UDP)
        {
            if(enable_debug_)RCLCPP_ERROR(this->get_logger(), "Failed to recv packet.");
        }
    }
}

#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(udp_connector_ros2::UdpConnectorROS2)