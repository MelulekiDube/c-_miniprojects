#Make file for c++ miniprjects

#Meluleki Dube
CPP =g++
CC =gcc
LL =-lpdcurses
CPPFLAGS =-std=c++11
CCFLAGS =-std=c99
TARGET=tic_tac_toe
SRC=tic_tac_toe.cpp 
OBJECTS=tic_tac_toe.o 


$(TARGET): $(OBJECTS)
	$(CPP) $(OBJECTS)-o $@ $(CPPFLAGS)

$(OBJECTS): $(SRC)
	$(CPP) $< -c $(SRC) $(CPPFLAGS)

#snake games
snake: snake.c
	$(CC) $< -o $@ $(LL) $(CCFLAGS) 
	
run:
	./$(TARGET)

runs:
	./snake
clean:
	rm -f -r *.o bin/*.o *.exe

# end of Makefile
