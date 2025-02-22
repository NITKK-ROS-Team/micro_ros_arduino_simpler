#pragma once

// microros ===================================================================
#include <micro_ros_arduino.h>

#include <rcl/error_handling.h>
#include <rcl/rcl.h>
#include <stdio.h>

#include <rclc/executor.h>
#include <rclc/rclc.h>

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;

#define LED_PIN 13

// ============================================================================
