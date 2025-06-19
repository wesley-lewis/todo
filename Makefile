run: build
	@./todo

build:
	@g++ main.cpp -lsqlite3 -o todo 
