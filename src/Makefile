CC = g++
CXXFLAGS = -std=c++11 -I/path/to/include/directory
LDFLAGS = -L/path/to/library/directory

SOURCES = block.cpp transaction.cpp merkle_tree.cpp network.cpp consensus.cpp node.cpp utils.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = blockchain

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

