#pragma once

#include "../base/unique_executor.hpp"
#include "../base/ip_utils.hpp"
#include "../uros_function/common_uros_function.hpp"

int setup_microros_usb(char *_node_name, char *_namespace, int _total_callback_count)
{
    set_microros_transports();
    get_default_allocator();

    rclc_init(_node_name, _namespace);
    init_executor(_total_callback_count);
    return 0;
}
