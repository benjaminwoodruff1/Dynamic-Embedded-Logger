#include <gtest/gtest.h>
#include "template.h"

// Test default log level is NONE
TEST(LoggerTest, DefaultLogLevel) {
    set_system_log_level(LOG_NAV, LOG_LEVEL_NONE);
    testing::internal::CaptureStdout();
    log_info_inline(LOG_NAV, LOG_LEVEL_INFO, "This should not be printed");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "");
}

TEST(LoggerTest, EnableLogLevel) {
    set_system_log_level(LOG_NAV, LOG_LEVEL_INFO);
    testing::internal::CaptureStdout();
    log_info_inline(LOG_NAV, LOG_LEVEL_INFO, "Satellites active: %d", 3);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "[NAV] [INFO]: Satellites active: 3\n");
}

TEST(LoggerTest, Filter) {
    set_system_log_level(LOG_NAV, LOG_LEVEL_WARNING);
    testing::internal::CaptureStdout();
    log_info_inline(LOG_NAV, LOG_LEVEL_INFO, "This should not be printed");
    log_info_inline(LOG_NAV, LOG_LEVEL_WARNING, "Battery low: %.1f%%", 15.5);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "System: NAV    WARNING: Battery low: 15.5%\n");
}