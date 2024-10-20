#pragma once
#include <iostream>
#include <fstream>
#include <string>

#define MaxFileLines 100

using namespace std;

// Puts comma between array elements and output it as single string (DISABLED FOR NOW)
string commaLiner(string columns[], int n) {
	string output = "";
	for (int i = 0; i < n; i++)
	{
		output += columns[i];
		if (i < n-1)
		{
			output += ",";
		}
	}
	return output;
}

int readFile(string path, string data[]) {
	ifstream f(path);
	if (!f)
	{
		cout << "Unable to open file";
		exit(-1);
	}
	int i = 0;
	// getting each line fully at first
	while (getline(f >> ws, data[i]))
		i++;

	f.close();
	// for future
	if (i > MaxFileLines)
	{
		cout << "ERROR: max count overflowed, please contact sys_admin";
		exit(-1);
	}
	// return count
	return i;
}

void writeFile(string path, string data[], int n) {
	ofstream f(path);
	if (!f)
	{
		cout << "Unable to open file";
		exit(-1);
	}
	for (int i = 0; i < n; i++)
		f << data[i] << "\n";

	f.close();
}

void copyFile(string path1, string path2) {
	string lines[MaxFileLines];
	int n = readFile(path1, lines);
	writeFile(path2, lines, n);
}

void backupFile(string path) {
	string backupPath = path + ".bac";
	copyFile(path, backupPath);
}

// ...Secure : ensures using backup before anything
// read secure : doesnt do much (no need to use)
// write secure : recommanded to use for safety reasons

int readFileSecure(string path, string data[]) {
	backupFile(path);
	return readFile(path, data);
}

void writeFileSecure(string path, string data[], int n) {
	backupFile(path);
	writeFile(path, data, n);
}

// utils

string to_lower(string text) {
	for (int i = 0; i < text.length(); i++)
		text[i] = tolower(text[i]);
	return text;
}

string to_upper(string text) {
	for (int i = 0; i < text.length(); i++)
		text[i] = toupper(text[i]);
	return text;
}