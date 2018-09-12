CXXFLAGS = -std=c++11 -O2

OBJ = DataHandler.o main.o

aetg: $(OBJ)
	g++ -o aetg $(OBJ)

DataHandler.o: DataHandler.h
main.o:

.PHONY : clean
clean:
	rm -r *.o aetg
