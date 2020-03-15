#final exec file.
EXEC = exec

#file to compile
TARGET = 2dspp.cpp

#parameter
INSTANCE = 

#compiler options
CFLAG = -O3 -Wall
CXX = g++

#tools
RM = rm -rf

#compile the program.
all: $(TARGET)
	$(CXX) $(TARGET) -o $(EXEC) $(CFLAG)

run:
	./$(EXEC) $(INSTANCE)

clean:
	$(RM) $(EXEC)