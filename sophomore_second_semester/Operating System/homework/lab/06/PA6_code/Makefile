all:
	gcc -lpthread -o main ./multi_thread_calculate_pi_MC.c

output: 
	./main -t 1 > ./result/output/test_thread=1.out
	./main -t 2 > ./result/output/test_thread=2.out
	./main -t 4 > ./result/output/test_thread=4.out
	./main -t 8 > ./result/output/test_thread=8.out
	./main -t 16 > ./result/output/test_thread=16.out
	./main -t 32 > ./result/output/test_thread=32.out
	./main -t 64 > ./result/output/test_thread=64.out

clear:
	rm -f main