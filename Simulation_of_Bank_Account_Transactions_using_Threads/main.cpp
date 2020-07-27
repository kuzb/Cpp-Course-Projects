#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <random>
#include <time.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <iomanip>

/*
++++++++++++++++++++++++++++
BURAK KUZ
20647
CS204 HW7
++++++++++++++++++++++++++++
*/


using namespace std;

int balance = 0;//initial balance

mutex fMutex;

// # of THREADS for withdraw
const int DECREMENT_THREADS = 3;
// # of THREADS for deposit
const int INCREMENT_THREADS = 1;

int iterationWithdraw, iterationDeposit; //declared to hold iteration values for deposit and withdraw

// Generate random numbers in a thread-safe manner in the range  (min, max).
int random_range(const int & min, const int & max) {
    static mt19937 generator(time(0));
    uniform_int_distribution<int> distribution(min,max);
    return distribution(generator);
}

// prints the local time when it is called
void printTimeNow()
{
	time_t tt = chrono::system_clock::to_time_t(chrono::system_clock::now()); //gets the current time
	struct tm *ptm = new struct tm; //creating the time struct to be used in thread
	localtime_r(&tt, ptm); //converting the time structures
	cout << put_time(ptm,"%X") << endl;
}

//for the thread that is used for a deposit transaction
void depositeIt(int minDeposit,int maxDeposit, ofstream& output)
{
	int iteration = 0;
	while (iteration < iterationDeposit)
	{
		int sleepDeposit = random_range(minDeposit, maxDeposit); //creates a sleep time for the thread via given range

		this_thread::sleep_for(chrono::seconds(sleepDeposit));

		fMutex.lock();
		balance += 1; //deposit money on the account
		cout << "\n1 TL has been deposited: balance is " << balance << ", ";
		printTimeNow();
		iteration++;
		fMutex.unlock();
	}
}

//for threads that are dedicated to the withdrawal transactions
void WithdrawIt(int i,int minWithdraw,int maxWithdraw, ofstream& output)
{
	int iteration = 0;
	while (iteration < iterationWithdraw)
	{
		int sleepWithdraw = random_range(minWithdraw, maxWithdraw); //creates a sleep time for the thread via given range

		this_thread::sleep_for(chrono::seconds(sleepWithdraw));

		fMutex.lock();
		if (balance > 0) //balance must be positive in order to withdraw money
		{
			balance -= 1; //withdrawing money from account
			cout << "\nWithdrawal thread " << i-1 << " successfully withdrawn 1 TL; balance is " << balance << ", ";
			printTimeNow();
		}
		else
		{
			cout << "\nWithdrawal thread " << i-1 << " failed to withdraw 1 TL; balance is " << balance << ", ";
			printTimeNow();
		}
		iteration++;
		fMutex.unlock();
	}
}

int main()
{
	string fileName;
	ofstream outputFile;
	thread action[DECREMENT_THREADS + INCREMENT_THREADS];
	int minWithdraw, maxWithdraw;
	int minDeposit, maxDeposit;


	cout << "Please, enter file name in which balance will be stored: ";
	cin >> fileName;
	outputFile.open(fileName.c_str());

	cout << "Please, enter min and max waiting times between withdrawal transactions per thread(in seconds): \n";
	cin >> minWithdraw >> maxWithdraw;
	cout << "Please, enter min and max waiting times between deposit transactions (in seconds): \n";
	cin >> minDeposit >> maxDeposit;
	cout << "Please, enter the total number of withdrawal transactions per thread: \n";
	cin >> iterationWithdraw;
	cout << "Please, enter the total number of deposit transactions: \n";
	cin >> iterationDeposit;


	cout << "Simulation starts at ";
	printTimeNow();

	//deposit and withdraw operations
	for (int i = 0; i < DECREMENT_THREADS + INCREMENT_THREADS; i++)
	{
		if (i == 0)
		{
			action[i] = thread(&depositeIt, minDeposit, maxDeposit, ref(outputFile));
		}
		else
		{
			action[i] = thread(&WithdrawIt, i, minWithdraw, maxWithdraw, ref(outputFile));
		}
	}

	//causes the main thread to be blocked until the thread on which it was called(action) completes and terminates
	for (int k = 0; k < DECREMENT_THREADS + INCREMENT_THREADS; k++)
	{
		action[k].join();
	}

	string s = to_string(balance);
	outputFile << s;
	cout << "\nSimulation ends at ";
	printTimeNow();

	outputFile.close();
	outputFile.clear();
	return 0;
}
