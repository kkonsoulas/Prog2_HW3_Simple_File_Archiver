OBJ = objdb.o project3.o
CFLAGS = -Wall -g
CC = gcc

project3: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

%.o: %.c objdb.h
	$(CC) $(CFLAGS) $< -c 

run: test1 test2 test3 test4 test5

test1:
	cd project3tests/ ;\
	../project3  < a_in_1 > my_out_1 2>/dev/null;\
	diff a_out_1 my_out_1

test2:
	cd project3tests/ ;\
	../project3 < a_in_2 > my_out_2 2>/dev/null;\
	diff a_out_2 my_out_2

test3:
	cd project3tests/ ;\
	../project3 < a_in_3 > my_out_3 2>/dev/null;\
	diff a_out_3 my_out_3

test4:
	cd project3tests/ ;\
	../project3 < a_in_4 > my_out_4 2>/dev/null;\
	diff a_out_4 my_out_4

test5:
	cd project3tests/ ;\
	../project3 < a_in_5 > my_out_5 2>/dev/null;\
	diff a_out_5 my_out_5

.PHONY: clean
clean:
	rm -f *.o
	rm -f project3tests/test*
	rm -f project3tests/my*
