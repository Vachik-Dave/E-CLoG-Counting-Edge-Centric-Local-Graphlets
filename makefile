CC=g++ -std=c++11 -g -O3
Link=-fopenmp

all: parallel

parallel:
	$(CC) $(Link) graphlet_counting_omp.cpp -o E_CLoG_omp 

clean:
	rm E_CLoG_omp
