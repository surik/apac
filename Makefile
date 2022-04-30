CC = g++
CCFLAGS = -O2 -Wall `libpng-config --cflags`
STRIP = strip
LIBS = `libpng-config --ldflags` -ltinyxml
OBJ = image.o atlas.o app.o main.o
TARGET = apac

.PJONE: all clean docker

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LIBS)
	$(STRIP) $(TARGET)

%.o : %.cc
	$(CC) $(CCFLAGS) -c -o $@ $<

docker:
	docker build -t apac .

clean:
	rm $(OBJ)
