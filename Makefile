OBJ = main.o
INC = -I "./"
THREAD = -lpthread

raytracer: build run

build: $(OBJ)
	g++ $(OBJ) -o raytracer.out $(THREAD)
	rm -f $(OBJ)

render:
	./raytracer.out

run: render show

show:
	eog scene.bmp

debug:
	g++ -g -o raytracer-debug.out main.cpp $(THREAD)

main.o:
	g++ -c main.cpp $(INC)

clean:
	rm -f $(OBJ) raytracer
