
COMPILER:=gcc 
CFLAGS:= -c

SRC:=des.c main.c
OBJ:=$(SRC:%.c=%.o)


all: $(OBJ)
	@echo "all subjects to devices " $^
	$(COMPILER) -o main $^ 


%.o: %.c 
	echo $^
	$(COMPILER) -c $^ 

.PHONY: clean 

clean:
	rm -rf *.o main
