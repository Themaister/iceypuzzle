TARGET := puzzle

OBJ := main.o entity.o game.o level.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -std=gnu++0x -Wall -Wextra -g -O0 -o $(TARGET) $(OBJ) -lsfml-graphics -lsfml-system -lsfml-window

clean:
	rm -f $(TARGET)
	rm -f $(OBJ)

%.o: %.cpp
	$(CXX) -g -c -o $@ $< -std=gnu++0x -g -O0 -Wall -Wextra
