main: main.c
	echo "Compiling main target"
	cc main.c modules/*.c -o ./bin/main
