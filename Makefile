CC=clang

run: build
	@./todo

create: build
	./todo -c "Hello"

delete: build
	./todo -d 1

list: build
	./todo -l

build:
	@$(CC) main.cpp -lsqlite3 -o todo 
