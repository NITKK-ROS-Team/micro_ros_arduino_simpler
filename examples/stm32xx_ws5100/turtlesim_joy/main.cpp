#include <Arduino.h>

// microros definition =====================================================
#include "micro_ros_arduino_simpler/simpler_base.h"
#include <geometry_msgs/msg/twist.h>

extern rclc_executor_t executor;
extern rclc_support_t support;
extern rcl_allocator_t allocator;
extern rcl_node_t node;

rcl_publisher_t publisher;
geometry_msgs__msg__Twist msg_twist;

rcl_timer_t timer;
// ==========================================================================

int counter = 0;
// timer callback (10ms) ====================================================
void timer_callback(rcl_timer_t* timer, int64_t last_call_time)
{
    (void)last_call_time;
    (void)timer;

    // read analog value
    int analog_x = analogRead(A0);
    int analog_y = analogRead(A1);
    // -5.0 to 5.0
    float x = analog_x * (5.0 / 1023.0) - 2.5;
    float y = -1 * (analog_y * (5.0 / 1023.0) - 2.5);
    msg_twist.linear.x = x;
    msg_twist.angular.z = y;
    rcl_publish(&publisher, &msg_twist, NULL);
}

// setup micro_ros_arduino ===============================================
void setup()
{
#if !defined(STM32L4xx)
#error "This example is only for STM32L4xx"
#endif

    // A0, A1
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);

    byte arduino_mac[] = {0xAA, 0xBB, 0xCC, 0xEE, 0xDD, 0xFF};
    IPAddress arduino_ip(192, 168, 0, 111);
    IPAddress agent_ip(192, 168, 0, 8);
    int agent_port = 2000;
    int cs = 6;

    setup_microros_ethernet("twist_joy", "turtle1", 1, arduino_mac, arduino_ip, agent_ip, agent_port, false, cs);

    // rclc-publisher-subscriber-timer ======================================
    rclc_publisher_init_default(&publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(geometry_msgs, msg, Twist), "cmd_vel");
    rclc_create_timer_and_add(&timer, 10, timer_callback);
}

// loop micro_ros_arduino ===============================================
void loop()
{
    rclc_delay(10);
}
