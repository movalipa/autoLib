#pragma once
#include <iostream>
#include <sstream>
#include "file.h"
#define CookiesDatabaseFile "cookies.csv"
using namespace std;

struct Cookie
{
	string name;
	string value;
};

int getAllCookies(Cookie cookies[]) {
	string lines[MaxFileLines];
	int n = readFile(CookiesDatabaseFile, lines);

	for (int i = 0; i < n - 1; i++)
	{
		// making string stream out of line to then be seprated by coma
		stringstream ss(lines[i + 1]);// plus one to skip header line
		string token;

		getline(ss >> ws, token, ',');
		cookies[i].name = token;

		getline(ss >> ws, token, ',');
		cookies[i].value = token;
	}
	// return count (-1 to skipp header)
	return n-1;
}

string getCookie(string name) {
	Cookie cookies[MaxFileLines];
	int n = getAllCookies(cookies);
	for (int i = 0; i < n; i++)
		if (name == cookies[i].name)
			return cookies[i].value;

	return "";
}

void overWriteCookies(Cookie cookies[], int n) {
	string headerLine = "name,value";
	string lines[MaxFileLines];
	lines[0] = headerLine;
	int j = 1;

	for (int i = 0; i < n; i++)
	{
		// reject empty cookies
		if (cookies[i].name == "")
			continue;

		lines[j] = cookies[i].name + "," + cookies[i].value;

		j++;
	}
	writeFileSecure(CookiesDatabaseFile, lines, j);
}

void setCookie(string name, string value) {
	Cookie cookies[MaxFileLines];
	int n = getAllCookies(cookies);
	for (int i = 0; i < n; i++)
		if (name == cookies[i].name)
		{
			cookies[i].value = value;
			overWriteCookies(cookies, n);
			return;
		}
	cookies[n].name = name;
	cookies[n].value = value;
	overWriteCookies(cookies, n + 1);
}

void unsetCookie(string name) {
	Cookie cookies[MaxFileLines];
	int n = getAllCookies(cookies);
	for (int i = 0; i < n; i++)
		if (name == cookies[i].name)
		{
			cookies[i].name = "";
			overWriteCookies(cookies, n);
			return;
		}
}

// be care full with this (never use)
void clearAllCookies() {
	Cookie cookies[MaxFileLines];
	int n = getAllCookies(cookies);
	for (int i = 0; i < n; i++)
			cookies[i].name = "";
	overWriteCookies(cookies, n);
}