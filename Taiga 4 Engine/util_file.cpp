
#include "main.h"
#include "util.h"

void cUtil::copyFile(string input, string output)
{
	wifstream in;
	in.open(input, ios::binary);
	if (!in.good()) { console.error << "[cUtil::copyFile] Unable to open the file [" + input + "]!" << endl; return; }

	wofstream out;
	out.open(output, ios::binary);
	if (!out.good()) { console.error << "[cUtil::copyFile] Unable to create the file [" + output + "]!" << endl; return; }

	in.seekg(0, ios::end);
	ifstream::pos_type size = in.tellg();
	in.seekg(0);
	wchar_t* buffer = new wchar_t[size];

	in.read(buffer, size);
	out.write(buffer, size);
}

void cUtil::copyFolder(string input, string output)
{
	bool search;
	string inpath = input;
	string outpath = output;
	CreateDirectoryA(outpath.c_str(), 0);
	string filepath = inpath + "*";
	WIN32_FIND_DATAA dataStruct;
	HANDLE hFile = FindFirstFileA(filepath.c_str(), &dataStruct);
	do
	{
		if (strlen(dataStruct.cFileName) > 2) {
			copyFile(inpath + dataStruct.cFileName, outpath + dataStruct.cFileName);
		}
	} while (FindNextFileA(hFile, &dataStruct));
}

void cUtil::clearFolder(string input)
{
	bool search;
	string inpath = input;
	string filepath = inpath + "*";
	string target;
	WIN32_FIND_DATAA dataStruct;
	HANDLE hFile = FindFirstFileA(filepath.c_str(), &dataStruct);
	do
	{
		if (strlen(dataStruct.cFileName) > 2) {
			target = inpath + dataStruct.cFileName;
			DeleteFileA(target.c_str());
		}
	} while (FindNextFileA(hFile, &dataStruct));
}