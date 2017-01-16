PHONY= doc clean

CC=g++
OPTIONS= -g -w
ALE= -lale
NCURSES= -lncurses
DEBUG= #-D DEBUG
LIBDIR=lib
OBJDIR=obj
INCLUDEDIR=include
AGENTS=nn_agent log_agent data_agent


#Objects
OBJ = $(sort $(NN_OBJ) $(U_OBJ) $(LR_OBJ))

_NN_OBJ= data.o nn.o
NN_OBJ = $(patsubst %,$(OBJDIR)/%,$(_NN_OBJ))

_U_OBJ= utils.o
U_OBJ = $(patsubst %,$(OBJDIR)/%,$(_U_OBJ))

_LR_OBJ= LogisticRegression.o
LR_OBJ = $(patsubst %,$(OBJDIR)/%,$(_LR_OBJ))


$(OBJDIR)/%.o : $(LIBDIR)/%.cpp $(INCLUDEDIR)/%.h
	$(CC) $(OPTIONS) $(DEBUG) -c -I$(INCLUDEDIR) -o $@ $<

all: 	src/log_agent.cpp src/nn_agent.cpp $(LR_OBJ) $(U_OBJ) $(NN_OBJ)
	$(CC) $(OPTIONS) $(DEBUG) -I$(INCLUDEDIR) src/log_agent.cpp $(LR_OBJ) $(U_OBJ) -o log_agent $(ALE)
	$(CC) $(OPTIONS) $(DEBUG) -I$(INCLUDEDIR) src/nn_agent.cpp $(NN_OBJ) $(U_OBJ) -o nn_agent $(ALE)
	$(CC) $(OPTIONS) $(DEBUG) src/data_agent.cpp -o data_agent $(ALE) $(NCURSES)

logbot: src/log_bot.cpp $(LR_OBJ) $(U_OBJ)
	$(CC) $(OPTIONS) $(DEBUG) -I$(INCLUDEDIR) src/log_bot.cpp $(LR_OBJ) $(U_OBJ) -o log_bot $(ALE)

log:	src/log_agent.cpp $(LR_OBJ) $(U_OBJ)
	$(CC) $(OPTIONS) $(DEBUG) -I$(INCLUDEDIR) src/log_agent.cpp $(LR_OBJ) $(U_OBJ) -o log_agent $(ALE)

net:	src/nn_agent.cpp $(NN_OBJ) $(U_OBJ)
	$(CC) $(OPTIONS) $(DEBUG) -I$(INCLUDEDIR) src/nn_agent.cpp $(NN_OBJ) $(U_OBJ) -o nn_agent $(ALE)

netbot: src/nn_bot.cpp $(NN_OBJ) $(U_OBJ)
	$(CC) $(OPTIONS) $(DEBUG) -I$(INCLUDEDIR) src/nn_bot.cpp $(NN_OBJ) $(U_OBJ) -o nn_bot $(ALE)

data:	src/data_agent.cpp
	$(CC) $(OPTIONS) $(DEBUG) src/data_agent.cpp -o data_agent $(ALE) $(NCURSES)

clean:
	rm -f $(OBJ) $(AGENTS) 
