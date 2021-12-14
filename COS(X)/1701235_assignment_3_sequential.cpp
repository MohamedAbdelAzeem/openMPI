// MPI.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Installiation video : https://www.youtube.com/watch?v=BA_Mqi3a9HI

#include <iostream>
#include <time.h>
#include <stdio.h>
#include <iomanip>
#include <math.h>
#include <chrono>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;


#define TIME_TEST
using namespace std::chrono;
using namespace std;

double factorial(int n)
{
	double res = 1, i;
	for (i = 2; i <= n; i++)
	res *= i;
	return res;
}
long double Calculate_Result(int Interval_Start, int Interval_End, double x)
{
	long double result = 0;

	for (int i = Interval_Start; i <= Interval_End; i++)
	{

		result += (powl(-1, i) * powl(x, (2 * (long double)i))) / (factorial(2 * i));
	}

	return result;
}

int main(void)
{
	int i = 0;
	double x = 0;
	
	cout << "Enter I and X" << endl;
	cin >> i >> x;


	#ifdef TIME_TEST
	// Get starting timepoint
	auto start = high_resolution_clock::now();
	#endif // TIME_TEST

	long double result = Calculate_Result(0, i, x);

	#ifdef TIME_TEST
	// Get ending timepoint
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	printf("Result = %lf\n", result);
	cout << "Time taken "
	<< duration.count() << " microseconds" << endl;
	#endif // TIME_TEST
	
	// printf(" %f seconds to execute \n", t2);


	return 0;
}




