# If you are using C++, change shell.c to shell.cpp or shell.cc
SERVERSOURCES=parser.c csapp.c rsh-server.c
CLIENTSOURCES=parser.c csapp.c rsh-client.c
CLIENTOBJECTS=parser.o csapp.o rsh-client.o
SERVEROBJECTS=parser.o csapp.o rsh-server.o

CLIENTBINARIES=rrsh-client
SERVERBINARIES=rrsh-server

CFLAGS = -std=c99 -D_GNU_SOURCE -Wall -pthread
CXXFLAGS = -std=c++11 -D_GNU_SOURCE -Wall

all: $(CLIENTBINARIES) $(SERVERBINARIES)

clean:
	-rm -f $(CLIENTBINARIES) $(CLIENTOBJECTS) core
	-rm -f $(SERVERBINARIES) $(SERVEROBJECTS) core

# If you are using C++, change the command to  $(CXX) $(CXXFLAGS) -o $@ $^
rrsh-client: $(CLIENTOBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

rrsh-server: $(SERVEROBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Rules for compiling.
%.o: %.c csapp.h rshell.h 
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cpp csapp.h rshell.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o: %.cc csapp.h rshell.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

zip:
	zip -l Program5.zip  ./*.c ./*.h ./Makefile ./README.txt
