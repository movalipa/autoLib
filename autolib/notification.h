#pragma once
#include "cookie.h"
#include "console.h"
#include <sstream>

#define NotifCookiePrefix "notif:"
#define NotifErrorCookieName "notif:error"
#define NotifWarningCookieName "notif:warning"
#define NotifSuccessCookieName "notif:success"
#define NotifTextCookieName "notif:text"
#define NotifSeprator '|'

// three types of notifications: error, warning, success, text

void setNotif(string type, string value) {
	string name = NotifCookiePrefix + type;
	string existing = getCookie(name);
	if (existing != "")
		value = existing + NotifSeprator + value;

	setCookie(name, value);
}

void clearNotif() {
	unsetCookie(NotifErrorCookieName);
	unsetCookie(NotifWarningCookieName);
	unsetCookie(NotifSuccessCookieName);
	unsetCookie(NotifTextCookieName);
}

void showNotif(bool cleanNotif = true) {
	string error = getCookie(NotifErrorCookieName);
	string warning = getCookie(NotifWarningCookieName);
	string success = getCookie(NotifSuccessCookieName);
	string text = getCookie(NotifTextCookieName);

	cout << "\n";
	if (error != "")
	{
		stringstream ss(error);
		string token;
		while (getline(ss>>ws, token, NotifSeprator))
		{
			setConsoleColor(LIGHTRED);
			cout << token << "\n";
		}
	}
	if (warning != "")
	{
		stringstream ss(warning);
		string token;
		while (getline(ss >> ws, token, NotifSeprator))
		{
			setConsoleColor(BROWN);
			cout << token << "\n";
		}
	}
	if (success != "")
	{
		stringstream ss(success);
		string token;
		while (getline(ss >> ws, token, NotifSeprator))
		{
			setConsoleColor(LIGHTGREEN);
			cout << token << "\n";
		}
	}
	setConsoleColor(WHITE);
	if (text != "")
	{
		stringstream ss(text);
		string token;
		while (getline(ss >> ws, token, NotifSeprator))
		{
			setConsoleColor(WHITE);
			cout << token << "\n";
		}
	}
	cout << "\n";

	if (cleanNotif)
		clearNotif();
}
