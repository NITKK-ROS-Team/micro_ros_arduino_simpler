#include <Arduino.h>

#if defined(ESP32)
#include <M5Atom.h>
// #include <M5Core2.h>
// #include <M5StickC.h>
#endif

// microros definition =====================================================
#include "micro_ros_arduino_simpler/simpler_base.h"
// #include "micro_ros_arduino_simpler/config_eeprom.h" // for ESP32

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
void bool_callback(const void* msgin)
{
    const std_msgs__msg__Bool* _msg = (const std_msgs__msg__Bool*)msgin;
    counter += _msg->data;
}

// timer callback (10ms) ====================================================
void timer_callback(rcl_timer_t* timer, int64_t last_call_time)
{
    (void)last_call_time;
    (void)timer;

    msg_int32.data = counter;
    rcl_publish(&publisher, &msg_int32, NULL);
}

// setup micro_ros_arduino ===============================================
void setup()
{

// ESP32 ---------------------------------------------------------------
#if defined(ESP32)
#ifdef LOAD_AGENT_ID_EEPROM_HPP_DEFINED
    // load config from eeprom-----
    uros_ns config = eeprom_load_agent_port(M5.Btn.isPressed());
    setup_microros_wifi(config, 2);
#else
    // Wi-Fi interface
    setup_microros_wifi("microros_node", "ns", 2, "ssid", "pass", "192.168.0.10", 2000);
#endif
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