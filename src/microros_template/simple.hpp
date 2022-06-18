#pragma once

// microros ===================================================================
#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>

#include <rclc/rclc.h>
#include <rclc/executor.h>
// ============================================================================

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

#if !defined(ESP32) && !defined(TARGET_PORTENTA_H7_M7) && !defined(ARDUINO_NANO_RP2040_CONNECT)
#error This example is only avaible for Arduino Portenta, Arduino Nano RP2040 Connect and ESP32 Dev module
#endif

#define LED_PIN 13

#define RCCHECK(fn)                  \
    {                                \
        rcl_ret_t temp_rc = fn;      \
        if ((temp_rc != RCL_RET_OK)) \
        {                            \
            error_loop();            \
        }                            \
    }
#define RCSOFTCHECK(fn)              \
    {                                \
        rcl_ret_t temp_rc = fn;      \
        if ((temp_rc != RCL_RET_OK)) \
        {                            \
        }                            \
    }

void error_loop()
{
    while (1)
    {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN));
        delay(100);
    }
}

// rclc init ===================================================================
void rclc_init(const char *_node_name, const char *_namespace)
{
    rclc_support_init(&support, 0, NULL, &allocator);
    rclc_node_init_default(&node, _node_name, _namespace, &support);
}

void get_default_allocator()
{
    allocator = rcl_get_default_allocator();
}

void init_executor(int _num_of_callback)
{
    rclc_executor_init(&executor, &support.context, (size_t)_num_of_callback, &allocator);
}

// setup micro_ros_arduino =====================================================
char *ipToString_under_16bit(uint32_t ip)
{
    char *result = (char *)malloc(7);

    sprintf(result, "%d_%d",
            (ip >> 16) & 0xFF,
            (ip >> 24) & 0xFF);

    return result;
}

int setup_microros_wifi(char *_node_name, char *_namespace, int _total_callback_count, char *_wifi_ssid, char *_wifi_password, char *host_ip, int host_port = 2000, bool _auto_ns_detect = false)
{
    set_microros_wifi_transports(_wifi_ssid, _wifi_password, host_ip, host_port);
    get_default_allocator();

    char *result_namespace;

    if (_auto_ns_detect && strlen(_namespace) == 0)
    {
        result_namespace = (char *)malloc(10);
        sprintf(result_namespace, "ip_%s", ipToString_under_16bit(WiFi.localIP()));
    }
    else if (_auto_ns_detect)
    {
        result_namespace = (char *)malloc(strlen(_namespace) + 10);
        sprintf(result_namespace, "ip_%s/%s", ipToString_under_16bit(WiFi.localIP()), _namespace);
        Serial.println(result_namespace);
    }
    else
    {
        result_namespace = (char *)malloc(strlen(_namespace) + 1);
        strcpy(result_namespace, _namespace);
    }

    rclc_init(_node_name, result_namespace);
    init_executor(_total_callback_count);
    return 0;
}

int setup_microros_usb(char *_node_name, char *_namespace, int _total_callback_count)
{
    set_microros_transports();
    get_default_allocator();

    rclc_init(_node_name, _namespace);
    init_executor(_total_callback_count);
    return 0;
}

// rclc_delay for "void loop()" ===================================================================
void rclc_delay(int _ms)
{
    delay(_ms);
    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(_ms));
}

// create timer and add to executor ==============================================================
void rclc_create_timer_and_add(rcl_timer_t *timer, unsigned int _timeout_ms, rcl_timer_callback_t _callback)
{
    rclc_timer_init_default(timer, &support, RCL_MS_TO_NS(_timeout_ms), _callback);
    rclc_executor_add_timer(&executor, timer);
}

// create subscription and add to executor =========================================================
void rclc_create_subscription_and_add(rcl_subscription_t *_subscription, const rosidl_message_type_support_t *_type_support, void *msg, rclc_subscription_callback_t _callback, char *_topic_name, const rclc_executor_handle_invocation_t _invocation = ON_NEW_DATA)
{
    rclc_subscription_init_default(_subscription, &node, _type_support, _topic_name);
    rclc_executor_add_subscription(&executor, _subscription, &msg, _callback, _invocation);
}
