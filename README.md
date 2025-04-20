# udp_connector_ros2
外部とUDP通信するROS2パッケージ。Ubuntu24.05　ROS2 Jazzyで動作確認済み

# 使用方法
## パラメーター
パラメーターをconfig/config.yamlから設定することができます。
|名前|初期値|概要|
|:--:|:--:|:--:|
|enable_debug|false|デバッグログを出力するかどうかを決める。|
|addr|"127.0.0.1"|IPアドレスを指定する|
|port|64201|UDPのポートを指定する|
|dest_addr|"127.0.0.1"|通信相手のIPアドレスを指定する|
|dest_port|64202|通信相手のポートを指定する|
|send_message|"Hello from ROS2"|通信相手に送り返す内容を指定する|

## 実行
```
ros2 launch udp_connector_ros2 udp_connector_ros2.launch.xml
```
