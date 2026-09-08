#include <gtest/gtest.h>
#include "template.h"

// Test default log level is NONE
TEST(LoggerTest, DefaultLogLevel) {
    set_system_log_level(LOG_NAV, LOG_LEVEL_NONE);
    testing::internal::CaptureStdout();
    logger(LOG_NAV, LOG_LEVEL_INFO, "This should not be printed");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "");
}

TEST(LoggerTest, EnableLogLevel) {
    set_system_log_level(LOG_NAV, LOG_LEVEL_INFO);
    testing::internal::CaptureStdout();
    logger(LOG_NAV, LOG_LEVEL_INFO, "Satellites active");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "System: NAV    INFO: Satellites active\n");
}

TEST(LoggerTest, Filter) {
    set_system_log_level(LOG_NAV, LOG_LEVEL_WARNING);
    testing::internal::CaptureStdout();
    logger(LOG_NAV, LOG_LEVEL_INFO, "This should not be printed");
    logger(LOG_NAV, LOG_LEVEL_WARNING, "Battery low");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "System: NAV    WARNING: Battery low\n");
}