#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include "cookie.h"
#include "file.h"
#include "notification.h"

#define UsersDatabaseFile "users.csv"
#define loggedUserCookieName "loggedUser"
#define lastUserIDCookieName "lastUserID"

using namespace std;

struct User {
	unsigned int ID = 0;
	unsigned long long studentID = 0;
	unsigned long long phoneNumber = 0;
	string name = "";
	string lastName = "";
	string password = "";
	int acceptedTerms = 0;
	int timestampAdded = 0;
};

int getAllUsers(User users[]) {
	string lines[MaxFileLines];
	int n = readFile(UsersDatabaseFile, lines);

	for (int i = 0; i < n-1; i++)
	{
		// making string stream out of line to then be seprated by coma
		stringstream ss(lines[i+1]);// plus one to skip header line
		string token;

		getline(ss >> ws, token, ',');
		users[i].ID = stoi(token);

		getline(ss >> ws, token, ',');
		users[i].studentID = stoll(token);

		getline(ss >> ws, token, ',');
		users[i].phoneNumber = stoll(token);

		getline(ss >> ws, token, ',');
		users[i].name = token;

		getline(ss >> ws, token, ',');
		users[i].lastName = token;

		getline(ss >> ws, token, ',');
		users[i].password = token;

		getline(ss >> ws, token, ',');
		users[i].acceptedTerms = stoi(token);

		getline(ss >> ws, token, ',');
		users[i].timestampAdded = stoi(token);
	}
	// return count (-1 to skipp header)
	return n-1;
}

int isSignedIn() {
	string loggedUser = getCookie(loggedUserCookieName);
	if (loggedUser == "")
		return 0;
	return stoi(loggedUser);
	// 0 -> not signed in
	// else -> returns user id
}

bool attempSignIn(User sample) {
	string loggedUser = getCookie(loggedUserCookieName);
	if (loggedUser != "")
	{
		setNotif("error", "Already signed in!");
		setNotif("warning", "if you want to change account, first sign out!");
		return false;
	}

	User users[MaxFileLines];
	int n = getAllUsers(users);
	for (int i = 0; i < n; i++)
	{
		if (sample.studentID == users[i].studentID &&
			sample.password == users[i].password)
		{
			setCookie("loggedUser", to_string(users[i].ID));
			setNotif("success", "Signed in successfully!");
			return true;
		}
	}
	setNotif("error", "Wrong credentials! Try again!!!");
	setNotif("warning", "If you are a new user try to sign up!");
	return false;
}

bool attempSignOut() {
	if (!isSignedIn())
	{
		setNotif("warning", "You are already signed out!");
		return false;
	}
	unsetCookie(loggedUserCookieName);
	setNotif("success", "Signed out Successfully!");
	return true;
}

User getUser(int id = isSignedIn()) {
	User users[MaxFileLines];
	int n = getAllUsers(users);
	for (int i = 0; i < n; i++)
		if (id == users[i].ID)
			return users[i];

	User empty;
	return empty;
}

void overWriteAllUsers(User users[], int n) {
	string headerLine = "ID,studentID,phoneNumber,name,lastName,password,acceptedTerms,timestampAdded";
	string lines[MaxFileLines];
	lines[0] = headerLine;
	int j = 1;

	for (int i = 0; i < n; i++)
	{
		// reject empty users
		if (users[i].ID == 0)
			continue;

		lines[j] = to_string(users[i].ID) + "," +
			to_string(users[i].studentID) + "," +
			to_string(users[i].phoneNumber) + "," +
			users[i].name + "," +
			users[i].lastName + "," +
			users[i].password + "," +
			to_string(users[i].acceptedTerms) + "," +
			to_string(users[i].timestampAdded);

		j++;
	}
	writeFileSecure(UsersDatabaseFile, lines, j);
}

bool checkUser(User user) {
	User users[MaxFileLines];
	int n = getAllUsers(users);
	for (int i = 0; i < n; i++)
	{
		// skip same user
		if (user.ID == users[i].ID)
			continue;

		if (user.studentID == users[i].studentID)
		{
			setNotif("error", "Duplicated student ID found!");
			setNotif("warning", "This student ID already exist.");
			return false;
		}
		if (user.phoneNumber == users[i].phoneNumber)
		{
			setNotif("error", "Duplicated phone number found!");
			setNotif("warning", "This phone number already exist.");
			return false;
		}
	}

	if (user.studentID > 4030000000 || user.studentID < 3950000000)
	{
		setNotif("error", "Invalid student ID!");
		setNotif("warning", "Valid range is from 3950000000 to 4030000000");
		return false;
	}
	if (user.phoneNumber < 9000000000 || user.phoneNumber > 9999999999)
	{
		setNotif("error", "Invalid phone number!");
		setNotif("warning", "Valid range is from 9000000000 to 9999999999 and don\'t use country code");
		return false;
	}
	if (user.name.length() < 3 || user.name.length() > 16 || user.name != user.name.substr(0, user.name.find(',')))
	{
		setNotif("error", "Invalid name!");
		setNotif("warning", "Valid name must be 3-16 characters and containt no commas");
		return false;
	}
	if (user.lastName.length() < 3 || user.lastName.length() > 16 || user.lastName != user.lastName.substr(0, user.lastName.find(',')))
	{
		setNotif("error", "Invalid last name!");
		setNotif("warning", "Valid last name must be 3-16 characters and containt no commas");
		return false;
	}
	if (user.password.length() < 8)
	{
		setNotif("error", "Invalid password!");
		setNotif("warning", "Valid password must be at least 8 characters and containt no commas");
		return false;
	}
	if (user.acceptedTerms < 0 || user.acceptedTerms > 1)
	{
		setNotif("error", "Invalid acceptedTerms!");
		setNotif("warning", "Valid acceptedTerms must be either 1 or 0");
		return false;
	}
	if (user.timestampAdded == 0)
	{
		setNotif("error", "Invalid timestampAdded!");
		setNotif("warning", "Valid timestampAdded must not be 0");
		return false;
	}
	return true;
}

bool updateUser(User user) {
	if (!checkUser(user))
		return false;

	user.name[0] = toupper(user.name[0]);
	user.lastName[0] = toupper(user.lastName[0]);

	User users[MaxFileLines];
	int n = getAllUsers(users);
	for (int i = 0; i < n; i++)
	{
		if (users[i].ID == user.ID)
		{
			users[i].studentID = user.studentID;
			users[i].password = user.password;
			users[i].phoneNumber = user.phoneNumber;
			users[i].name = user.name;
			users[i].lastName = user.lastName;
			users[i].acceptedTerms = user.acceptedTerms;
			users[i].timestampAdded = user.timestampAdded;
			overWriteAllUsers(users, n);
			setNotif("success", "User updated successfully!");
			return true;
		}
	}

	int lastID = stoll(getCookie(lastUserIDCookieName));
	lastID++;
	setCookie(lastUserIDCookieName, to_string(lastID));

	users[n].ID = lastID;
	users[n].studentID = user.studentID;
	users[n].password = user.password;
	users[n].phoneNumber = user.phoneNumber;
	users[n].name = user.name;
	users[n].lastName = user.lastName;
	users[n].acceptedTerms = user.acceptedTerms;
	users[n].timestampAdded = user.timestampAdded;

	overWriteAllUsers(users, n+1);

	setNotif("success", "User added successfully!");
	return true;
}

void addUser(User user, bool signInAfter) {
	user.timestampAdded = time(0);
	bool isAdded = updateUser(user);

	if (signInAfter && isAdded)
		attempSignIn(user);
}

bool hasAcceptedTerms(int userID = isSignedIn()) {
	User user = getUser(userID);
	if (user.acceptedTerms == 1)
		return true;
	return false;
}
