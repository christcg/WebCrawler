CXX = g++
CPPFLAGS = -g -Wall -Wextra -Wconversion -std=c++11

all: search

config.o: config.cpp config.h
	$(CXX) $(CPPFLAGS) -c config.cpp -o config.o

webpage.o: webpage.cpp webpage.h
	$(CXX) $(CPPFLAGS) -c webpage.cpp -o webpage.o

crawler: webcrawler.cpp webpage.o config.o
	$(CXX) $(CPPFLAGS) webcrawler.cpp webpage.o config.o -o crawler

search: websearch.cpp webpage.o config.o
	$(CXX) $(CPPFLAGS) websearch.cpp webpage.o config.o -o search