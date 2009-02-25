PROJ = libImageLib.a
OBJS = Camera.o

CC = g++
EXEFLAGS = -O3
OBJFLAGS = -O3 -I"../ImageLib/" -I"../MatrixLib/" -I"../PixmapLib/" -I"../BitmapLib/" -I"../CmdIOLib/"

$(PROJ) : $(OBJS)
	ar rc $(PROJ) $(OBJS)

Camera.o : Camera.h 
	$(CC) $(OBJFLAGS) -c Camera.cpp

.PHONY : clean
clean :
	-rm $(PROJ) $(OBJS)
