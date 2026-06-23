CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -I./include
SRC_DIR  := src
TEST_DIR := tests
BUILD    := build

SRCS     := $(SRC_DIR)/main.cpp $(SRC_DIR)/PaymentService.cpp
TEST_SRC := $(TEST_DIR)/test_payment.cpp $(SRC_DIR)/PaymentService.cpp

TARGET      := $(BUILD)/payment_gateway
TEST_TARGET := $(BUILD)/test_runner

.PHONY: all clean test run

all: $(TARGET)

$(BUILD):
	mkdir -p $(BUILD)

$(TARGET): $(SRCS) | $(BUILD)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_TARGET): $(TEST_SRC) | $(BUILD)
	$(CXX) $(CXXFLAGS) $^ -o $@

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -rf $(BUILD)
