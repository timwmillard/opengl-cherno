

CC=clang++
CFLAGS=-std=c++17 -stdlib=libc++
INCLUDES=-Idependancies/include
LIBARYS=-Ldependancies/lib dependancies/lib/libglfw3.a dependancies/lib/libGLEW.a
FRAMEWORKS=-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
MACROS=-DGLEW_STATIC
DEBUG=-g

main: src/app.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBARYS) $(FRAMEWORKS) src/app.cpp -o build/opengl-app

debug: src/app.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBARYS) $(FRAMEWORKS) $(DEBUG) src/app.cpp -o debug/opengl-app

prod: src/app.cpp
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBARYS) $(FRAMEWORKS) src/app.cpp -o build/Canvas.app/Contents/MacOS/canvas

clean:
	rm -f build/opengl-app
	rm -rf debug/*
