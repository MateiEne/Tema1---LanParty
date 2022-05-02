build:
	gcc *.c -Wall -o lanParty
run: build
	./lanParty c.in d.in r.out
clean:
	rm -f *.c
