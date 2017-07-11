#include<iostream>
#include<limits.h>
#include<Windows.h>

#define _IF_SYNC if (SHOULD_SYNCHRONIZE) 

using namespace std;


long* getNumbers(int &size) {

	int numElements;

	cout << "Enter number of elements: ";
	cin >> numElements;

	long* numbers = new long[numElements];
	for (int i = 0; i < numElements; i++)
	{
		cout << "Enter element " << i << ": ";
		cin >> numbers[i];
	}

	size = numElements;
	return numbers;
}

long findMin(long* elements, int length) {
	cout << "MAIN - FIND_MIN: Execution started." << endl;
	long min = LONG_MAX;
	for (int i = 0; i < length; i++) {
		cout << "MAIN - FIND_MIN: Comparing " << elements[i] << " and " << min << endl;
		if (elements[i] < min) {
			min = elements[i];
			cout << "MAIN - FIND_MIN: New min elemet is: " << min << endl;
			Sleep(7);
		}
	}
	cout << "MAIN - FIND_MIN: Execution finished." << endl;
	return min;
}

long findMax(long* elements, int length) {
	cout << "MAIN - FIND_MAX: Execution started." << endl;
	long max = LONG_MIN;
	for (int i = 0; i < length; i++) {
		cout << "MAIN - FIND_MAX: Comparing " << elements[i] << " and " << max << endl;
		if (elements[i] > max) {
			max = elements[i];
			cout << "MAIN - FIND_MAX: New max elemet is: " << max << endl;
			Sleep(7);
		}
	}
	cout << "MAIN - FIND_MAX: Execution finished." << endl;
	return max;
}

double findAverage(long* elements, int length) {
	long sum = 0;
	for (int i = 0; i < length; i++) sum += elements[i];
	return sum / length;
}

double findAverageAmongEven(long* elements, int length) {
	cout << "WORKER - EVEN_AVERAGE: Execution started." << endl;
	long sum = 0;
	for (int i = 0; i < length; i++) {
		cout << "WORKER - EVEN_AVERAGE: Checking if index " << i << " is even. " << endl;
		if (i % 2 == 0) {
			cout << "WORKER - EVEN_AVERAGE: Accumulating " << elements[i] << endl;
			sum += elements[i];
			Sleep(12);
		}
	}
	cout << "WORKER - EVEN_AVERAGE: Execution finished." << endl;
	return sum / ((int)(length / 2) + 1);
}

double findAverageWithSleep(long* elements, int length) {
	cout << "WORKER - FIND_AVERAGE: Execution started." << endl;
	long sum = 0;
	for (int i = 0; i < length; i++) {
		cout << "WORKER - FIND_AVERAGE: Accumulating " << elements[i] << endl;
		sum += elements[i];
		Sleep(12);
	}
	cout << "WORKER - FIND_AVERAGE: Execution finished." << endl;
	return sum / length;
}



long countAboveAverage(long* elements, int length) {
	double average = findAverage(elements, length);
	long count = 0;
	for (int i = 0; i < length; i++) if (elements[i] > average) count++;
	return count;
}



/*************************************************************************************************/
/*************************************************************************************************/

struct EnteredNumbers {
	int numElements;
	long* elements;
};


DWORD WINAPI threadWorker(LPVOID);
CRITICAL_SECTION cs;

const bool SHOULD_SYNCHRONIZE = true;

int main() {
	cout << "MAIN:  Thread Execution started." << endl;
	if (SHOULD_SYNCHRONIZE) InitializeCriticalSection(&cs);

	EnteredNumbers numbers;
	numbers.elements = getNumbers(numbers.numElements);
	cout << endl;


	// Create worker
	DWORD threadWorkerId;
	HANDLE hThreadWorker = CreateThread(NULL, 0, threadWorker, &numbers, 0, &threadWorkerId);

	if (SHOULD_SYNCHRONIZE) EnterCriticalSection(&cs);
	cout << "MAIN:  Min element: " << findMin(numbers.elements, numbers.numElements) << endl;
	if (SHOULD_SYNCHRONIZE) LeaveCriticalSection(&cs);

	if (SHOULD_SYNCHRONIZE) EnterCriticalSection(&cs);
	cout << "MAIN:  Max element: " << findMax(numbers.elements, numbers.numElements) << endl;
	if (SHOULD_SYNCHRONIZE) LeaveCriticalSection(&cs);


	WaitForSingleObject(hThreadWorker, INFINITE); 	// Wait for worker to finish

													//if (SHOULD_SYNCHRONIZE) EnterCriticalSection(&cs);
	cout << "MAIN:  Above average: " << countAboveAverage(numbers.elements, numbers.numElements) << endl;
	//if (SHOULD_SYNCHRONIZE) LeaveCriticalSection(&cs);

	system("PAUSE");
	return 0;
}

DWORD WINAPI threadWorker(LPVOID lpData) {
	cout << "WORKER:  Thread Execution started." << endl;
	EnteredNumbers* numbers = (EnteredNumbers*)lpData;

	if (SHOULD_SYNCHRONIZE) EnterCriticalSection(&cs);
	cout << "WORKER:  Average: " << findAverageWithSleep(numbers->elements, numbers->numElements) << endl;
	if (SHOULD_SYNCHRONIZE) LeaveCriticalSection(&cs);

	if (SHOULD_SYNCHRONIZE) EnterCriticalSection(&cs);
	cout << "WORKER:  Average among even: " << findAverageAmongEven(numbers->elements, numbers->numElements) << endl;
	if (SHOULD_SYNCHRONIZE) LeaveCriticalSection(&cs);

	cout << "Worker:  Thread Execution finished." << endl;

	return 0;
}
