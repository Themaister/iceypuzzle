TARGET := puzzle

OBJ := main.o entity.o game.o level.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -s -o $(TARGET) $(OBJ) -lsfml-graphics -lsfml-system -lsfml-window

clean:
	rm -f $(TARGET)
	rm -f $(OBJ)

%.o: %.cpp
	$(CXX) -c -o $@ $< -std=gnu++0x -O2 -Wall -Wextra
