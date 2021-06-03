#include <mpi.h>
#include <iostream>
#include <algorithm>

using namespace std;

int leaderN = 2;
int comm_rank;
int procn;
int leader_mode = 0;
bool am_i_leader(int r)
{
	int ppn = 24;
	int divideN = procn / leaderN;
	divideN = max(divideN,1);
	switch(leader_mode)
	{
	case 0:
		if(r < leaderN) return true;
		break;
	case 1:
		if(r % divideN == 0 && r / divideN < leaderN) return true;
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

	int msgsz = (1<<20);
	int sz= msgsz*procn;
	int buffer_sz = sizeof(double)*sz;

	double * sendbuf = new double[sz];
	double * recvbuf = new double[sz];
	double * gather_buffer = 0;
	{
				// cout<<comm_rank<<" is a leader"<<endl;
			gather_buffer = new double[sz];
	}

	int i,loop;

	int k = 0;
	for (leader_mode = 0; leader_mode <= 1; leader_mode++)
	{
	    for (i = 0;i<20;i++)
	    {
	    	int count = (1<<i);
	    	for (k =0;k<=3;k++)
	    	{
	    		leaderN = (1<<k);
			    MPI_Win win;
			    MPI_Win_create(sendbuf, buffer_sz, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win);
			    MPI_Win_fence(0, win);
			    int loopN = 16384;
			    if (i > 15) loopN = 512;
			    int realloop = loopN/leaderN;
			    MPI_Barrier(MPI_COMM_WORLD);
			    MPI_Barrier(MPI_COMM_WORLD);
			    double startT = MPI_Wtime();
			    for(loop = 0;loop < realloop;loop++ )
			    {
			    	int target = 0;
			    	if(am_i_leader(comm_rank))
			    	{
			    		// cout<<comm_rank<<" ";
			    		// if(0)
			    		for(int xx = 0;xx<procn;xx++)

				    	{
				    		int target = (comm_rank + xx) % procn;
				    		MPI_Get(gather_buffer,count,MPI_DOUBLE,target,0,count,MPI_DOUBLE,win);
				    	}
			    	}
			    	MPI_Win_fence(0, win);
			    }
			    double endT = MPI_Wtime();
			    double TotalT = (endT - startT)/realloop;
			    if(comm_rank == 0)
				    cout<<1e6*TotalT<<"\t";

			    MPI_Win_fence(0, win);
			    MPI_Win_free(&win);
	    	}
			    if(comm_rank == 0) cout<<endl;

	    }
			    if(comm_rank == 0) cout<<"---------------------------------------------"<<endl;
	}




	return 0;
}