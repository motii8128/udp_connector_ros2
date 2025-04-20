cmake_minimum_required(VERSION 3.5)
project(udp_connector_ros2)

find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()

ament_auto_add_library(target_udp_connector_ros2 SHARED
  DIRECTORY src
)

target_include_directories(target_udp_connector_ros2
  SYSTEM PUBLIC
)

rclcpp_components_register_node(target_udp_connector_ros2
  PLUGIN "udp_connector_ros2::UdpConnectorROS2"
  EXECUTABLE udp_connector_ros2_node
)

install(DIRECTORY
launch
config
DESTINATION share/${PROJECT_NAME}
)

ament_auto_package(
  INSTALL_TO_SHARE
)