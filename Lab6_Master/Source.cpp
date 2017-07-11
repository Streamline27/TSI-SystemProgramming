#include<iostream>
#include<Windows.h>
#include<tchar.h>

using namespace std;

/*****************************************/
/*****************************************/
/*******                             *****/
/*******          MASTER             *****/
/*******                             *****/
/*****************************************/
/*****************************************/

const int SLAVE_DELAY = 10000;

TCHAR* getSlaveArgs();

int wmain(int argc, wchar_t* argv[]) {

	cout << "MASTER: Starting MASTER execution." << endl;

	STARTUPINFO cif;
	ZeroMemory(&cif, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;

	if (argc > 1) {

		cout << "MASTER: Name of the application to execute has been passed." << endl;
		wcout << L"MASTER: Trying to execute application: " << argv[1] << endl;

		bool executionSucceeded = CreateProcess(argv[1], NULL, NULL, NULL, NULL, NULL, NULL, NULL, &cif, &pi);
		if (executionSucceeded) cout << "MASTER: Execution succeeded." << endl;
		else                    cout << "MASTER: Execution failed." << endl;

		if (executionSucceeded) {
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}
	else {
		cout << "MASTER: No command line arguments passes." << endl;
		cout << "MASTER: Tring to execute SLAVE" << endl;

		TCHAR* slaveArgs = getSlaveArgs();
		wcout << L"MASTER: Command passed to CreateProcess: " << slaveArgs << endl;

		cout << "MASTER: Executing SLAVE" << endl;

		bool executionSucceeded = CreateProcess(NULL, slaveArgs, NULL, NULL, TRUE, NULL, NULL, NULL, &cif, &pi);
		if (executionSucceeded)
		{
			cout << "MASTER: Waiting for Slave to finish." << endl;
			WaitForSingleObject(pi.hProcess, INFINITE);
			cout << "MASTER: Finished waiting" << endl;

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		else cout << "MASTER: Slave execution failed." << endl;
	}

	cout << "MASTER: Finishing MASTER execution." << endl << "MASTER: ";
	system("PAUSE");

	return 0;

}


TCHAR* getSlaveArgs() {
	DWORD processId = GetCurrentProcessId();

	TCHAR *buffer = new TCHAR[100];
	wsprintf(buffer, L"Lab6_Slave.exe %i, %ld\0", SLAVE_DELAY, (long)processId);
	return buffer;
}