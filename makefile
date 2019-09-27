TARGETS = shell

all: $(TARGETS)

%o: %.c
	gcc -g -c -o  $@ $<
shell: core_functions.o main.o cd.o pwd.o exit.o echo.o pinfo.o ls.o history_.o nightswatch.o setenv_.o unsetenv_.o jobs.o kjob.o fg.o bg.o overkill.o
	gcc -o shell core_functions.o main.o cd.o pwd.o exit.o echo.o pinfo.o ls.o history_.o nightswatch.o setenv_.o unsetenv_.o jobs.o kjob.o fg.o bg.o overkill.o

.PHONY: clean
clean:
	rm -f $(TARGETS) *.o
