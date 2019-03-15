tp3 : arn.o test_arn.o
	gcc -g -Wall arn.o test_arn.o -o tp3
arn.o : arn.c arn.h
	gcc -c -g -Wall arn.c arn.h
test_arn.o : test_arn.c
	gcc -c -g -Wall test_arn.c

clean :
	rm -rf *.o
	rm -rf *.h.gch