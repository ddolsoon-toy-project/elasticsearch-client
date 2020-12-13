CXX=g++
LIBS=-Llib/ -lEsClient -lcurl 
INCLUDES=-Iinclude/

all: esclient_examples 

SRCS=./esclient_examples.cpp

OBJS=$(SRCS:.cpp=.o)

%.o : %.cpp
	@printf "\n\033[0;40;33m ****** Build $@ ($^) Start    ****** \033[0m\n";
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $^
	@printf "\033[0;40;33m ****** Build $@ ($^) Complete ****** \033[0m\n";

esclient_examples: esclient_examples.o
	@printf "\n\n\n\033[0;40;33m ****** Build $@($^) Start       ****** \033[0m\n";
	$(CXX) -o $@ $^ $(LIBS)
	@printf "\033[0;40;33m ****** Build $@($^) Complete    ****** \033[0m\n";

clean:
	rm -rf esclient_examples *.o
