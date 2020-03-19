#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char* argv[])
{
	if(argc != 3)
		printf("Usage: ./hist <Number of integers> <Number of classes>\n");
	else{
		//MPI Initialize
		int nodes, id;
		MPI_Status status;
		MPI_Init(&argc,&argv);
		MPI_Comm_size(MPI_COMM_WORLD,&nodes);
		MPI_Comm_rank(MPI_COMM_WORLD,&id);
		
		//Retrieve all the arguments
		long int num = atoi(argv[1]);
		int class = atoi(argv[2]);
		//printf("num per process: %ld class:%d \n",ntask,class);
		
	
		int data_global[num], data_node[num];
		//data_global = (int *)malloc(num*sizeof(int));
		//data_node = (int *)malloc(num*sizeof(int));
		int  hist_global[class], hist_node[class];
	
		long int k=0, i=0, j=0, sum_check=0;
		for(i=0;i<num;i++){
			hist_global[i] = 0;
			hist_node[i] = 0;
		}

		int class_node = class / nodes;

		//Initialize the data
		srand(time(NULL));
		for(i=0;i<num;i++)
			data_global[i] = 1 + rand()%1000;

		MPI_Scatter(hist_global, class, MPI_INT, hist_node, class, MPI_INT, 0, MPI_COMM_WORLD);
	
		//Calculate the starting and ending index based on the process ID
		int start = id * ( num / nodes);
		int end = (id+1) * ( num / nodes) - 1;
	
		int id_limit, lower_limit, upper_limit, step;
		int node_lower_limit, node_upper_limit;
		id_limit = 1000 / nodes;
		step = 1000 / class;

		lower_limit = (id) * id_limit + 1;
		upper_limit = lower_limit - 1 + step;

		//Histogram - all nodes have all data but divided classes
		for(i=0; i<class_node;i++){
	
			//Print and sort them
			//printf(" %d-%d\t:\t ",lower_limit,upper_limit);
			for(j=0;j<num;j++){
				if(data_global[j] > lower_limit && data_global[j]<=upper_limit){
					//printf("%d ",data_global[j]);
					hist_node[i]++;
				}
			}
			
			//Calculate the ranges of each class
			lower_limit += step;
			upper_limit = lower_limit + step;
			upper_limit = (upper_limit>1000)? 1000 : upper_limit;

			//printf("\n");
		}
		//printf("\n");

		//Print the histogram of each node to check if each is working
		/*for(i=0;i<class;i++){
			printf("hist class %d: %d\n", i, hist_node[i]);
		}*/
		
		MPI_Gather(hist_node, class_node, MPI_INT, hist_global, class_node, MPI_INT, 0, MPI_COMM_WORLD);

		if(id == 0){	
			for(i=0;i<class;i++){
				int lower_limit = i*(1000/class)+1;
				int upper_limit = ((i+1)*(1000/class));
				upper_limit = (upper_limit>1000)? 1000 : upper_limit;

				printf(" %d-%d\t: %d\n",lower_limit,upper_limit, hist_global[i]);
			}
		}

		MPI_Finalize();
	
	}


	return 0;
}
