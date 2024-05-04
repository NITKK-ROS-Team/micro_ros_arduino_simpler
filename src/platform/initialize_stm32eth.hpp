#pragma once

#if defined(ARDUINO_ARCH_STM32) || defined(ARDUINO_ARCH_RENESAS)

#include "../base/unique_executor.hpp"
#include "../base/ip_utils.hpp"
#include "../uros_function/common_uros_function.hpp"

int setup_microros_ethernet(char *_node_name, char *_namespace, int _total_callback_count, byte *_mac_adder, IPAddress _my_ip, IPAddress _host_ip, int _host_port = 2000, bool _auto_ns_detect = false, int _cs= -1)
{
    if (_cs == - 1)
    {
        set_microros_native_ethernet_udp_transports(_mac_adder, _my_ip, _host_ip, _host_port);
    }
    else
    {
        set_microros_native_ethernet_udp_transports(_mac_adder, _my_ip, _host_ip, _host_port, _cs);
    }

    int status_ethernet = Ethernet.hardwareStatus();
    Serial.println(status_ethernet);
    get_default_allocator();

    char *_result_namespace;

    if(_auto_ns_detect && strlen(_namespace) == 0)
    {
        _result_namespace = (char *)malloc(10);
        sprintf(_result_namespace, "ip_%s", ipToString_under_16bit(_host_ip));
    }
    else if (_auto_ns_detect)
    {
        _result_namespace = (char *)malloc(strlen(_namespace) + 10);
        sprintf(_result_namespace, "ip_%s/%s", ipToString_under_16bit(_host_ip), _namespace);
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

#endif
