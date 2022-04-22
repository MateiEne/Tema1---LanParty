build: lanParty.c
	gcc *.c -Wall -o lanParty
run: build
	./lanParty
clean:
	rm -f *.c
