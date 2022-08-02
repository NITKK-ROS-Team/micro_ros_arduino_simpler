#pragma once

#include "../base/unique_executor.hpp"

extern rclc_executor_t executor;
extern rclc_support_t support;
extern rcl_allocator_t allocator;
extern rcl_node_t node;

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

// rclc_delay for "void loop()" ===================================================================
void rclc_delay(int _ms)
{
    delay(_ms);
    rclc_executor_spin_some(&executor, RCL_MS_TO_NS(_ms));
}

// Add executor ===================================================================
void rclc_create_timer_and_add(rcl_timer_t *timer, unsigned int _timeout_ms, rcl_timer_callback_t _callback)
{
    rclc_timer_init_default(timer, &support, RCL_MS_TO_NS(_timeout_ms), _callback);
    rclc_executor_add_timer(&executor, timer);
}

void rclc_create_subscription_and_add(rcl_subscription_t *_subscription, const rosidl_message_type_support_t *_type_support, void *msg, rclc_subscription_callback_t _callback, char *_topic_name, const rclc_executor_handle_invocation_t _invocation = ON_NEW_DATA)
{
    rclc_subscription_init_default(_subscription, &node, _type_support, _topic_name);
    rclc_executor_add_subscription(&executor, _subscription, &msg, _callback, _invocation);
}