# microROS-Arduino-Template

Template of microROS-Arduino on PlatformIO

## main.cpp

This is a sample program that counts the number of times a bool type is received and true is received.


```c++
#include <Arduino.h>
#include <M5Atom.h>

// microros definition =====================================================
// ### header
#include "microros_template/simple.hpp"
#include <std_msgs/msg/bool.h>
#include <std_msgs/msg/int32.h>

extern rclc_executor_t executor;
extern rclc_support_t support;
extern rcl_allocator_t allocator;
extern rcl_node_t node;

// ### message definition
rcl_publisher_t publisher;
std_msgs__msg__Int32 msg_int32;

rcl_subscription_t subscription;
std_msgs__msg__Bool msg_bool;
rcl_timer_t timer;
// ==========================================================================

unsigned int counter = 0;

// subscription callback ("/bool_data", "std_msgs/msg/bool") ================
void bool_callback(const void *msgin)
{
  const std_msgs__msg__Bool *_msg = (const std_msgs__msg__Bool *)msgin;
  counter += _msg->data;
}

// timer callback (10ms) ====================================================
void timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
  (void)last_call_time; (void)timer;

  msg_int32.data = counter;
  rcl_publish(&publisher, &msg_int32, NULL);
}

// setup micro_ros_arduino ===============================================
// ### setup
void setup()
{
  setup_microros_usb("microros_node", "", 2);
  // setup_microros_wifi("microros_node", "", 2, "ssid", "pass", "192.168.0.10", 2000);

  rclc_publisher_init_default(&publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), "int32_data");

  rclc_create_timer_and_add(&timer, 10, timer_callback);
  rclc_create_subscription_and_add(&subscription, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool), &msg_bool, &bool_callback, "bool_data");
}

void loop()
{
  rclc_delay(10);
}

```

### header
- include `"microros_template/simple.hpp"` to use microROS-Arduino-Template API.
- include ros-messages to publish or subscribe messages.

### message definition
- `rcl_publisher_t` : Equivalent to `rclcpp::Publisher<...>`.
- `std_msgs__msg__Int32` : Equivalent to `std_msgs::msg::Int32`.
- `rcl_subscription_t` : Equivalent to `rclcpp::Subscription<...>`.
- `rcl_timer_t timer` : Equivalent to `node->create_wall_timer`.

### void setup()
- Initialize of micro-ROS
  - (Wi-Fi): `setup_microros_wifi("microros_node", "", 2, "ssid", "pass", "192.168.0.10", 2000);`
    - node name : node name [e.g. `"microros_node"`]
    - namespace : namespace [e.g. `""`]
    - number_of_handle : Total count of subscriber and timer. [e.g. `2`]
    - ssid : SSID for Wi-Fi. [e.g. `"ssid"`]
    - pass : Password for Wi-Fi. [e.g. `"pass"`]
    - agent ip : IP address on host computer running microROS-Agent. [e.g. `"192.168.0.10"`]
    - port : Port number on host computer running microROS-Agent. [e.g. `2000`]
  - (USB) : `setup_microros_usb("microros_node", "", 2);`
    - node name : node name [e.g. `"microros_node"`]
    - namespace : namespace [e.g. `""`]
    - number_of_handle : Total count of subscriber and timer. [e.g. `2`]

<br>

- Definition of publisher (`rclc_publisher_init_default`)
  - `rclc_publisher_init_default` : Equivalent to `node->create_publisher`.
    - `ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32)` : Type of publish message.
    - `int32_data` : topic name
- Definition of timer (`rclc_create_timer_and_add`)
  - `rclc_create_timer_and_add` : Equivalent to `node->create_timer`.
    - `10` : timeout (ms)
    - `timer_callback` : Define a callback function for timeouts.
- Definition of subscriber (`rclc_create_subscription_and_add`)
  - `rclc_publisher_init_default` : Equivalent to `node->create_subscription`.
    - `ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool)` : Type of subscribe message.
    - `timer_callback` : Message chache for subscribe.
    - `"bool_data"` : Subscribe message name. (You have to publish `"bool_data"` on any ROS-node to call `timer_callback` on microROS)

### void loop()
- Arduino Loop. It can be written to work, but the basic idea is to use a timer callback.
  - rclc_delay : Equivalent to `rclcpp::spin_some()`.