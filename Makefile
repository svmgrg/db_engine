# Makefile for db_engine

OBJS = main.o expr_eval.o ra_op.o support_fn.o table_fn.o db_rw.o aggregate.o
CC = g++
DEBUG = -g    # write '-g' for switching debugging options on
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

all: main.out

main.out: $(OBJS)		
	$(CC) $(LFLAGS) $(OBJS) -o main.out

main.o: main.cpp support_fn.hh db_rw.hh
	$(CC) $(CFLAGS) main.cpp

expr_eval.o: expr_eval.cpp expr_eval.hh data_str.hh ra_op.hh \
aggregate.hh
	$(CC) $(CFLAGS) expr_eval.cpp

ra_op.o: ra_op.cpp ra_op.hh data_str.hh expr_eval.hh
	$(CC) $(CFLAGS) ra_op.cpp

support_fn.o: support_fn.cpp support_fn.hh ra_op.hh data_str.hh \
	      table_fn.hh
	$(CC) $(CFLAGS) support_fn.cpp

table_fn.o: table_fn.cpp table_fn.hh data_str.hh support_fn.hh
	$(CC) $(CFLAGS) table_fn.cpp

db_rw.o: db_rw.cpp support_fn.hh
	$(CC) $(CFLAGS) db_rw.cpp

aggregate.o: aggregate.cpp aggregate.hh data_str.hh
	$(CC) $(CFLAGS) aggregate.cpp

.PHONY: clean tar

clean:
	-rm *.o *.tar.gz *.out *~
# .o   for object files
# .tar for compressed archive
# .out for final executable
# *~   for emacs backup files

tar:
	tar -czvf db_engine.tar.gz *.cpp *.hh Makefile data.txt \
*.dat README

