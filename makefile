TARGETS = shell

all: $(TARGETS)

%o: %.c
	gcc -g -c -o  $@ $<
shell: core_functions.o main.o cd.o pwd.o exit.o echo.o pinfo.o ls.o history_.o
	gcc -o shell core_functions.o main.o cd.o pwd.o exit.o echo.o pinfo.o ls.o history_.o

.PHONY: clean
clean:
	rm -f $(TARGETS) *.o
