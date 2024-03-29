SOURCES=$(shell find . -name "*.cpp")
OBJECTS=$(SOURCES:%.cpp=%.o)
TARGET=sandbox

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS)