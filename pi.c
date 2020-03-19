#include<mpi.h>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>

#define DARTS 100000
#define ITER 10000

#define sqr(x) (x*x)

double dartboard(int darts, int *totaldarts){
	long inside=0;
	double x,y;
	int i=0;
	for(i=0;i<darts;i++){
		x = (2*((double)rand()/(double)(RAND_MAX))) - 1;
		y = (2*((double)rand()/(double)(RAND_MAX))) - 1;

		//printf("x:%f y:%f\n",x,y);
		if(sqr(x) + sqr(y) < 1)
			inside++;
		(*totaldarts)++;

	}

	
	return inside;
	
}

int main(int argc, char* argv[])
{
	int tasks, ntask, *totaldarts;
	*totaldarts = 0;
	double taskinside, totalinside, pi, avgpi=0;

	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&tasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&ntask);

	//printf("Num tasks: %d\n",tasks);

	srand(time(NULL));
	int i;
	for(i=0;i<ITER;i++){
		taskinside = dartboard(DARTS,totaldarts);

		MPI_Reduce(&taskinside, &totalinside, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	}
	
	//printf("Total darts: %f Total Inside: %f\n", DARTS, totalinside);	
	pi = 4 * (double)totalinside / (double)(DARTS);
	MPI_Finalize();
	if(ntask == 0)
		printf("Pi: %10.15f\n", pi);	
	return 0;
}
