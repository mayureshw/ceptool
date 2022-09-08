%.out:	%.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS)  $< -o $@

BINS	= example.out

all:	$(BINS)

clean:
	rm -f $(BINS)

include $(CEPTOOLDIR)/Makefile.ceptool
