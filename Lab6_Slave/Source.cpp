#include<iostream>
#include<Windows.h>

using namespace std;

/*****************************************/
/*****************************************/
/*******                             *****/
/*******           SLAVE             *****/
/*******                             *****/
/*****************************************/
/*****************************************/

int main(int argc, char* argv[]) {

	cout << "SLAVE: Slave execution started." << endl;
	if (argc < 2)
	{
		cout << "SLAVE: Not enough parameters was passed." << endl;
		cout << "SLAVE: Calling for ExitProcess." << endl << "SLAVE: ";
		system("PAUSE");
		ExitProcess(0);
	}
	else {
		long delay = atoi(argv[1]);
		cout << "SLAVE: Received delay parameter is: " << delay << endl;
		cout << "SLAVE: Starting delay for " << delay << " milliseconds." << endl;
		Sleep(delay);
		cout << "SLAVE: Delay finished" << endl;

		if (argc == 1)
		{
			cout << "SLAVE: Slave was executed manually. Without HANDLE as third parrameter passed." << endl;
		}
		else if (argc == 3)
		{
			HANDLE hMaster = (HANDLE)atoi(argv[2]);
			cout << "SLAVE: Slave received handle from other process." << endl;
			cout << "SLAVE: Handle id: " << (int)hMaster << endl;
		}
	}

	cout << "SLAVE: Finishing Slave execution." << endl;
	cout << "SlAVE: ";
	system("PAUSE");

	return 0;
}