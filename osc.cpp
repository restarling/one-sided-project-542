#include <cmath>
#include <mpi.h>

void matmat(int n, float* A, float* B, float* C) {
    float val;
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            val = C[row*n+col];
            for (int k = 0; k < n; k++)
                val += A[row*n+k] * B[k*n+col];
            C[row*n+col] = val;
    	}
    }
}

void mpi_communicate(int send_proc, int recv_proc, int tag, 
	int size, int send_first, float* sendbuf, float* recvbuf) {

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Request send_request, recv_request;
	MPI_Status send_status, recv_status;

    if (send_proc == rank) memcpy(recvbuf, sendbuf, size*sizeof(float));
    else if (send_first) {
        MPI_Isend(sendbuf, size, MPI_FLOAT, send_proc, tag, MPI_COMM_WORLD, &send_request);
        MPI_Irecv(recvbuf, size, MPI_FLOAT, recv_proc, tag, MPI_COMM_WORLD, &recv_request);
    	MPI_Wait(&send_request, &send_status);
    	MPI_Wait(&recv_request, &recv_status);
    }
    else {
        MPI_Irecv(recvbuf, size, MPI_FLOAT, recv_proc, tag, MPI_COMM_WORLD, &recv_request);
        MPI_Isend(sendbuf, size, MPI_FLOAT, send_proc, tag, MPI_COMM_WORLD, &send_request);
        MPI_Wait(&send_request, &send_status);
        MPI_Wait(&recv_request, &recv_status);
    }
}

void osc_communicate_fence(int send_proc, int recv_proc, int tag, 
	int size, int send_first, float* sendbuf, float* recvbuf, int rank_col) {

    int rank, newrank;
    MPI_Comm comm;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_split(MPI_COMM_WORLD, rank_col, rank, &comm);
	MPI_Comm_rank(comm, &newrank);
	//sus
    float winbuf[size*sizeof(float)] = {0.0};
    MPI_Win win;
    MPI_Win_create(&winbuf, (MPI_Aint)(size) * sizeof(float), sizeof(float), MPI_INFO_NULL, comm, &win);
    MPI_Win_fence(0, win);    

    if (send_proc == rank) memcpy(recvbuf, sendbuf, size*sizeof(float));
    else if (send_first) {
    	MPI_Put(&sendbuf, size, MPI_FLOAT, recv_proc, 0, size, MPI_FLOAT, win);
    }
    else {
    	MPI_Put(&sendbuf, size, MPI_FLOAT, recv_proc, 0, size, MPI_FLOAT, win);
    }
    MPI_Win_fence(0, win);
    MPI_Win_free(&win);
    MPI_Comm_free(&comm);
}	//endsus


int get_proc(int row, int col, int sq_procs) {
    return row*sq_procs + col;
}

void get_init_procs(int rank_row, int rank_col, int sq_num_procs,
        int* send_proc_A, int* send_proc_B, int* recv_proc_A, int* recv_proc_B) {
    *send_proc_A = get_proc(rank_row, rank_col-rank_row, sq_num_procs);
    *send_proc_B = get_proc(rank_row-rank_col, rank_col, sq_num_procs);
    *recv_proc_A = get_proc(rank_row, rank_col+rank_row, sq_num_procs);
    *recv_proc_B = get_proc(rank_row+rank_col, rank_col, sq_num_procs);
    if (rank_col+rank_row >= sq_num_procs) {
        *recv_proc_A = get_proc(rank_row, rank_col+rank_row-sq_num_procs, sq_num_procs);
        *recv_proc_B = get_proc(rank_row+rank_col-sq_num_procs, rank_col, sq_num_procs);
    }
    if (rank_col - rank_row < 0)
        *send_proc_A = get_proc(rank_row, rank_col-rank_row+sq_num_procs, sq_num_procs);
    if (rank_row - rank_col < 0)
        *send_proc_B = get_proc(rank_row-rank_col+sq_num_procs, rank_col, sq_num_procs);
}

void get_rotation_procs(int rank_row, int rank_col, int sq_num_procs,
        int* send_proc_A, int* send_proc_B, int* recv_proc_A, int* recv_proc_B) {
    *send_proc_A = get_proc(rank_row, rank_col+1, sq_num_procs);
    *send_proc_B = get_proc(rank_row+1, rank_col, sq_num_procs);
    *recv_proc_A = get_proc(rank_row, rank_col-1, sq_num_procs);
    *recv_proc_B = get_proc(rank_row-1, rank_col, sq_num_procs);

    if (rank_col == sq_num_procs-1) *send_proc_A = get_proc(rank_row, 0, sq_num_procs);
    if (rank_row == sq_num_procs-1) *send_proc_B = get_proc(0, rank_col, sq_num_procs);
    if (rank_col == 0) *recv_proc_A = get_proc(rank_row, sq_num_procs-1, sq_num_procs);
    if (rank_row == 0) *recv_proc_B = get_proc(sq_num_procs-1, rank_col, sq_num_procs);
}

void swap(float** send_A, float** recv_A, float** send_B, float** recv_B) {
    float* tmp;
    tmp = *send_A;
    *send_A = *recv_A;
    *recv_A = tmp;

    tmp = *send_B;
    *send_B = *recv_B;
    *recv_B = tmp;
}

void mpi_cannon(float* A, float* B, float* C,
        int n, int sq_num_procs, int rank_row, int rank_col)
{
    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int size = n*n;

    float* send_A = new float[size];
    float* recv_A = new float[size];
    float* send_B = new float[size];
    float* recv_B = new float[size];

    int send_proc_A, send_proc_B;
    int recv_proc_A, recv_proc_B;
    int tag_a = 1234;
    int tag_b = 4321;

    memset(C, 0, size*sizeof(float));

    // Initial Shift : 
    get_init_procs(rank_row, rank_col, sq_num_procs,
            &send_proc_A, &send_proc_B, &recv_proc_A, &recv_proc_B);
    mpi_communicate(send_proc_A, recv_proc_A, tag_a, size, 
            rank_row && rank_col / rank_row % 2 == 0, A, recv_A);
    mpi_communicate(send_proc_B, recv_proc_B, tag_b, size, 
            rank_col && rank_row / rank_col % 2 == 0, B, recv_B);
    matmat(n, recv_A, recv_B, C);

    // Send and recv A and B from neighborhing processes in proc grid
    get_rotation_procs(rank_row, rank_col, sq_num_procs,
            &send_proc_A, &send_proc_B, &recv_proc_A, &recv_proc_B);
    for (int i = 1; i < sq_num_procs; i++)
    {
        swap(&send_A, &recv_A, &send_B, &recv_B);
        mpi_communicate(send_proc_A, recv_proc_A, tag_a, size, rank_col % 2 == 0,
                send_A, recv_A);
        mpi_communicate(send_proc_B, recv_proc_B, tag_b, size, rank_row % 2 == 0,
                send_B, recv_B);
        matmat(n, recv_A, recv_B, C);
    }

    delete[] send_A;
    delete[] recv_A;
    delete[] send_B;
    delete[] recv_B;
}

void osc_cannon_fence(float* A, float* B, float* C,
        int n, int sq_num_procs, int rank_row, int rank_col)
{
    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int size = n*n;

    float* send_A = new float[size];
    float* send_B = new float[size];
    float* recv_A = new float[size];
    float* recv_B = new float[size];

    int send_proc_A, send_proc_B;
    int recv_proc_A, recv_proc_B;
    int tag_a = 1234;
    int tag_b = 4321;

    memset(C, 0, size*sizeof(float));

    // Initial Shift : 
    get_init_procs(rank_row, rank_col, sq_num_procs,
            &send_proc_A, &send_proc_B, &recv_proc_A, &recv_proc_B);
    osc_communicate_fence(send_proc_A, recv_proc_A, tag_a, size, 
            rank_row && rank_col / rank_row % 2 == 0, A, recv_A, rank_col);
    osc_communicate_fence(send_proc_B, recv_proc_B, tag_b, size, 
            rank_col && rank_row / rank_col % 2 == 0, B, recv_B, rank_col);
    matmat(n, recv_A, recv_B, C);

    // Send and recv A and B from neighborhing processes in proc grid
    get_rotation_procs(rank_row, rank_col, sq_num_procs,
            &send_proc_A, &send_proc_B, &recv_proc_A, &recv_proc_B);
    for (int i = 1; i < sq_num_procs; i++)
    {
        swap(&send_A, &recv_A, &send_B, &recv_B);
        osc_communicate_fence(send_proc_A, recv_proc_A, tag_a, size, rank_col % 2 == 0,
                send_A, recv_A, rank_col);
        osc_communicate_fence(send_proc_B, recv_proc_B, tag_b, size, rank_row % 2 == 0,
                send_B, recv_B, rank_col);
        matmat(n, recv_A, recv_B, C);
    }

    delete[] send_A;
    delete[] recv_A;
    delete[] send_B;
    delete[] recv_B;
}

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);

    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int N = 128;
    int sq_num_procs = sqrt(num_procs);
    int rank_row = rank / sq_num_procs;
    int rank_col = rank % sq_num_procs;
    int n = N / sq_num_procs;
    int size = n*n;

    float *h_A, *h_B, *h_C;
    h_A = new float[size];
    h_B = new float[size];
    h_C = new float[size];
    
    for (int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            h_A[i*n+j] = 1.0/(rank*n*n + i*n + j+1);
            h_B[i*n+j] = 1.0/(rank*n*n + i*n + j+1);
        }
    }

    mpi_cannon(h_A, h_B, h_C, n, sq_num_procs, rank_row, rank_col);
	osc_cannon_fence(h_A, h_B, h_C, n, sq_num_procs, rank_row, rank_col);
    delete[] h_A;
    delete[] h_B;
    delete[] h_C;
	MPI_Finalize();
}
