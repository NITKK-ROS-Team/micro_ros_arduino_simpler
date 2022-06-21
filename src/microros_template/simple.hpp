#pragma once

// microros ===================================================================
#include <micro_ros_arduino.h>

#include <stdio.h>
#include <rcl/rcl.h>
#include <rcl/error_handling.h>

#include <rclc/rclc.h>
#include <rclc/executor.h>
// ============================================================================
#include "config_uros_namespace.hpp"

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

#ifdef CONFIG_UROS_NAMESPACE_HPP_DEFINED
int setup_microros_wifi(uros_ns _config, int _total_callback_count)
{
    char _ssid[DATA_LENGTH];
    char _password[DATA_LENGTH];
    char _host_ip[DATA_LENGTH];
    int _host_port;
    char _node_name[DATA_LENGTH];
    char _namespace[DATA_LENGTH];
    bool _auto_ns_detect;
    bool _agent_port_as_namespace;

    char _agent_port_header[4] = "ag_";
    char _my_ip_header[4] = "ip_";

    char *_result_namespace;
    int _allocator_size = 0;

    strcpy(_ssid, _config.ssid);
    strcpy(_password, _config.pass);
    strcpy(_host_ip, _config.agent_ip_address);
    _host_port = _config.agent_port;
    strcpy(_node_name, _config.node_name);
    strcpy(_namespace, _config.node_namespace);
    _auto_ns_detect = _config.auto_ns_detect;
    _agent_port_as_namespace = _config.agent_port_as_namespace;

    set_microros_wifi_transports(_ssid, _password, _host_ip, _host_port);
    Serial.println("Connected to WiFi");
    get_default_allocator();

    int _slash_allocate_size =1;

    // /ag_port/ip_addr/ns/node_name
    if (_agent_port_as_namespace)
    {
        // port -> 5 chars
        _allocator_size += strlen(_agent_port_header) + 5 + _slash_allocate_size;
    }
    if (_auto_ns_detect)
    {
        _allocator_size += strlen(_my_ip_header) + 7 + _slash_allocate_size;
    }
    if (strlen(_namespace) > 0)
    {
        _allocator_size += strlen(_namespace);
    }
    _allocator_size += 1; // to add '\0'

    _result_namespace = (char *)malloc(_allocator_size);
// -----------------------------------------------------------------------------
    if (_agent_port_as_namespace)
    {
        char *host_port_char = (char *)malloc(5);

        strcat(_result_namespace, _agent_port_header);
        sprintf(host_port_char, "%d", _host_port);

        strcat(_result_namespace, host_port_char);
        if (_auto_ns_detect || strlen(_namespace) > 0)
        {
            strcat(_result_namespace, "/");
        }
        free(host_port_char);
    }
    if (_auto_ns_detect)
    {
        strcat(_result_namespace, _my_ip_header);
        strcat(_result_namespace, ipToString_under_16bit(WiFi.localIP()));
         if (strlen(_namespace) > 0)
        {
            strcat(_result_namespace, "/");
        }
    }
    if (strlen(_namespace) > 0)
    {
        strcat(_result_namespace, _namespace);
    }
    else
    {
        strcat(_result_namespace, "\0");
    }

    Serial.print("Namespace: ");
    Serial.println(String(_result_namespace));

    rclc_init(_node_name, _result_namespace);
    init_executor(_total_callback_count);

    free(_result_namespace);
    return 0;
}
#endif

int setup_microros_wifi(char *_node_name, char *_namespace, int _total_callback_count, char *_ssid, char *_password, char *_host_ip, int _host_port = 2000, bool _auto_ns_detect = false)
{
    set_microros_wifi_transports(_ssid, _password, _host_ip, _host_port);
    Serial.println("Connected to WiFi");
    get_default_allocator();

    char *_result_namespace;

    if(_auto_ns_detect && strlen(_namespace) == 0)
    {
        _result_namespace = (char *)malloc(10);
        sprintf(_result_namespace, "ip_%s", ipToString_under_16bit(WiFi.localIP()));
    }
    else if (_auto_ns_detect)
    {
        _result_namespace = (char *)malloc(strlen(_namespace) + 10);
        sprintf(_result_namespace, "ip_%s/%s", ipToString_under_16bit(WiFi.localIP()), _namespace);
        Serial.println(_result_namespace);
    }
    else
    {
        _result_namespace = (char *)malloc(strlen(_namespace) + 1);
        strcpy(_result_namespace, _namespace);
    }

    rclc_init(_node_name, _result_namespace);
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