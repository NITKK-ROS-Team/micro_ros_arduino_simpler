#pragma once

#define CONFIG_UROS_NAMESPACE_HPP_DEFINED

#define DATA_LENGTH 25

struct uros_ns
{
    char node_name[DATA_LENGTH];
    char node_namespace[DATA_LENGTH];
    char description[DATA_LENGTH];
    char ssid[DATA_LENGTH];
    char pass[DATA_LENGTH];
    char agent_ip_address[DATA_LENGTH];
    int agent_port;
    int auto_ns_detect;
    int agent_port_as_namespace;
};