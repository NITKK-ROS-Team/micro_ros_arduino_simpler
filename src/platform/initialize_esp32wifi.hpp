#pragma once

// Setting up ROS =================================================================================
#if defined(ESP32)

#include "../base/unique_executor.hpp"
#include "../base/ip_utils.hpp"
#include "../uros_function/common_uros_function.hpp"

#include "../base/config_uros_namespace.hpp"

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
        _allocator_size += strlen(_agent_port_header) + 9 + _slash_allocate_size;
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
        char *host_port_char = (char *)malloc(9);

        // _host_ip -> "192.168.xxx.yyy" -> get yyy
        char *host_ip_char = hostip_under_8bit(_host_ip);


        strcat(_result_namespace, _agent_port_header);
        strcat(_result_namespace, host_ip_char);
        sprintf(host_port_char, "_%d", _host_port);

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

#endif