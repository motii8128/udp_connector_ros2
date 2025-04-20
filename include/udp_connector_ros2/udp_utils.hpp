#ifndef UDP_UTILS_HPP_
#define UDP_UTILS_HPP_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <vector>

namespace udp_connector_ros2
{
    enum UDP_STATE
    {
        ERR_CREATE_SOCKET = 1,
        ERR_BIND_SOCKET = 2,
        ERR_UDP = 3,
        OK_UDP = 4,
    };

    class UdpHandler
    {
        public:
        UdpHandler(uint16_t port, std::string addr);

        UDP_STATE open();

        UDP_STATE receive();
        std::string get_packet();
        UDP_STATE send(std::string tx_packet, std::string dest_addr, uint16_t dest_port);

        private:
        int sock_fd_;
        sockaddr_in addr_;
        uint16_t port_;
        std::string addr_str_;

        std::string rx_packet_;
    };
}

#endif