PROJ = libPixmapLib.a
OBJS = pfm.o pgm.o ppm.o

CC = g++
EXEFLAGS = -O3
OBJFLAGS = -O3

$(PROJ) : $(OBJS)
	ar rc $(PROJ) $(OBJS)

pfm.o : pfm.cpp
	$(CC) $(OBJFLAGS) -c pfm.cpp

pgm.o : pgm.cpp
	$(CC) $(OBJFLAGS) -c pgm.cpp

ppm.o : ppm.cpp
	$(CC) $(OBJFLAGS) -c ppm.cpp

.PHONY : clean
clean :
	-rm $(PROJ) $(OBJS)
