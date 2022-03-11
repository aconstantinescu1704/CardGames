build: tema1.c 
	gcc -o tema1 tema1.c 
run: build
	./tema1
.PHONY: clean
clean:
	rm -f *.o tema1