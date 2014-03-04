CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

DisplayImage: DisplayImage.cpp
	g++ $(CFLAGS) $(LIBS) -o DisplayImage DisplayImage.cpp
