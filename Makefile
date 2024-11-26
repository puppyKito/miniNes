BUILD_DIR := ./build
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:%.o=%.d)
CXX := g++
CXXFLAGS := -g -Wall

test: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

.PHONY: clean
clean:
	rm *.o

-include $(DEPS)

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@
