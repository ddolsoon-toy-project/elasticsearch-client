CXX=g++
CXXFLAGS= -fPIC -Wall $(DEFINES)
LIBS=-lcurl -L../common/lib -ljsoncpp -ltinyxml2 
INCLUDES=-I../common/include/

LDSO=$(CXX) -shared -rdynamic -fPIC

SRCS=./EsClient.cpp \
	 ./QueryMapper.cpp \
	 ./EsSearch.cpp \
	 ./EsIndex.cpp \
	 ./EsDocument.cpp \
	 ./util/StringUtil.cpp \
	 ./util/HttpUtil.cpp

ifeq  ($(unit), on)
CXXFLAGS += -fprofile-arcs -ftest-coverage -fno-exceptions
LDSO += -fprofile-arcs -ftest-coverage -fno-exceptions
endif

ifeq ($(debug), on)
CXXFLAGS += -g
endif

OBJS=$(SRCS:.cpp=.o)

TARGET=libEsClient.so libEsClient.a

all: $(TARGET)

%.o : %.cpp
	@printf "\n\033[0;40;33m ****** Build $@ ($<) Start    ****** \033[0m\n";
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<
	@printf "\033[0;40;33m ****** Build $@ ($<) Complete ****** \033[0m\n";

libEsClient.so: $(OBJS) 
	@printf "\n\n\n\033[0;40;33m ****** Build $@ Start       ****** \033[0m\n";
	$(LDSO) $(CXXFLAGS) $(OBJS) $(LIBS) -o $@
	@printf "\033[0;40;33m ****** Build $@ Complete    ****** \033[0m\n";

libEsClient.a: $(OBJS)
	ar -x ../common/lib/libjsoncpp.a
	ar -x ../common/lib/libtinyxml2.a
	ar srcv libEsClient.a *.o util/*.o
	@cp libEsClient.a ../examples/lib/
	@cp *.h ../examples/include/elasticsearch

clean:
	@printf "\033[0;40;33m ****** Make Clean Start    ****** \033[0m\n";
	rm -f $(TARGET) $(OBJS) *.o *.gcno *.gcda
	@printf "\033[0;40;33m ****** Make Clean Complete ****** \033[0m\n";

