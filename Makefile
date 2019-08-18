OBJ = main.o
INC = -I "./"

raytracer: $(OBJ)
	g++ $(OBJ) -o raytracer.out
	rm -f $(OBJ)
	./raytracer.out

main.o:
	g++ -c main.cpp $(INC)

clean:
	rm -f $(OBJ) raytracer
