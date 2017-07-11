#include<windows.h>
#include<tchar.h>
#include<iostream>
#include<vector>
#include<map>
#include <conio.h>
#include <stdlib.h>
#include<string>

#ifdef _UNICODE
#define _tcout wcout
#define _tcin wcin
#else
#define _tcout cout
#define _tcin cin
#endif

using namespace std;

/**********************************************************************************/
/***************                      Domain                        ***************/

struct Student {
	TCHAR name[255];
	TCHAR surname[255];
	TCHAR group[255];


	Student() {}

	Student(LPTSTR name, LPTSTR surname, LPTSTR group) {

		_tcscpy_s(this->name, name);
		_tcscpy_s(this->surname, surname);
		_tcscpy_s(this->group, group);
	}

	void show() {
		_tprintf(_TEXT("Name:    %s\n"), this->name);
		_tprintf(_TEXT("Surname: %s\n"), this->surname);
		_tprintf(_TEXT("Group:   %s\n"), this->group);
	}

};

struct Statistic {
	TCHAR groupName[255];
	long count;

	Statistic() {};

	Statistic(LPTSTR groupName, long count) {
		_tcscpy_s(this->groupName, groupName);
		this->count = count;
	}

	void show() {
		_tcout << "Group: " << groupName << endl;
		_tcout << "Count: " << count << endl;
	}
};

struct FileTimeInfo {
	SYSTEMTIME creationTime;
	SYSTEMTIME lastAccessTime;
	SYSTEMTIME lastWriteTime;
};

/**********************************************************************************/
/**********************************************************************************/

struct StudentDao {

	const LPTSTR FOLDER_PATH = _TEXT("C:\\Students");
	const LPTSTR FILE_PATH = _TEXT("C:\\Students\\stud.dat");

	HANDLE file;
	FileTimeInfo fileTimeInfo;

	bool fileCreated;

public:

	StudentDao() {
		CreateDirectory(FOLDER_PATH, NULL);
		this->file = CreateFile(FILE_PATH, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (GetLastError() == ERROR_ALREADY_EXISTS) this->fileCreated = false;
		else									    this->fileCreated = true;

		constructFileTimeStructure();
	}

	bool fileWasCreated() {
		return fileCreated;
	}

	void create(Student &student) {

		DWORD bytesWritten;
		SetFilePointer(file, 0, NULL, FILE_END);
		WriteFile(file, &student, sizeof(student), &bytesWritten, NULL);
	}

	vector<Student> getAll() {

		vector<Student> students;
		DWORD bytesRed;
		SetFilePointer(file, 0, NULL, FILE_BEGIN);

		for (;;)
		{
			Student student;
			ReadFile(file, &student, sizeof(Student), &bytesRed, NULL);
			if (bytesRed != 0) students.push_back(student);
			else			   break;
		}
		return students;
	}

	FileTimeInfo getFileTimeInfo() {
		return fileTimeInfo;
	}

	~StudentDao() {
		CloseHandle(file);
	}

private:

	void constructFileTimeStructure() {

		FILETIME creationTime, lastAccessTime, lastWriteTime;
		SYSTEMTIME stUTC;

		GetFileTime(file, &creationTime, &lastAccessTime, &lastWriteTime);

		FileTimeToSystemTime(&creationTime, &stUTC);
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &(fileTimeInfo.creationTime));

		FileTimeToSystemTime(&lastAccessTime, &stUTC);
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &(fileTimeInfo.lastAccessTime));

		FileTimeToSystemTime(&lastWriteTime, &stUTC);
		SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &(fileTimeInfo.lastWriteTime));
	}

};

/**********************************************************************************/
/**********************************************************************************/

struct cmp_str
{
	bool operator()(const LPTSTR a, const LPTSTR b) const
	{
		return _tcscmp(a, b) < 0;
	}
};

struct StatisticsService {

	const LPTSTR FOLDER_PATH = _TEXT("C:\\Students");
	const LPTSTR FILE_PATH = _TEXT("C:\\Students\\group.dat");


	void computeAndSave(vector<Student> &students) {
		CreateDirectory(FOLDER_PATH, NULL);
		HANDLE file = CreateFile(FILE_PATH, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		vector<Statistic> statistics = groupByGroup(students);
		for (size_t i = 0; i < statistics.size(); i++) create(statistics[i], file);
		CloseHandle(file);
	}

	vector<Statistic> getAll() {
		vector<Statistic> statistics;

		HANDLE file = CreateFile(FILE_PATH, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); // Open only if exists, else show error

		if (GetLastError() == ERROR_FILE_NOT_FOUND) return statistics;

		DWORD bytesRed;
		SetFilePointer(file, 0, NULL, FILE_BEGIN);

		for (;;)
		{
			Statistic statistic;
			ReadFile(file, &statistic, sizeof(Statistic), &bytesRed, NULL);
			if (bytesRed != 0) statistics.push_back(statistic);
			else			   break;
		}
		CloseHandle(file);
		return statistics;
	}


private:
	vector<Statistic> groupByGroup(vector<Student> &students) {

		map<LPTSTR, Statistic, cmp_str> statsByGroups;

		for (int i = 0; i < students.size(); i++)
		{
			LPTSTR group = students[i].group;

			auto it = statsByGroups.find(group);
			if (it == statsByGroups.end()) statsByGroups.insert(std::pair<LPTSTR, Statistic>(group, Statistic(group, 1)));
			else						   it->second.count++;
		}

		vector<Statistic> statistics;
		for (auto it = statsByGroups.begin(); it != statsByGroups.end(); it++) statistics.push_back(it->second);
		return statistics;
	}

	void create(Statistic &statistic, HANDLE file) {
		DWORD bytesWritten;
		SetFilePointer(file, 0, NULL, FILE_END);
		WriteFile(file, &statistic, sizeof(Statistic), &bytesWritten, NULL);
	}
};

/**********************************************************************************/
/**********************************************************************************/
/*******************             MAIN BLOCK                   *********************/
/**********************************************************************************/
/**********************************************************************************/


void showTopBanner();
void showFileInfo(StudentDao *studentDao);
void showMenu();
void reinitializeScreen();

void performAddStudentRoutine(StudentDao *studentDao);
void performShowStudentsRoutine(StudentDao *studentDao);
void performShowStatisticsRoutine(StudentDao *studentDao);
void performComputeStatisticsRoutine(StudentDao *studentDao);

int main() {
	StudentDao *studentDao = new StudentDao();

	showTopBanner();
	showFileInfo(studentDao);
	showMenu();

	char ch;
	for (;;)
	{

		ch = _getch();

		switch (ch)
		{
		case '1':
			performAddStudentRoutine(studentDao);
			break;
		case '2':
			performShowStudentsRoutine(studentDao);
			break;
		case '3':
			performShowStatisticsRoutine(studentDao);
			break;
		case '4':
			performComputeStatisticsRoutine(studentDao);
			break;
		case '5':
			exit(0);
			break;
		}

		system("PAUSE");
		reinitializeScreen();
	}



}

void performAddStudentRoutine(StudentDao *studentDao) {

	TCHAR *name = new TCHAR[255];
	TCHAR *surname = new TCHAR[255];
	TCHAR *group = new TCHAR[255];

	_tcout << "Name: ";
	_tcin >> name;
	_tcout << "Surname: ";
	_tcin >> surname;
	_tcout << "Group: ";
	_tcin >> group;
	_tcout << endl;

	studentDao->create(Student(name, surname, group));

	_tcout << _T("Student has been added.") << endl << endl;

}

void performShowStudentsRoutine(StudentDao *studentDao) {

	vector<Student> students = studentDao->getAll();

	if (students.size() == 0) _tcout << "There is no students to show.";

	_tcout << endl;
	for (int i = 0; i < students.size(); i++) {
		students[i].show();
		_tcout << endl;
	}
	_tcout << endl;

}

void performComputeStatisticsRoutine(StudentDao *studentDao) {

	StatisticsService *service = new StatisticsService();
	service->computeAndSave(studentDao->getAll());

	_tcout << _T("Statistics computed and saved successfully") << endl << endl;

}

void performShowStatisticsRoutine(StudentDao *studentDao) {

	StatisticsService *service = new StatisticsService();
	vector<Statistic> statistics = service->getAll();

	if (statistics.size() == 0) _tcout << "There is no statistics available." << endl;

	for (int i = 0; i < statistics.size(); i++) statistics[i].show();

	_tcout << endl;

}




void showTopBanner() {

	_tcout << _T("================================================================") << endl;
	_tcout << _T("====") << endl;
	_tcout << _T("====  Simple student database ") << endl;
	_tcout << _T("====") << endl;
	_tcout << _T("================================================================") << endl << endl;

}

void showFileInfo(StudentDao *studentDao) {
	if (studentDao->fileWasCreated()) _tcout << _T("File has just been created.") << endl << endl;
	else {
		FileTimeInfo info = studentDao->getFileTimeInfo();

		_tcout << _T("File was created before.") << endl << endl;
		_tprintf(_TEXT("Creation time :      %02d:%02d:%02d\n"), info.creationTime.wHour, info.creationTime.wMinute, info.creationTime.wSecond);
		_tprintf(_TEXT("Last access time :   %02d:%02d:%02d\n"), info.lastAccessTime.wHour, info.lastAccessTime.wMinute, info.lastAccessTime.wSecond);
		_tprintf(_TEXT("Last write time :    %02d:%02d:%02d\n\n"), info.lastWriteTime.wHour, info.lastWriteTime.wMinute, info.lastWriteTime.wSecond);

	}
}

void showMenu() {
	_tcout << _T("======== Menu:") << endl;
	_tcout << _T("      1) Add student") << endl;
	_tcout << _T("      2) Show students") << endl;
	_tcout << _T("      3) Show statistics") << endl;
	_tcout << _T("      4) Compute statistics ") << endl;
	_tcout << _T("      5) Exit ") << endl << endl;
	_tcout << "Press key to chose menu option." << endl << endl;
}

void reinitializeScreen() {
	system("cls");
	showTopBanner();
	showMenu();
}