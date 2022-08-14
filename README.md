# micro_ros_arduino_simpler [WIP]

Simpler library for [microROS-Arduino](https://github.com/micro-ROS/micro_ros_arduino) on [PlatformIO](https://platformio.org).

<br>

## 💬 Discussion

- [Discussions tab](https://github.com/NITKK-ROS-Team/micro_ros_arduino_simpler/discussions)

<br>

## Support devices

| Board | Ethernet | Wi-Fi | USB | configure |
| --- | --- | --- | --- | --- |
| M5Core2 | | ✅ | ✅ | ✅ (EEPROM) |
| ESP32-Pico | | ✅ | ✅ | ✅ (EEPROM) |
| STM32F7Discovery | ✅ | | ❓ | |
| STM32L4 + WS5100S | ✅ | | ❓ | |

<br>

## ⚡ ESP32 (M5Core2, M5Atom, M5Stick)

Device used : M5Atom

<details>
<summary>Platformio.ini</summary>

```ini
;; ==========================================================================
;;                      ESP32, ESP32-Pico Wi-Fi Model
;; ==========================================================================
[env:microros-template]
platform = https://github.com/platformio/platform-espressif32.git#feature/arduino-upstream
; board = m5stack-core2
board = m5stick-c ;; m5stick-c or m5atom
framework = arduino

monitor_speed = 115200
upload_speed = 115200
upload_device = /dev/ttyUSB0

lib_deps =
;; m5core2 ---------------------------------------------------------------
    ; m5stack/M5Core2@^0.1.0
;; m5stick-c -------------------------------------------------------------
    ; m5stack/M5StickCPlus@^0.0.5
;; m5atom ----------------------------------------------------------------
    m5stack/M5Atom@^0.0.9
    fastled/FastLED@^3.5.0

;; microros ---------------------------------------------------------------
    https://github.com/micro-ROS/micro_ros_arduino.git#v2.0.5-humble
    https://github.com/NITKK-ROS-Team/micro_ros_arduino_simpler.git#main
    stm32duino/STM32Ethernet@^1.3.0

build_flags =
    -L ./.pio/libdeps/esp32dev/micro_ros_arduino/src/esp32/
    -l microros
    -D ESP32

platform_packages =
  toolchain-xtensa32 @ ~2.80400.0
  framework-arduinoespressif32@https://github.com/espressif/arduino-esp32.git#2.0.2
```

</details>

<br>

### Example source code

This is a sample program that counts the number of times a true (bool type) is received.

<details>
<summary>main.cpp</summary>

```c++
#include <Arduino.h>
#include <M5Atom.h>

// microros definition =====================================================
// ### header
#include "micro_ros_arduino_simpler/simpler_base.h"
#include <std_msgs/msg/bool.h>
#include <std_msgs/msg/int32.h>

extern rclc_executor_t executor;
extern rclc_support_t support;
extern rcl_allocator_t allocator;
extern rcl_node_t node;

// ### message definition
rcl_publisher_t publisher;
std_msgs__msg__Int32 msg_int32;

rcl_subscription_t subscription;
std_msgs__msg__Bool msg_bool;
rcl_timer_t timer;
// ==========================================================================

unsigned int counter = 0;

// subscription callback ("/bool_data", "std_msgs/msg/bool") ================
void bool_callback(const void *msgin)
{
  const std_msgs__msg__Bool *_msg = (const std_msgs__msg__Bool *)msgin;
  counter += _msg->data;
}

// timer callback (10ms) ====================================================
void timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
  (void)last_call_time; (void)timer;

  msg_int32.data = counter;
  rcl_publish(&publisher, &msg_int32, NULL);
}

// setup micro_ros_arduino ===============================================
// ### setup
void setup()
{
  setup_microros_usb("microros_node", "", 2);
  // setup_microros_wifi("microros_node", "", 2, "ssid", "pass", "192.168.0.10", 2000);

  rclc_publisher_init_default(&publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), "int32_data");

  rclc_create_timer_and_add(&timer, 10, timer_callback);
  rclc_create_subscription_and_add(&subscription, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool), &msg_bool, &bool_callback, "bool_data");
}

void loop()
{
  rclc_delay(10);
}

```
</details>

<br>

## 🖊️ microROS-arduino cording

```c
#include <Arduino.h>

#if defined(ESP32)
#include <M5Atom.h>
// #include <M5Core2.h>
// #include <M5StickC.h>
#endif

// microros definition =====================================================
#include "micro_ros_arduino_simpler/simpler_base.h"
// #include "micro_ros_arduino_simpler/config_eeprom.h" // using EEPROM (ESP32)

#include <std_msgs/msg/bool.h>
#include <std_msgs/msg/int32.h>

extern rclc_executor_t executor;
extern rclc_support_t support;
extern rcl_allocator_t allocator;
extern rcl_node_t node;

rcl_publisher_t publisher;
std_msgs__msg__Int32 msg_int32;

rcl_subscription_t subscription;
std_msgs__msg__Bool msg_bool;
rcl_timer_t timer;
// ==========================================================================

int counter = 0;

// subscription callback ("/bool_data", "std_msgs/msg/bool") ================
void bool_callback(const void *msgin)
{
  const std_msgs__msg__Bool *_msg = (const std_msgs__msg__Bool *)msgin;
  counter += _msg->data;
}

// timer callback (10ms) ====================================================
void timer_callback(rcl_timer_t *timer, int64_t last_call_time)
{
  (void)last_call_time;
  (void)timer;

  msg_int32.data = counter;
  rcl_publish(&publisher, &msg_int32, NULL);
}

// setup micro_ros_arduino ===============================================
void setup()
{
// ESP32 ---------------------------------------------------------------
  uros_ns config = eeprom_load_agent_port(M5.Btn.isPressed());
  setup_microros_wifi(config, 2);

  // using without eeprom -------
  // setup_microros_wifi("microros_node", "ns", 2, "ssid", "pass", "192.168.0.10", 2000);
  // UART mode ------
  // setup_microros_usb("microros_node", "ns", 2);


  // rclc-publisher-subscriber-timer ======================================
  rclc_publisher_init_default(&publisher, &node, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32), "int32_data");

  rclc_create_timer_and_add(&timer, 10, timer_callback);
  rclc_create_subscription_and_add(&subscription, ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool), &msg_bool, &bool_callback, "bool_data");
}

// loop micro_ros_arduino ===============================================
void loop()
{
  rclc_delay(10);
}
```


### header
- include `"micro_ros_arduino_simpler/simpler_base.h"` to use microROS-Arduino-Template API.
- include ros-messages to publish or subscribe messages.

### message definition
- `rcl_publisher_t` : Equivalent to `rclcpp::Publisher<...>`.
- `std_msgs__msg__Int32` : Equivalent to `std_msgs::msg::Int32`.
- `rcl_subscription_t` : Equivalent to `rclcpp::Subscription<...>`.
- `rcl_timer_t timer` : Equivalent to `node->create_wall_timer`.

### void setup()
- Initialize of micro-ROS
  - (Wi-Fi): `setup_microros_wifi("microros_node", "", 2, "ssid", "pass", "192.168.0.10", 2000);`
    - node name : node name [e.g. `"microros_node"`]
    - namespace : namespace [e.g. `""`]
    - number_of_handle : Total count of subscriber and timer. [e.g. `2`]
    - ssid : SSID for Wi-Fi. [e.g. `"ssid"`]
    - pass : Password for Wi-Fi. [e.g. `"pass"`]
    - agent ip : IP address on host computer running microROS-Agent. [e.g. `"192.168.0.10"`]
    - port : Port number on host computer running microROS-Agent. [e.g. `2000`]
  - (USB) : `setup_microros_usb("microros_node", "", 2);`
    - node name : node name [e.g. `"microros_node"`]
    - namespace : namespace [e.g. `""`]
    - number_of_handle : Total count of subscriber and timer. [e.g. `2`]

<br>

- Definition of publisher (`rclc_publisher_init_default`)
  - `rclc_publisher_init_default` : Equivalent to `node->create_publisher`.
    - `ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32)` : Type of publish message.
    - `int32_data` : topic name
- Definition of timer (`rclc_create_timer_and_add`)
  - `rclc_create_timer_and_add` : Equivalent to `node->create_timer`.
    - `10` : timeout (ms)
    - `timer_callback` : Define a callback function for timeouts.
- Definition of subscriber (`rclc_create_subscription_and_add`)
  - `rclc_publisher_init_default` : Equivalent to `node->create_subscription`.
    - `ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool)` : Type of subscribe message.
    - `timer_callback` : Message chache for subscribe.
    - `"bool_data"` : Subscribe message name. (You have to publish `"bool_data"` on any ROS-node to call `timer_callback` on microROS)

### void loop()
- Arduino Loop. It can be written to work, but the basic idea is to use a timer callback.
  - rclc_delay : Equivalent to `rclcpp::spin_some()`.
Template of microROS-Arduino on PlatformIO

<br>

### 🔧 Using [microROS-agent-pairing](https://github.com/NITKK-ROS-Team/microROS-agent-pairing) (ESP32-EEPROM)

"microROS-agent-pairing" is a means of overriding the microROS-agent's settings without changing the program, even if the microROS-agent's connection point changes.

### requirements

- Wi-Fi (ESP32)
- Any button or trigger

### code

Replace `setup_microros_wifi` with the following

```c++
#include "micro_ros_arduino_simpler/config_eeprom.h" // add

// ~~~

  // setup_microros_wifi("microros_node", "", 2, "ssid", "pass", "192.168.0.10", 2000);

  // Toggle between write/read configuration　(true: write config mode, false: load config mode)
  uros_ns config = eeprom_load_agent_port(bool_trigger); // e.g. M5.Btn.isPressed()
  setup_microros_wifi(config, 2);
```

<br>

## ⚡ STM32F7Discovery

Replace `setup_microros_wifi` with the following

```c++
  byte arduino_mac[] = {0xAA, 0xBB, 0xCC, 0xEE, 0xDD, 0xFF};
  IPAddress arduino_ip(192, 168, 10, 111);
  IPAddress agent_ip(192, 168, 10, 6);
  int agent_port = 2000;

  setup_microros_ethernet("uros_node", "ns", 2, arduino_mac, arduino_ip, agent_ip, agent_port);
```

STM32 Series using ststm32 platform. Change platformio.ini

<details>
<summary>Platformio.ini</summary>

```ini
;; ==========================================================================
;;                       STM32F4, F7 Ethernet Model
;; ==========================================================================

[env:disco_f746ng]
platform = ststm32
board = disco_f746ng
framework = arduino

monitor_speed = 115200
upload_speed = 115200
upload_device = /dev/ttyUSB0

lib_deps =

;; microros ---------------------------------------------------------------
    https://github.com/NITKK-ROS-Team/micro_ros_arduino.git#humble
    https://github.com/NITKK-ROS-Team/micro_ros_arduino_simpler.git#main

    stm32duino/STM32Ethernet@^1.3.0
    stm32duino/STM32duino LwIP@^2.1.2

build_flags =
    -L ./.pio/libdeps/disco_f746ng/micro_ros_arduino/src/cortex-m4/
    -l microros

```
</details>

<br>

## Easy test on commandline

You can test if the device's pub-sub and timer are working properly following commands.

### Terminal 1

```bash
docker run -it --rm --privileged --net=host microros/micro-ros-agent:humble udp4 --port 2000
```

### Terminal2

```bash
ros2 topic pub /ns/bool_data std_msgs/msg/Bool data:\ true
```

### Terminal3

```bash
ros2 topic echo /ns/int32_data
# The number will increase with each second.
```

<br>

## STM32L432KC + WS5100

```ini
[env:nucleo_l432kc]
platform = ststm32
board = nucleo_l432kc
framework = arduino
; change microcontroller
board_build.mcu = stm32l432kcu6

; change MCU frequency
board_build.f_cpu = 80000000L

monitor_speed = 115200
upload_speed = 115200
upload_device = /dev/ttyACM0

lib_deps =
;; microros ---------------------------------------------------------------
    https://github.com/NITKK-ROS-Team/micro_ros_arduino.git#features/add_spi_eth

    https://github.com/WIZnet-ArduinoEthernet/Ethernet.git

build_flags =
    -L ./.pio/libdeps/pico/micro_ros_arduino/src/cortex-m4/
    -l microros
```
