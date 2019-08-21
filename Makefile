OBJ = main.o
INC = -I "./"
LIB = -lSDL2

raytracer: $(OBJ)
	g++ $(OBJ) $(LIB) -o raytracer.out
	rm -f $(OBJ)
	./raytracer.out

debug:
	g++ -g -o raytracer-debug.out main.cpp $(LIB)

main.o:
	g++ -c main.cpp $(INC)

clean:
	rm -f $(OBJ) raytracer
