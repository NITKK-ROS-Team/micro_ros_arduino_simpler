#pragma once

#define LOAD_AGENT_ID_EEPROM_HPP_DEFINED

#include <Arduino.h>
#include <EEPROM.h>

#include "config_uros_namespace.hpp"

uros_ns uros_namespace_config;

void clear_data()
{
    while (Serial.available())
    {
        Serial.read();
    }
}

void save_data(uros_ns _data)
{
    EEPROM.put<uros_ns>(0, _data);
    EEPROM.commit();
}

// --------------------------------------------------

bool serial_input_return_chararray(char *_input_char, bool anonymous=false)
{
    char c;
    bool _skiped = false;
    while (c != '\n')
    {
        if (Serial.available())
        {
            c = Serial.read();
            if (c == '\n')
            {
                _skiped = false;
                break;
            }
            else if (c == '/')
            {
                Serial.println("Skip!");
                _input_char++;
                _skiped = true;
                break;
            }
            else
            {
                *_input_char = c;
                _input_char++;
                if (anonymous)
                {
                    Serial.print("*");
                }
                else
                {
                    Serial.print(c);
                }
            }
        }
        delay(10);
    }
    *_input_char = '\0';
    return _skiped;
}

void input_node_name()
{

    // if read something from serial, escape
    bool sended = false;
    while (sended == false)
    {
        char out_char = 'a';

        if (Serial.available())
        {
            char c = Serial.read();
            sended = true;
        }
        else
        {
            Serial.write(out_char);
        }
        delay(10);
    }

    Serial.print("Input node-name address");
    Serial.print(" input '/' to skip: ");

    char input_char[DATA_LENGTH];
    bool skiped = false;

    skiped = serial_input_return_chararray(input_char);
    if (!skiped)
    {
        strcpy(uros_namespace_config.node_name, input_char);
    }
    Serial.println("");
}

void input_node_namespace()
{
    Serial.print("Input node_namespace address");
    Serial.print(" input '/' to skip: ");

    char input_char[DATA_LENGTH];
    bool skiped = false;

    skiped = serial_input_return_chararray(input_char);
    if (!skiped)
    {
        strcpy(uros_namespace_config.node_namespace, input_char);
    }
    Serial.println("");
}

void input_description()
{
    Serial.print("Input description address");
    Serial.print(" input '/' to skip: ");

    char input_char[DATA_LENGTH];
    bool skiped = false;

    skiped = serial_input_return_chararray(input_char);
    if (!skiped)
    {
        strcpy(uros_namespace_config.description, input_char);
    }
    Serial.println("");
}

void input_ssid()
{
    Serial.print("Input SSID address");
    Serial.print(" input '/' to skip: ");

    char input_char[DATA_LENGTH];
    bool skiped = false;

    skiped = serial_input_return_chararray(input_char);
    if (!skiped)
    {
        strcpy(uros_namespace_config.ssid, input_char);
    }
    Serial.println("");
}

void input_pass()
{
    Serial.print("Input Password address");
    Serial.print(" input '/' to skip: ");

    char input_char[DATA_LENGTH];
    bool skiped = false;

    skiped = serial_input_return_chararray(input_char, true);
    if (!skiped)
    {
        strcpy(uros_namespace_config.pass, input_char);
    }
    Serial.println("");
}

void input_agent_ip_address()
{
    Serial.print("Input IP address");
    Serial.print(" input '/' to skip: ");

    char input_char[DATA_LENGTH];
    bool skiped = false;

    skiped = serial_input_return_chararray(input_char);
    if (!skiped)
    {
        strcpy(uros_namespace_config.agent_ip_address, input_char);
    }
    Serial.println("");
}

void input_agent_id()
{
    Serial.print("Input agent-ID");
    Serial.print(" input '/' to skip: ");

    char input_char[DATA_LENGTH];
    bool skiped = false;

    clear_data();
    skiped = serial_input_return_chararray(input_char);
    if (!skiped)
    {
        int port = atoi(input_char);
        uros_namespace_config.agent_port = port;
    }
    Serial.println("");
}

void input_auto_ns_detect()
{
    Serial.print("Input agent-ns-detect (1 or 0)");
    Serial.print(" input '/' to skip: ");

    char input_char[DATA_LENGTH];
    bool skiped = false;

    clear_data();
    skiped = serial_input_return_chararray(input_char);
    if (!skiped)
    {
        int _auto_detect = atoi(input_char);
        uros_namespace_config.auto_ns_detect = _auto_detect;
    }
    Serial.println("");
}

void input_agent_port_as_namespace()
{
    Serial.print("Input agent-port-as-namespace (1 or 0)");
    Serial.print(" input '/' to skip: ");

    char input_char[DATA_LENGTH];
    bool skiped = false;

    clear_data();
    skiped = serial_input_return_chararray(input_char);
    if (!skiped)
    {
        int _agent_port_as_namespace = atoi(input_char);
        uros_namespace_config.agent_port_as_namespace = _agent_port_as_namespace;
    }
    Serial.println("");
}
// --------------------------------------------------

uros_ns load_data(bool _btn_is_pressed)
{
    EEPROM.get<uros_ns>(0, uros_namespace_config);

    Serial.println("");
    Serial.println("-------- Load data ----------");
    Serial.println("Node name: " + String(uros_namespace_config.node_name));
    Serial.println("Node namespace: " + String(uros_namespace_config.node_namespace));
    Serial.println("Description: " + String(uros_namespace_config.description));
    Serial.println("SSID: " + String(uros_namespace_config.ssid));
    // Serial.println("PASS: " + String(uros_namespace_config.pass));
    Serial.println("PASS: ******");
    Serial.println("IP: " + String(uros_namespace_config.agent_ip_address));
    Serial.println("port: " + String(uros_namespace_config.agent_port));
    Serial.println("auto_ns_detect: " + String(uros_namespace_config.auto_ns_detect));
    Serial.println("agent_port_as_namespace: " + String(uros_namespace_config.agent_port_as_namespace));
    Serial.println("------------------------------");

    if (!_btn_is_pressed)
    {
        Serial.println("Skip");
        return uros_namespace_config;
    }

    input_node_name();
    input_node_namespace();
    input_description();

    input_ssid();
    input_pass();
    input_agent_ip_address();
    input_agent_id();

    input_auto_ns_detect();
    input_agent_port_as_namespace();


    Serial.println("");
    Serial.println("-------- ReWrite data ----------");
    Serial.println("Node name: " + String(uros_namespace_config.node_name));
    Serial.println("Node namespace: " + String(uros_namespace_config.node_namespace));
    Serial.println("Description: " + String(uros_namespace_config.description));
    Serial.println("SSID: " + String(uros_namespace_config.ssid));
    // Serial.println("PASS: " + String(uros_namespace_config.pass));
    Serial.println("PASS: ******");
    Serial.println("IP: " + String(uros_namespace_config.agent_ip_address));
    Serial.println("port: " + String(uros_namespace_config.agent_port));
    Serial.println("auto_ns_detect: " + String(uros_namespace_config.auto_ns_detect));
    Serial.println("agent_port_as_namespace: " + String(uros_namespace_config.agent_port_as_namespace));
    Serial.println("------------------------------");

    save_data(uros_namespace_config);
    EEPROM.end();
    return uros_namespace_config;
}

uros_ns eeprom_load_agent_port(int _btn_is_pressed)
{
    EEPROM.begin(sizeof(uros_ns));
    return load_data(_btn_is_pressed);
}
