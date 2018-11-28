#Make file for c++ miniprjects

#Meluleki Dube
CPP =g++
CPPFLAGS =-std=c++11
TARGET=tic_tac_toe
SRC=tic_tac_toe.cpp 
OBJECTS=tic_tac_toe.o 


$(TARGET): $(OBJECTS)
	$(CPP) $(OBJECTS)-o $@ $(CPPFLAGS)

$(OBJECTS): $(SRC)
	$(CPP) $< -c $(SRC) $(CPPFLAGS)

run:
	./$(TARGET)

clean:
	rm -f -r *.o bin/*.o *.exe

# end of Makefile