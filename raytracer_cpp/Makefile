CFLAGS = -fopenmp -Wno-deprecated -std=c++11 -O3 -ffast-math -lstdc++
MACFLAGS = -framework GLUT -framework OpenGL -framework Cocoa

mac_gnu:
	gcc-4.7 $(CFLAGS) $(MACFLAGS) *.cpp -o raytracer
clean:
	rm raytracer
