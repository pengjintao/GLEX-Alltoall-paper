#include <mpi.h>
#include <iostream>
#include <algorithm>

using namespace std;

int leaderN = 2;
int comm_rank;
int procn;
int leader_mode = 1;
bool am_i_leader(int r)
{
	int p = r%(procn/2);
	int ppn = 24;
	int divideN = (procn/2) / leaderN;
	divideN = max(divideN,1);
	switch(leader_mode)
	{
	case 0:
		if(p < leaderN) return true;
		break;
	case 1:
		if(p % divideN == 0 && p / divideN < leaderN) return true;
		break;

	}
	return false;
}

int main(int argc,char **argv)
{
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&comm_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &procn);

	leaderN = 8;
	// if(am_i_leader(comm_rank))
	// 					if(comm_rank >= (procn/2))
	// 	cout<<comm_rank<<endl;
	// return 0;

	int msgsz = (1<<20);
	int sz= msgsz;
	int buffer_sz = sizeof(double)*sz;

	double * sendbuf = new double[sz];
	double * recvbuf = new double[sz];
	leaderN = 8;

	int i,loop;

	int k = 0;
	{
	    for (i = 0;i<=20;i++)
	    {
	    	int count = (1<<i);
	    	for (k =0;k<=3;k++)
	    	{
	    		leaderN = (1<<k);
			    int loopN = 10000;
			    if (i > 14) loopN = 512;
				fflush(stdout);
			    MPI_Barrier(MPI_COMM_WORLD);
			    MPI_Barrier(MPI_COMM_WORLD);
			    double startT = MPI_Wtime();
				for(int loop = 0;loop<loopN;loop++)
				{
					if(am_i_leader(comm_rank))
					{
						if(comm_rank >= (procn/2))
						{
							//receive
							MPI_Status status;
							MPI_Recv(recvbuf,count,MPI_DOUBLE,comm_rank-(procn/2),0,MPI_COMM_WORLD,&status);
						}else{
							// cout<<comm_rank<<" ";
							//sender
							MPI_Send(sendbuf,count,MPI_DOUBLE,comm_rank+(procn/2),0,MPI_COMM_WORLD);
						}
					}
				}
			    double endT = MPI_Wtime();

			    double TotalT = (endT - startT)/loopN;
			    MPI_Barrier(MPI_COMM_WORLD);
			    MPI_Barrier(MPI_COMM_WORLD);
			    if(comm_rank == 0)
					// cout<<endl;
				    cout<<1e6*TotalT<<"\t";

	    	}
			    MPI_Barrier(MPI_COMM_WORLD);
			    if(comm_rank == 0) cout<<endl;

	    }
			    // if(comm_rank == 0) cout<<"---------------------------------------------"<<endl;
	}




	return 0;
}