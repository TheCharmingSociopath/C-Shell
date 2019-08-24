TARGETS = shell

all: $(TARGETS)

%o: %.c
	gcc -g -c -o  $@ $<
shell: core_functions.o main.o cd.o pwd.o exit.o echo.o pinfo.o ls.o history_.o nightswatch.o
	gcc -o shell core_functions.o main.o cd.o pwd.o exit.o echo.o pinfo.o ls.o history_.o nightswatch.o

.PHONY: clean
clean:
	rm -f $(TARGETS) *.o
