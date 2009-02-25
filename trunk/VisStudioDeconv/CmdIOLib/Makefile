PROJ = libCmdIOLib.a
OBJS = CmdArgParser.o

CC = g++
EXEFLAGS = -O3
OBJFLAGS = -O3 -I"../MatrixLib/"

$(PROJ) : $(OBJS)
	ar rc $(PROJ) $(OBJS)

CmdArgParser.o : CmdArgParser.h 
	$(CC) $(OBJFLAGS) -c CmdArgParser.cpp

.PHONY : clean
clean :
	-rm $(PROJ) $(OBJS)
