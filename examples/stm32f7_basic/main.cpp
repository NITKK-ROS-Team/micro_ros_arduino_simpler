#include <Arduino.h>

// microros definition =====================================================
#include "micro_ros_arduino_simpler/simple.hpp"


#include <std_msgs/msg/bool.h>
#include <std_msgs/msg/int32.h>

extern rclc_executor_t executor;
extern rclc_support_t support;
extern rcl_allocator_t allocator;
extern rcl_node_t node;

rcl_publisher_t publisher;
std_msgs__msg__Int32 msg_int32;

rcl_subscription_t subscription;
std_msgs__msg__Bool msg_bool;
rcl_timer_t timer;
// ==========================================================================

int counter = 0;

// subscription callback ("/bool_data", "std_msgs/msg/bool") ================
void bool_callback(const void *msgin)
{
  const std_msgs__msg__Bool *_msg = (const std_msgs__msg__Bool *)msgin;
  counter += _msg->data;
}

// timer callback (10ms) ====================================================
void timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
  (void)last_call_time;
  (void)timer;

  msg_int32.data = counter;
  rcl_publish(&publisher, &msg_int32, NULL);
}

// setup micro_ros_arduino ===============================================
void setup()
{
// STM32F746NG -----------------------------------------------------------
#if defined(ARDUINO_ARCH_STM32)
  byte arduino_mac[] = {0xAA, 0xBB, 0xCC, 0xEE, 0xDD, 0xFF};
  IPAddress arduino_ip(192, 168, 10, 111);
  IPAddress agent_ip(192, 168, 10, 6);
  int agent_port = 2000;

  setup_microros_ethernet("uros_node", "ns", 2, arduino_mac, arduino_ip, agent_ip, agent_port);

// Generic Interface ------------------------------------------------------
#else
  // UART
  setup_microros_usb("microros_node", "ns", 2);
#endif

  // rclc-publisher-subscriber-timer ======================================
  rclc_publisher_init_default(&publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), "int32_data");

  rclc_create_timer_and_add(&timer, 10, timer_callback);
  rclc_create_subscription_and_add(&subscription, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool), &msg_bool, &bool_callback, "bool_data");
}

// loop micro_ros_arduino ===============================================
void loop()
{
  rclc_delay(10);
}