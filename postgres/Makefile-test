all: test

test: main.cc pg.cc pg.h helperJson.cc helperJson.h
	g++ main.cc pg.cc helperJson.cc -lpq -lpqxx -ljsoncpp -o$@
