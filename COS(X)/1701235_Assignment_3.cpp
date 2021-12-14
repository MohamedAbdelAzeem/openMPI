// MPI.cpp : This file contains the 'main' function. Program execution begins and ends there.
// MPI Installiation video : https://www.youtube.com/watch?v=BA_Mqi3a9HI
// execute with  mpiexec -n 4 .\MPI.exe

#include <iostream>
#include <math.h>
#include <iomanip>
#include "mpi.h"
using namespace std;
#define _CRT_SECURE_NO_WARNINGS


void Get_Input(int Proccess_Rank, int Comm_Size, int* i, int* n, double* x);
long double Calculate_Result(int Interval_Start, int Interval_End, double x);
double factorial(int n);

int main(void)
{
    int rank;  /* Current proccess rank */
    double time1 = 0, time2, duration;
    int size; /* Size of the commuincator  */
    double x; //x value to calculate cos(x)
    int i; //upper summation limit 
    int  n;   //number of proccesses 
  //  printf("Parallel Version:\n");
    MPI_Init(NULL, NULL);
    /* Each process inside of a communicator is assigned an incremental rank starting from zero  */
    /* returns the rank of a process in a communicator */
    /* our commuincator is MPI_COMM_WORLD */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /*returns the size of a communicator */
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* Get input form user 
    *  x : value to calcualte cos(x)
      i : number of iterations*/
    Get_Input(rank, size, &i, &size, &x);

    /* if i is divisble by number of proccesses : we cannot divide iterations equally */
    if ((i+1) % size != 0)
    {
        MPI_Finalize();
        if (rank == 0)
            printf("Error:**** K is not divisible by %d", size); 
        return 0;
    }

    n = size;
        
    /* Divide iterations for all proccess equally  */
    int Interval_End = 0;
    int Interval_start = 0;
    int chunk_size = (i+1) / n;
    Interval_start = rank * chunk_size;
    Interval_End = Interval_start-1 + chunk_size;
   
    long double proccess_result = 0;
    long double total = 0;
     /* Start calculating time  */
    if (rank == 0)
    {
        time1 = MPI_Wtime();

    }
    /* calculate result for each process on its interval */
     proccess_result = Calculate_Result(Interval_start, Interval_End, x);
       
     //printf("I am %d result = %lf start = %d  end = %d\n", rank, proccess_result, Interval_start, Interval_End);
     if(rank != 0 )
     {
          /* every proccess except process 0 send its result to proccess 0*/
          MPI_Send(&proccess_result, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
        
    }
    else
    {
         /* process 0 add its result and receive all results form all processess */
        total = proccess_result;
        for (int proccess = 1; proccess < n; proccess++)
        {
            MPI_Recv(&proccess_result, 1, MPI_DOUBLE, proccess, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total += proccess_result;
        }


    }

     /* Process 0 print the result */
    if (rank == 0)
    {
        time2 = MPI_Wtime();
        duration = time2 - time1;
        cout << "With " << n <<" Processes, " <<"Cos(" << fixed << x << setprecision(7) << ") = " << fixed << total << setprecision(7) << endl;
        cout << "Time = " << fixed << duration * 1000000 << setprecision(7) << " microseconds";
        //printf("With %d proccess, result is %lf\n", n, total);
        //printf("time : %f microseconds", duration*1000000);
    }
   
  
   // printf("I am %d of %d", rank, size);

    MPI_Finalize();      /* is used to clean up the MPI environment. (Release Resources) */
    return 0;
}

void Get_Input(int Proccess_Rank, int Comm_Size, int* i, int* n, double* x)
{

    if (Proccess_Rank == 0)
    {
        //printf("Enter i, x, and n\n");
        //scanf_s("%d %lf", i, x);
        cout << "Enter i and x " << endl;
        cin >> *i >> *x;
       
     //  printf("%d %f %d\n", *i, *x, *n);
        /* Send input to all processes */
        for (int destination = 1; destination < *n; destination++)
        {
            MPI_Send(i, 1, MPI_DOUBLE, destination, 0, MPI_COMM_WORLD);
            MPI_Send(n, 1, MPI_INT, destination, 0, MPI_COMM_WORLD);
            MPI_Send(x, 1, MPI_DOUBLE, destination, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        /* receive inputs from process 0 */
        MPI_Recv(i, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(x, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

       // printf("I am %d received %f and %d and %f", Proccess_Rank, *i, *n,*x );
    }
}


long double Calculate_Result(int Interval_Start, int Interval_End,double x)
{
    long double result = 0;
   
    for (int i = Interval_Start; i <= Interval_End; i++)
    {
          /*cos(x) */
        result += (powl(-1, i) * powl(x, 2 * (double)i)) / (factorial(2 * i));
    }                                                                 

    return result;
} 
double factorial(int n)
{
    double res = 1, i;
    for (i = 2; i <= n; i++)
        res *= i;
    return res;
}