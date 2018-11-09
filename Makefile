CXX     = g++
CXXFLAGS = -std=c++17 -I include
LDLIBS  = -lstdc++fs `pkg-config opencv --cflags --libs`
WFLAGS = -Wall -Wextra -O3

sobel: src/sobel.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(WFLAGS) $(LDLIBS) 

distortion: src/distortion.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(WFLAGS) $(LDLIBS) 

pca: src/pca.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(WFLAGS) $(LDLIBS) 


.PHONY: clean

clean:
	rm -rf pca distortion
