COMPILER = g++
STD = -std=c++17
WARNING_OPTIONS = -Wextra -Wall -Werror
SOURCE = src/*.cpp
INCLUDE = -I/usr/include/glm -I/usr/include/freetype2
LIBS_PATHS = -L/usr/lib/x86_64-linux-gnu
LIBS = -lGLEW -lglfw -lGL -lm -lfreetype
TARGET = SpaceSimulator
MAIN_CMD = $(STD) -o $(TARGET) $(SOURCE) $(INCLUDE) $(LIBS_PATHS) $(LIBS)

TEST_TARGET = SpaceSimulatorTests
TEST_SOURCE = $(filter-out src/main.cpp, $(wildcard src/*.cpp)) $(wildcard tests/*.cpp)
TEST_CMD = $(STD) -o $(TEST_TARGET) $(TEST_SOURCE) -Isrc -Itests $(INCLUDE) $(LIBS_PATHS) $(LIBS)


default:
	$(COMPILER) $(MAIN_CMD)

warnings:
	$(COMPILER) $(MAIN_CMD) $(WARNING_OPTIONS)

debug:
	$(COMPILER) -g $(MAIN_CMD) $(WARNING_OPTIONS)

test:
	$(COMPILER) $(TEST_CMD) $(WARNING_OPTIONS)
	./$(TEST_TARGET)

.PHONY: default warnings debug test
