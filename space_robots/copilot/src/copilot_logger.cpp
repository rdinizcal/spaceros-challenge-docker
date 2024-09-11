#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "std_msgs/msg/empty.hpp"
#include "std_msgs/msg/bool.hpp"
using std::placeholders::_1;

class CopilotLogger : public rclcpp::Node {
  public:
    CopilotLogger() : Node("copilotlogger") {
      handlerBatteryCopilot_subscription_ = this->create_subscription<std_msgs::msg::Bool>(
        "copilot/handlerCopilot", 10,
        std::bind(&CopilotLogger::handlerBatteryCopilot_callback, this, _1));

    }

  private:
    void handlerBatteryCopilot_callback(const std_msgs::msg::Bool::SharedPtr msg) const {
      RCLCPP_INFO(this->get_logger(), "Copilot monitor violation: handlerBatteryCopilot");

    }


    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr handlerBatteryCopilot_subscription_;

};

int main(int argc, char* argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CopilotLogger>());
  rclcpp::shutdown();
  return 0;
}
