#include<iostream>
#include<Windows.h>
#include<tchar.h>
#include <stdio.h>
#include<string>

using namespace std;

TCHAR* toRegTypeString(DWORD type);

struct RegValue {
	TCHAR name[255];
	BYTE data[255];
	DWORD type;

	void show() {
		wcout << "Name: " << name << endl;
		wcout << "Data: " << data << endl;
		wcout << "Type: " << toRegTypeString(type) << endl << endl;
	}
};

LPTSTR* getSubkeyNames(PHKEY, int&);
RegValue* getRegValues(PHKEY, int&);

void showCurrentVariables(HKEY&);
void showNames(LPTSTR*, int);

/*******************************************************************/
/********** RegEdit *********/

int main() {

	cout << "Chose Hive Key:" << endl << endl;
	cout << "1 - HKEY_CLASSES_ROOT" << endl;
	cout << "2 - HKEY_CURRENT_USER" << endl;
	cout << "3 - HKEY_LOCAL_MACHINE" << endl;
	cout << "4 - HKEY_USERS" << endl;
	cout << "5 - HKEY_CURRENT_CONFIG" << endl << endl;

	cout << "Type 'var' to show current key variables." << endl;
	cout << "Type 'exit' to exit." << endl;
	cout << "Type matching number to navigate." << endl << endl;

	string input;

	cout << "Navigate to: ";
	cin >> input;
	cout << endl;
	int key = atoi(input.c_str());
	if (key < 1 || key >5) exit(0);

	HKEY hKey;

	switch (key) {
	case 1: hKey = HKEY_CLASSES_ROOT;   break;
	case 2: hKey = HKEY_CURRENT_USER;	  break;
	case 3: hKey = HKEY_LOCAL_MACHINE;  break;
	case 4: hKey = HKEY_USERS;		  break;
	case 5: hKey = HKEY_CURRENT_CONFIG; break;
	}

	for (;;) {

		int countDisplayed;
		LPTSTR *names = getSubkeyNames(&hKey, countDisplayed);
		showNames(names, countDisplayed);

		cout << "Navigate to: ";
		cin >> input;

		if (input.compare("var") == 0)  showCurrentVariables(hKey);
		if (input.compare("exit") == 0)  exit(0);
		else {
			int pressedNumber = atoi(input.c_str());
			if ((pressedNumber > 0) && (pressedNumber <= countDisplayed))
			{
				HKEY resultKey;
				int code = RegOpenKeyEx(hKey, names[pressedNumber - 1], 0, KEY_READ, &resultKey);
				hKey = resultKey;
			}
		}

	}
	return 0;
}

/*******************************************************************/

LPTSTR* getSubkeyNames(PHKEY hKey, int &size) {

	DWORD count, maxNameLength;
	RegQueryInfoKey(*hKey, NULL, NULL, NULL, &count, &maxNameLength, NULL, NULL, NULL, NULL, NULL, NULL);

	DWORD bufferSize;
	TCHAR ** subkeyNames = new LPTSTR[count];

	for (int i = 0; i < count; i++)
	{
		bufferSize = maxNameLength + 1;
		TCHAR *subkeyName = new TCHAR[maxNameLength];
		int code = RegEnumKeyEx(*hKey, i, subkeyName, &bufferSize, NULL, NULL, NULL, NULL);
		subkeyNames[i] = subkeyName;
	}
	size = count;
	return subkeyNames;
}

RegValue* getRegValues(PHKEY hKey, int &size) {

	DWORD count, maxNameLength, maxDataLength;
	RegQueryInfoKey(*hKey, NULL, NULL, NULL, NULL, NULL, NULL, &count, &maxNameLength, &maxDataLength, NULL, NULL);

	DWORD nameBufferSize, dataBufferSize;
	RegValue *regValues = new RegValue[count];

	for (int i = 0; i < count; i++)
	{
		nameBufferSize = maxNameLength + 1;
		dataBufferSize = maxDataLength + 1;
		RegValue v;
		int code = RegEnumValue(*hKey, i, v.name, &nameBufferSize, NULL, &v.type, v.data, &dataBufferSize);
		regValues[i] = v;
	}
	size = count;
	return regValues;
}


void showCurrentVariables(HKEY &hKey) {

	cout << endl << "Variables:" << endl;

	int size;
	RegValue* variables = getRegValues(&hKey, size);

	if (size == 0) cout << "No variables to show." << endl << endl;
	for (int i = 0; i < size; i++) variables[i].show();
}

void showNames(LPTSTR *names, int size) {
	for (int i = 0; i < size; i++) wcout << i + 1 << " - " << names[i] << endl;
	cout << endl;
}

TCHAR* toRegTypeString(DWORD type) {
	switch (type)
	{
	case REG_BINARY: return _T("REG_BINARY");
	case REG_DWORD: return _T("REG_DWORD");
	case REG_DWORD_BIG_ENDIAN: return _T("REG_DWORD_BIG_ENDIAN");
	case REG_EXPAND_SZ: return _T("REG_EXPAND_SZ");
	case REG_LINK: return _T("REG_LINK");
	case REG_MULTI_SZ: return _T("REG_MULTI_SZ");
	case REG_NONE: return _T("REG_NONE");
	case REG_QWORD: return _T("REG_QWORD");
	case REG_SZ: return _T("REG_SZ");

	default:
		break;
	}
	return _T("NOT_PROCESSED");

}