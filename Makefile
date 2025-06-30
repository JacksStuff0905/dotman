main: main.c
	echo "Compiling main target"
	mkdir -p bin
	cc -std=c17 main.c modules/*.c -o bin/main
