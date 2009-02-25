PROJ = libBitmapLib.a
OBJS = bitmap.o 

CC = g++
EXEFLAGS = -O3
OBJFLAGS = -O3 

$(PROJ) : $(OBJS)
	ar rc $(PROJ) $(OBJS)

bitmap.o : bitmap.h 
	$(CC) $(OBJFLAGS) -c bitmap.cpp

.PHONY : clean
clean :
	-rm $(PROJ) $(OBJS)
