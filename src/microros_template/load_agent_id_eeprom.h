#pragma once

#include <Arduino.h>
#include <EEPROM.h>

#define DATA_LENGTH 25

struct DATA_SET
{
    char node_name[DATA_LENGTH];
    char node_namespace[DATA_LENGTH];
    char description[DATA_LENGTH];
    char ssid[DATA_LENGTH];
    char pass[DATA_LENGTH];
    char agent_ip_address[DATA_LENGTH];
    int agent_port;
};
DATA_SET uros_config;

void clear_data()
{
    while (Serial.available())
    {
        Serial.read();
    }
}

void save_data(DATA_SET _data)
{
    EEPROM.put<DATA_SET>(0, _data);
    EEPROM.commit();
}

// --------------------------------------------------
void input_node_name()
{
    Serial.print("Input node-name address");
    Serial.println(" input '/' to skip): ");

    char c;
    int i = 0;
    char input_char[DATA_LENGTH];
    bool skiped = false;

    while (c != '\n')
    {
        if (Serial.available())
        {
            c = Serial.read();
            if (c == '\n')
            {
                break;
            }
            else if (c == '/')
            {
                Serial.println("Skip!");
                skiped = true;
                break;
            }
            else
            {
                input_char[i] = c;
                i++;
                Serial.print(c);
            }
        }
        delay(10);
    }
    if (!skiped)
    {
        input_char[i] = '\0';
        strcpy(uros_config.node_name, input_char);
    }
    Serial.println("");
}

void input_node_namespace()
{
    Serial.print("Input node_namespace address");
    Serial.println(" input '/' to skip): ");

    char c;
    int i = 0;
    char input_char[DATA_LENGTH];
    bool skiped = false;

    while (c != '\n')
    {
        if (Serial.available())
        {
            c = Serial.read();
            if (c == '\n')
            {
                break;
            }
            else if (c == '/')
            {
                Serial.println("Skip!");
                skiped = true;
                break;
            }
            else
            {
                input_char[i] = c;
                i++;
                Serial.print(c);
            }
        }
        delay(10);
    }
    if (!skiped)
    {
        input_char[i] = '\0';
        strcpy(uros_config.node_namespace, input_char);
    }
    Serial.println("");
}

void input_description()
{
    Serial.print("Input description address");
    Serial.println(" input '/' to skip): ");

    char c;
    int i = 0;
    char input_char[DATA_LENGTH];
    bool skiped = false;

    while (c != '\n')
    {
        if (Serial.available())
        {
            c = Serial.read();
            if (c == '\n')
            {
                break;
            }
            else if (c == '/')
            {
                Serial.println("Skip!");
                skiped = true;
                break;
            }
            else
            {
                input_char[i] = c;
                i++;
                Serial.print(c);
            }
        }
        delay(10);
    }
    if (!skiped)
    {
        input_char[i] = '\0';
        strcpy(uros_config.description, input_char);
    }
    Serial.println("");
}

void input_ssid()
{
    Serial.print("Input SSID address");
    Serial.println(" input '/' to skip): ");

    char c;
    int i = 0;
    char input_char[DATA_LENGTH];
    bool skiped = false;

    while (c != '\n')
    {
        if (Serial.available())
        {
            c = Serial.read();
            if (c == '\n')
            {
                break;
            }
            else if (c == '/')
            {
                Serial.println("Skip!");
                skiped = true;
                break;
            }
            else
            {
                input_char[i] = c;
                i++;
                Serial.print(c);
            }
        }
        delay(10);
    }
    if (!skiped)
    {
        input_char[i] = '\0';
        strcpy(uros_config.ssid, input_char);
    }
    Serial.println("");
}

void input_pass()
{
    Serial.print("Input Password address");
    Serial.println(" input '/' to skip): ");

    char c;
    int i = 0;
    char input_char[DATA_LENGTH];
    bool skiped = false;

    while (c != '\n')
    {
        if (Serial.available())
        {
            c = Serial.read();
            if (c == '\n')
            {
                break;
            }
            else if (c == '/')
            {
                Serial.println("Skip!");
                skiped = true;
                break;
            }
            else
            {
                input_char[i] = c;
                i++;
                Serial.print(c);
            }
        }
        delay(10);
    }
    if (!skiped)
    {
        input_char[i] = '\0';
        strcpy(uros_config.pass, input_char);
    }
    Serial.println("");
}

void input_agent_ip_address()
{
    Serial.print("Input IP address");
    Serial.println(" input '/' to skip): ");

    char c;
    int i = 0;
    char input_char[DATA_LENGTH];
    bool skiped = false;

    while (c != '\n')
    {
        if (Serial.available())
        {
            c = Serial.read();
            if (c == '\n')
            {
                break;
            }
            else if (c == '/')
            {
                Serial.println("Skip!");
                skiped = true;
                break;
            }
            else
            {
                input_char[i] = c;
                i++;
                Serial.print(c);
            }
        }
        delay(10);
    }
    if (!skiped)
    {
        input_char[i] = '\0';
        strcpy(uros_config.agent_ip_address, input_char);
    }
    Serial.println("");
}

void input_agent_id()
{
    Serial.print("Input agent-ID");
    Serial.println(" input '/' to skip): ");

    char c;
    int i = 0;
    char input_char[DATA_LENGTH];
    bool skiped = false;

    clear_data();
    while (c != '\n')
    {
        if (Serial.available())
        {
            c = Serial.read();
            if (c == '\n')
            {
                break;
            }
            else if (c == '/')
            {
                Serial.println("Skip!");
                skiped = true;
                break;
            }
            else
            {
                input_char[i] = c;
                i++;
                Serial.print(c);
            }
        }
        delay(10);
    }
    if (!skiped)
    {
        input_char[i] = '\0';
        int port = atoi(input_char);
        uros_config.agent_port = port;
    }
    Serial.println("");
}
// --------------------------------------------------

DATA_SET load_data(bool _btn_is_pressed)
{
    EEPROM.get<DATA_SET>(0, uros_config);
    Serial.println("");
    Serial.println("-------- Load data ----------");
    Serial.println("Node name: " + String(uros_config.node_name));
    Serial.println("Node namespace: " + String(uros_config.node_namespace));
    Serial.println("Description: " + String(uros_config.description));
    Serial.println("SSID: " + String(uros_config.ssid));
    Serial.println("PASS: " + String(uros_config.pass));
    Serial.println("IP: " + String(uros_config.agent_ip_address));
    Serial.println("port: " + String(uros_config.agent_port));
    Serial.println("------------------------------");

    if (!_btn_is_pressed)
    {
        Serial.println("Skip");
        return uros_config;
    }

    input_node_name();
    input_node_namespace();
    input_description();

    input_ssid();
    input_pass();
    input_agent_ip_address();
    input_agent_id();

    // enter command "_" on end of input

    Serial.println("");
    Serial.println("-------- Input OK ! ----------");
    Serial.println("Node name: " + String(uros_config.node_name));
    Serial.println("Node namespace: " + String(uros_config.node_namespace));
    Serial.println("Description: " + String(uros_config.description));
    Serial.println("SSID: " + String(uros_config.ssid));
    Serial.println("PASS: " + String(uros_config.pass));
    Serial.println("IP: " + String(uros_config.agent_ip_address));
    Serial.println("port: " + String(uros_config.agent_port));
    Serial.println("------------------------------");

    save_data(uros_config);
    EEPROM.end();
    return uros_config;
}

DATA_SET eeprom_load_agent_port(int _btn_is_pressed)
{
    EEPROM.begin(sizeof(DATA_SET));
    return load_data(_btn_is_pressed);
}
