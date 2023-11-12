#include <cstdlib.h>

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

void communicate(int send_proc, int recv_proc,
        int tag, int size, int send_first,
        float* sendbuf, float* recvbuf) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (send_proc == rank) memcpy(recvbuf, sendbuf, size*sizeof(float));
    else if (send_first) {
        MPI_Send(sendbuf, size, MPI_FLOAT, send_proc, tag, MPI_COMM_WORLD);
        MPI_gRecv(recvbuf, size, MPI_FLOAT, recv_proc, tag, MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
    }
    else {
        MPI_Recv(recvbuf, size, MPI_FLOAT, recv_proc, tag, MPI_COMM_WORLD,
                MPI_STATUS_IGNORE);
        MPI_Send(sendbuf, size, MPI_FLOAT, send_proc, tag, MPI_COMM_WORLD);
    }
}

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

int main(int argc, char* argv[]) {
    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int N = 128;
    int sq_num_procs = sqrt(num_procs);
    int rank_row = rank / sq_num_procs;
    int rank_col = rank % sq_num_procs;
    int n = N / sq_num_procs;
    int size = n*n;

    float *h_A, *h_B, *h_C
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

    mpi_cannon(h_A, h_B, h_C, n, sq_num_procs, rank_row, rank_col);;

    for (int i = 0; i < size; i++) {
        ASSERT_NEAR(h_C_CPU[i], h_C_cuda[i], 1e-5);
        ASSERT_NEAR(h_C_CPU[i], h_C_copy[i], 1e-5);
    }

    delete[] h_A;
    delete[] h_B;
    delete[] h_C;;
}
