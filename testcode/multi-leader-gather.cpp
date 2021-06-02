#include <mpi.h>
#include <iostream>

using namespace std;

bool am_i_leader(int r)
{
	int ppn = 24;
	int mode = 1;
	switch(mode)
	{
	case 0:
		if(r % ppn == 0) return true;

	}
	return true;
}

int main(int argc,char **argv)
{
	int rank,procn;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&procn);


	int msgsz = (1<<20);
	int sz= msgsz*procn;
	int buffer_sz = sizeof(double)*sz;

	double * sendbuf = new double[sz];
	double * recvbuf = new double[sz];
	double * gather_buffer = 0;
	if (am_i_leader(rank))
			gather_buffer = new double[sz];

	int i,loop;

    for( i = 0;i<20;i++)
    {
    	int count = (1<<i);
	    MPI_Win win;
	    MPI_Win_create(sendbuf, buffer_sz, 1, MPI_INFO_NULL, MPI_COMM_WORLD, &win);
	    MPI_Win_fence(0, win);
	    int loopN = 1000;
	    if(i>16) loopN = 400;
	    MPI_Barrier(MPI_COMM_WORLD);
	    double startT = MPI_Wtime();
	    for(loop = 0;loop < 1000;loop++ )
	    {
	    	int target = 0;
	    	for(target = 0;target < procn;target++)
	    	{
	    		MPI_Get(gather_buffer,count,MPI_DOUBLE,target,0,count,MPI_DOUBLE,win);
	    	}
	    	MPI_Win_fence(0, win);
	    }
	    double endT = MPI_Wtime();
	    double TotalT = (endT - startT)/loop;
	    if(rank == 0)
		    cout<<1e6*TotalT<<endl;

	    MPI_Win_fence(0, win);
	    MPI_Win_free(&win);

    }


	return 0;
}