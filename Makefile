CXX := clang++
CXXFLAGS := -std=c++23 -O2 -Wall -Wextra -Wpedantic -Iinclude -I/opt/homebrew/include
LDFLAGS := -L/opt/homebrew/lib
LDLIBS := -lssl -lcrypto -lboost_system -lboost_thread -lboost_json -lpthread

SRCS := src/jpwrap/jsonplaceholder/client.cpp
OBJS := $(patsubst src/%.cpp, build/obj/%.o, $(SRCS))
EXE := build/example_main

all: $(EXE)

$(EXE): $(OBJS) example/example_main.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

build/obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build

.PHONY: all clean
