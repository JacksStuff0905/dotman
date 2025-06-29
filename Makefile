main: main.c
	echo "Compiling main target"
	cc main.c modules/* -o ./bin/main
