#pragma once

#include "../base/unique_executor.hpp"
#include "../base/ip_utils.hpp"
#include "../uros_function/common_uros_function.hpp"

#include "../platform/initialize_stm32eth.hpp"
#include "../platform/initialize_esp32wifi.hpp"
#include "../platform/initialize_usb.hpp"


extern rclc_executor_t executor;
extern rclc_support_t support;
extern rcl_allocator_t allocator;
extern rcl_node_t node;
