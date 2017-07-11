#include<iostream>
#include<Windows.h>

using namespace std;

struct Data {
	char charData;
	int integerData[500];
};

Data* getData(int length) {
	Data* data = new Data[8];
	for (int i = 0; i < length; i++) {
		data[i].charData = 'A' + i;
		data[i].integerData[499] = i;
	}
	return data;
}


int main() {


	_SYSTEM_INFO systemInfo = { 0 };

	GetSystemInfo(&systemInfo);

	DWORD pageSize = systemInfo.dwPageSize;
	DWORD region = systemInfo.dwAllocationGranularity;

	cout << "Page size: " << systemInfo.dwPageSize << endl;
	cout << "Allocation granularity: " << systemInfo.dwAllocationGranularity << endl;

	LPVOID startingAddressWhereCanBeAllocated = VirtualAlloc(NULL, systemInfo.dwAllocationGranularity, MEM_RESERVE, PAGE_READWRITE);

	LPVOID baseAddressOfPage = VirtualAlloc(startingAddressWhereCanBeAllocated, systemInfo.dwPageSize, MEM_COMMIT, PAGE_READWRITE);
	LPVOID endAddressOfPage = (BYTE*)startingAddressWhereCanBeAllocated + systemInfo.dwPageSize;
	LPVOID endAddressOfRegion = (BYTE*)startingAddressWhereCanBeAllocated + systemInfo.dwAllocationGranularity;

	cout << "Base address of region: " << startingAddressWhereCanBeAllocated << endl;
	cout << "End address of allocated page:  " << endAddressOfPage << endl;
	cout << "End address of reserved region: " << endAddressOfRegion << endl << endl;

	// Writing data
	LPVOID currentPos = baseAddressOfPage;

	int integerToWrite = 1234;
	double doubleToWrite = 3.14;

	cout << "Process: Starting writing int and double to allocated memory." << endl;

	CopyMemory(currentPos, &integerToWrite, sizeof(int));
	currentPos = (BYTE*)currentPos + sizeof(int);

	CopyMemory(currentPos, &doubleToWrite, sizeof(double));
	currentPos = (BYTE*)currentPos + sizeof(double);

	cout << "Process: Variables written successfully. Current positio: " << currentPos << endl;
	cout << "Process: Starting writting Data structures to allocated memory." << endl;
	Data* data = getData(8);


	cout << "Process: Size of single structure: " << sizeof(Data) << endl;
	cout << "Process: Size of 8 structures: " << 8 * sizeof(Data) << endl;
	cout << "Process: Starting writting data bit by bit." << endl << endl;


	int pagesAllocated = 1;
	int sizeOfData = 8 * sizeof(Data);
	BYTE* cpDataBlob = (BYTE*)data;
	BYTE* cpMemory = (BYTE*)currentPos;

	BYTE* currentPageEndAddress = (BYTE*)endAddressOfPage;

	for (int i = 0; i < sizeOfData; i++)
	{

		if (cpMemory == currentPageEndAddress)
		{
			(BYTE*)VirtualAlloc((LPVOID)currentPageEndAddress, systemInfo.dwPageSize, MEM_COMMIT, PAGE_READWRITE);
			currentPageEndAddress = currentPageEndAddress + systemInfo.dwPageSize;
			pagesAllocated++;

			cout << "Process: Allocated new page at: " << (LPVOID)currentPageEndAddress << " (Page number: " << pagesAllocated << ")" << endl;
		}

		if (i % sizeof(Data) == 0) cout << "Process: Starting writing New structure to address: " << (LPVOID)cpMemory << endl;

		// Performing byte writting routine 
		*cpMemory = *cpDataBlob;
		cpMemory++;
		cpDataBlob++;

		if (i % sizeof(Data) == sizeof(Data) - 1)
		{
			cout << "Process: Finished writting structure. " << endl;
			char* lpLetter = (char*)(cpMemory - sizeof(Data));
			LPVOID lpIndex = cpMemory - sizeof(int);

			cout << "         Address of letter : " << (LPVOID)lpLetter << ", letter : " << lpLetter[0] << endl;
			cout << "         Address of index  : " << lpIndex << ", index  : " << *((int*)lpIndex) << endl << endl;
		}
	}



	VirtualFree(startingAddressWhereCanBeAllocated, 0, MEM_RELEASE);

	system("PAUSE");
	return 0;
}