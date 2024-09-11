#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/empty.hpp"
#include "std_msgs/msg/u_int8.hpp"
#include "std_msgs/msg/u_int16.hpp"
#include "std_msgs/msg/u_int32.hpp"
#include "std_msgs/msg/u_int64.hpp"
#include "std_msgs/msg/int8.hpp"
#include "std_msgs/msg/int16.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/int64.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/float64.hpp"
#include <cstdint>
#include <fstream>
#include <iostream>

#include "monitor.h"
#include "monitor.c"

using std::placeholders::_1;

std::int64_t input_signal;
float input_signal_float;
double input_signal_double;

class CopilotRV : public rclcpp::Node {
  public:
    CopilotRV() : Node("copilotrv") {
      //input_signal_subscription_ = this->create_subscription<std_msgs::msg::Int64>(
       // "/demo/topic", 10,
       // std::bind(&CopilotRV::input_signal_callback, this, _1));

      input_signal_float_subscription_ = this->create_subscription<std_msgs::msg::Float32>(
        "/demo/topicf", 10,
        std::bind(&CopilotRV::input_signal_float_callback, this, _1));

      //input_signal_double_subscription_ = this->create_subscription<std_msgs::msg::Float64>(
      //  "/demo/topicd", 10,
      //  std::bind(&CopilotRV::input_signal_double_callback, this, _1));

      handlerBatteryCopilot_publisher_ = this->create_publisher<std_msgs::msg::Bool>(
        "copilot/handlerCopilot", 10);

    }

    // Report (publish) monitor violations.
    void handlerBatteryCopilot(bool value) {
      auto output = std_msgs::msg::Bool();
      output.data = value;

      handlerBatteryCopilot_publisher_->publish(output);
    }

    // Needed so we can report messages to the log.
    static CopilotRV& getInstance() {
      static CopilotRV instance;
      return instance;
    }

  private:
    void input_signal_callback(const std_msgs::msg::Int64::SharedPtr msg) const {
      input_signal = msg->data;
      step();
    }

    void input_signal_float_callback(const std_msgs::msg::Float32::SharedPtr msg) const {
      input_signal_float = msg->data;
      step();
    }

    void input_signal_double_callback(const std_msgs::msg::Float64::SharedPtr msg) const {
      input_signal_double = msg->data;
      step();
    }

    //rclcpp::Subscription<std_msgs::msg::Int64>::SharedPtr input_signal_subscription_;

    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr input_signal_float_subscription_;

    //rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr input_signal_double_subscription_;

    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr handlerBatteryCopilot_publisher_;

};

// Pass monitor violations to the actual class, which has ways to
// communicate with other applications.
void handlerBatteryCopilot(bool value) {
  CopilotRV::getInstance().handlerBatteryCopilot(value);
  std::cout << "Handler triggered. Battery. Low?: " << value << std::endl;
  //std::ofstream("example.txt") << "Handler triggered.\n";
 }

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CopilotRV>());
  rclcpp::shutdown();
  return 0;
}
