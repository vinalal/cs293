.PHONY: all test clean

COMPILE = g++ -std=c++2a

part1:
	$(COMPILE) main.cpp -o part1
	./part1 < inputs/p1-input.txt > result.txt
	diff -Bw result.txt output/p1-output.txt
	rm part1

part2:
	$(COMPILE) main.cpp -o part2
	./part2 < inputs/p2-input.txt > result.txt
	diff -Bw result.txt output/p2-output.txt
	rm part2

part3:
	$(COMPILE) main.cpp -o part3
	./part3 < inputs/p3-input.txt
	rm part3

