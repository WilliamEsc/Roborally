CXX      = g++ -Wall

.DEFAULT_GOAL := all

SOURCES = 
SOURCES += board.cpp graph.cpp 

OBJECTS = $(SOURCES:.cpp=.o)

CXXFLAGS += -g -Wall -pedantic
LDFLAGS +=

$(OBJECTS) : %.o : %.cpp
	$(CXX) -MMD $(CXXFLAGS) -c $< -o $@

CLEAN_OBJECTS = $(OBJECTS)
TARGETS =

########## test_board ##########

TEST_BOARD_SOURCES = test_board.cpp 
TEST_BOARD_OBJECTS = $(TEST_BOARD_SOURCES:.cpp=.o)

test_board : $(TEST_BOARD_OBJECTS) $(OBJECTS) $(HEADERS)
	$(CXX) $(TEST_BOARD_OBJECTS) $(OBJECTS) -o $@ $(LDFLAGS)
	
$(TEST_BOARD_OBJECTS): %.o : %.cpp
	$(CXX) -MMD $(CXXFLAGS) -c $< -o $@

all : test_board
TARGETS += test_board
CLEAN_OBJECTS += $(TEST_BOARD_OBJECTS)

########## test_board ##########

DUEL_SOURCES = duel.cpp 
DUEL_OBJECTS = $(DUEL_SOURCES:.cpp=.o)

duel : $(DUEL_OBJECTS) $(OBJECTS) $(HEADERS)
	$(CXX) $(DUEL_OBJECTS) $(OBJECTS) -o $@ $(LDFLAGS)
	
$(DUEL_OBJECTS): %.o : %.cpp
	$(CXX) -MMD $(CXXFLAGS) -c $< -o $@

all : duel
TARGETS += duel
CLEAN_OBJECTS += $(DUEL_OBJECTS)

########## app ##########

APP_SOURCES = app.cpp
APP_OBJECTS = $(APP_SOURCES:.cpp=.o)

app : $(APP_OBJECTS) $(OBJECTS) $(HEADERS)
	$(CXX) -pthread $(APP_OBJECTS) $(OBJECTS) -o $@ $(LDFLAGS)
	
$(APP_OBJECTS): %.o : %.cpp
	$(CXX) -MMD $(CXXFLAGS) -c $< -o $@

all : app
TARGETS += app
CLEAN_OBJECTS += $(APP_OBJECTS)

########## cleanup ##########
DEPS = $(CLEAN_OBJECTS:.o=.d)

clean:
	@rm -f $(DEPS) $(TARGETS) $(CLEAN_OBJECTS)

-include $(DEPS)
