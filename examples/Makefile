# Define a suitable compiler that supports c++14
CPP=/usr/pkg/gcc12/bin/c++

%.out:	%.cpp
	$(CPP) $(CPPFLAGS) $(LDFLAGS)  $< -o $@

all:	example.out

include $(CEPTOOLDIR)/Makefile.ceptool
