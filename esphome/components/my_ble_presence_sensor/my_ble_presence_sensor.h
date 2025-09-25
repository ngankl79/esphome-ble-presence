#pragma once
#include "esphome/core/component.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace my_ble_presence_sensor {

class MyBLEPresenceSensor : public Component, public esp32_ble_tracker::ESP32BLETrackerListener {
 public:
  void set_sensor(binary_sensor::BinarySensor *sensor) { presence_sensor_ = sensor; }

  void setup() override {
    esp32_ble_tracker::ESP32BLETracker::get()->add_on_device_advertise_callback(this);
  }

  void on_device_advertise(const esp32_ble_tracker::ESPBTDevice &device) override {
    if (device.get_name().find("Xperia") != std::string::npos ||
        device.get_name().find("Home Assistant") != std::string::npos) {
      int rssi = device.get_rssi();
      ESP_LOGI("ble_presence", "Detected device with RSSI: %d", rssi);
      presence_sensor_->publish_state(rssi > -75);
    }
  }

 protected:
  binary_sensor::BinarySensor *presence_sensor_;
};

}  // namespace my_ble_presence_sensor
}  // namespace esphome
