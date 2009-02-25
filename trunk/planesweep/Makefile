PROJ = planesweep
OBJS = mvgcut.o

CC = g++
EXEFLAGS = -O3 -L"../ImageLib/" -lImageLib -L"../BitmapLib/" -lBitmapLib -L"../PixmapLib/" -lPixmapLib -L"../CmdIOLib/" -lCmdIOLib
OBJFLAGS = -O3 -I"../MatrixLib/" -I"../ImageLib/" -I"../CmdIOLib/" -I"../RefCntMem"

$(PROJ) : $(OBJS)
	$(CC) -o $(PROJ) $(OBJS) $(EXEFLAGS)
#	condor_compile $(CC) -o $(PROJ) $(OBJS) $(EXEFLAGS)

mvgcut.o : 
	$(CC) $(OBJFLAGS) -c mvgcut.cpp

.PHONY : clean
clean :
	-rm $(PROJ) $(OBJS)
