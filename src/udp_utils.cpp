#include "udp_connector_ros2/udp_utils.hpp"

namespace udp_connector_ros2
{
    UdpHandler::UdpHandler(uint16_t port, std::string addr)
    {
        port_ = port;
        addr_str_ = addr;
    }

    UDP_STATE UdpHandler::open()
    {
        sock_fd_ = socket(AF_INET, SOCK_DGRAM, 0);

        if(sock_fd_ < 0)
        {
            close(sock_fd_);
            return UDP_STATE::ERR_CREATE_SOCKET;
        }

        addr_ = sockaddr_in{};
        addr_.sin_family = AF_INET;
        addr_.sin_port = htons(port_);
        addr_.sin_addr.s_addr = inet_addr(addr_str_.c_str());

        if(bind(sock_fd_, (struct sockaddr*)&addr_, sizeof(addr_)) < 0)
        {
            close(sock_fd_);
            return UDP_STATE::ERR_BIND_SOCKET;
        }

        return UDP_STATE::OK_UDP;
    }

    UDP_STATE UdpHandler::receive()
    {
        char buf[512];

        ssize_t len = recv(sock_fd_, buf, sizeof(buf)-1, 0);

        if(len > 0)
        {
            buf[len] = '\0';

            rx_packet_ = std::string(buf);

            return UDP_STATE::OK_UDP;
        }
        else
        {
            close(sock_fd_);
            return UDP_STATE::ERR_UDP;
        }
    }

    std::string UdpHandler::get_packet()
    {
        return rx_packet_;
    }

    UDP_STATE UdpHandler::send(std::string tx_packet, std::string dest_addr, uint16_t dest_port)
    {
        sockaddr_in dest_addr_{};
        dest_addr_.sin_family = AF_INET;
        dest_addr_.sin_port = htons(dest_port);
        inet_pton(AF_INET, dest_addr.c_str(), &dest_addr_.sin_addr);

        ssize_t sent = sendto(sock_fd_, tx_packet.c_str(), tx_packet.size(), 0, (sockaddr*)&dest_addr_, sizeof(dest_addr_));

        if(sent > 0)
        {
            return UDP_STATE::OK_UDP;
        }
        else
        {
            close(sock_fd_);
            return UDP_STATE::ERR_UDP;
        }
    }
}