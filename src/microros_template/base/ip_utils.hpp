#pragma once

#include <stdio.h>
#include <memory>
#include <assert.h>
#include <string.h>

// detect `IPAddress` type
#if defined(TARGET_STM32F4) || defined(ARDUINO_ARCH_STM32)
#include <STM32Ethernet.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif


size_t split(char* s, const char* separator, char** result, size_t result_size)
{
    assert(s != NULL);
    assert(separator != NULL);
    assert(result != NULL);
    assert(result_size > 0);

    size_t i = 0;

    char* p = strtok(s, separator);
    while (p != NULL) {
        assert(i < result_size);
        result[i] = p;
        ++i;

        p = strtok(NULL, separator);
    }

    return i;
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

char *ipToString_under_16bit(IPAddress ip)
{
    char *result = (char *)malloc(7);

    sprintf(result, "%d_%d",
            ip[2], ip[3]);

    return result;
}

char *hostip_under_8bit(char *hostip)
{
    // e.g. 192,168,100.200
    char *result = (char *)malloc(4);

    char *ip_array[4];
    size_t ip_array_size = split(hostip, ".", ip_array, 4);

    return ip_array[3];
}

char *hostip_under_8bit(IPAddress hostip)
{
    // e.g. 192,168,100.200
    char *result = (char *)malloc(4);

    sprintf(result, "%d", hostip[3]);

    return result;
}
