#include<windows.h>
#include<iostream>
#include<Lmcons.h>
#include<tchar.h>

using namespace std;

TCHAR* getDriveType(TCHAR* p) {

	UINT type = GetDriveType(p);

	if (type == DRIVE_FIXED)			return _TEXT("Fixed");
	else if (type == DRIVE_REMOTE)		return _TEXT("Remote");
	else if (type == DRIVE_REMOVABLE)	return _TEXT("Removable");
	else if (type == DRIVE_CDROM)		return _TEXT("CD Rom");
	else if (type == DRIVE_RAMDISK)		return _TEXT("RAM disk");
	else if (type == DRIVE_NO_ROOT_DIR) return _TEXT("Not a root dir.");
	else if (type == DRIVE_UNKNOWN)		return _TEXT("Unknown");

	else return NULL;
}

int main() {

	/*******************************************************************************/
	/***********                    GetVersionEx                          **********/

	OSVERSIONINFO versionInfo = {};
	versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&versionInfo);

	cout << "Build Number: " << versionInfo.dwBuildNumber << endl;
	cout << "Platform ID: " << versionInfo.dwPlatformId << endl;
	cout << "Major Version: " << versionInfo.dwMajorVersion << endl;
	cout << "Minor Version: " << versionInfo.dwMinorVersion << endl;
	_tprintf(_TEXT("CSD Version: %s\n\n"), versionInfo.szCSDVersion);


	TCHAR* computerName = new TCHAR[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD cpNameSize = sizeof(TCHAR) * MAX_COMPUTERNAME_LENGTH;

	GetComputerName(computerName, &cpNameSize);


	_tprintf(_TEXT("Computer Name: %s\n\n"), computerName);

	/*******************************************************************************/
	/***********                    GetLocalTime                          **********/

	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);
	_tprintf(_TEXT("The local time is: %02d:%02d:%02d\n"), systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

	/*******************************************************************************/
	/***********                   GetTimeZoneInformation                 **********/

	TIME_ZONE_INFORMATION timeZoneInformation = { 0 };
	DWORD code = GetTimeZoneInformation(&timeZoneInformation);

	cout << "Return value: " << code << endl << endl;

	cout << "TIME_ZONE_INFORMATION" << endl;

	_tprintf(_TEXT("Day Light Name: %s\n"), timeZoneInformation.DaylightName);
	_tprintf(_TEXT("Standard Name: %s\n"), timeZoneInformation.StandardName);

	cout << "Standard Bias: " << timeZoneInformation.StandardBias << endl;
	cout << "Daylight Bias: " << timeZoneInformation.DaylightBias << endl;

	_tprintf(_TEXT("Standard Date: %02d:%02d:%02d\n"), timeZoneInformation.StandardDate.wHour, timeZoneInformation.StandardDate.wMinute, timeZoneInformation.StandardDate.wSecond);
	_tprintf(_TEXT("DayLight Date: %02d:%02d:%02d\n\n"), timeZoneInformation.DaylightDate.wHour, timeZoneInformation.DaylightDate.wMinute, timeZoneInformation.DaylightDate.wSecond);


	/********************************************************************************/
	/***********  GetUserDefaultLangID , VerLanguageName , GetUserName     **********/

	LANGID defaultLangId = GetUserDefaultLangID();

	TCHAR *languageName = new TCHAR[255];
	VerLanguageName(defaultLangId, languageName, 255);

	cout << "Default language id: " << defaultLangId << endl;
	_tprintf(_TEXT("Language name: %s \n\n"), languageName);

	TCHAR* userName = new TCHAR[UNLEN + 1];
	DWORD nameSize = sizeof(TCHAR) * UNLEN;
	GetUserName(userName, &nameSize);

	_tprintf(_TEXT("User Name: %s \n\n"), userName);


	/*******************************************************************************/
	/*********** GetCurrentDirectory , GetWindowsDirectory , GetSystemDirectory  **********/

	TCHAR dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, dir);

	TCHAR winDir[MAX_PATH];
	GetWindowsDirectory(winDir, MAX_PATH);

	TCHAR sysDir[MAX_PATH];
	GetSystemDirectory(sysDir, MAX_PATH);

	_tprintf(_TEXT("Current Directory: %s \n"), dir);
	_tprintf(_TEXT("Windows Directory: %s \n"), winDir);
	_tprintf(_TEXT("System Directory : %s \n\n"), sysDir);


	/***************************************************************************************************/
	/*********** GetLogicalDrivers , GetLogicalDriveString , GetDriveType , GetDiskFreeSpace  **********/


	DWORD drives = GetLogicalDrives();
	cout << "Drives: " << drives << endl;

	int bufferSize = GetLogicalDriveStrings(0, NULL);
	TCHAR *sdrives = new TCHAR[bufferSize];
	GetLogicalDriveStrings(bufferSize, sdrives);

	
	for (TCHAR* p = sdrives; *p; p += 4) {

		wcout << "Drive: " << p;

		DWORD sectorsPerCluster;
		DWORD bytesPerSector;
		DWORD numberOfFreeClusters;
		DWORD totalNumberOfClusters;

		GetDiskFreeSpace(p, &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters);

		ULONGLONG bytesPerCluster = (ULONGLONG)bytesPerSector * (ULONGLONG)sectorsPerCluster;
		ULONGLONG freeBytes  = bytesPerCluster * (ULONGLONG)numberOfFreeClusters;
		ULONGLONG totalBytes = bytesPerCluster * (ULONGLONG)totalNumberOfClusters;

		cout << " Free space: " << freeBytes << "/" << totalBytes << " (bytes)" << " Drive type: " << getDriveType(p) << endl;
	}

	system("PAUSE");


	return 0;
}



