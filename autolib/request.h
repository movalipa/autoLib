#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include "cookie.h"
#include "file.h"
#include "user.h"
#include "book.h"
#include "notification.h"

#define RequestsDatabaseFile "requests.csv"
#define lastRequestIDCookieName "lastRequestID"

using namespace std;

struct Request {
	unsigned int ID = 0;
	unsigned int fromUserID = 0;
	unsigned int toUserID = 0;
	unsigned int bookID = 0;
	int statusCode = 0;
	unsigned int timestampSubmited = 0;
	unsigned int timestampRecieved = 0;
	unsigned int timestampReturned = 0;
};
// 1 -> accepted
// 0 -> requested
// -1 -> declined

int getAllRequests(Request requests[]) {
	string lines[MaxFileLines];
	int n = readFile(RequestsDatabaseFile, lines);

	for (int i = 0; i < n - 1; i++)
	{
		// making string stream out of line to then be seprated by coma
		stringstream ss(lines[i + 1]);// plus one to skip header line
		string token;

		getline(ss >> ws, token, ',');
		requests[i].ID = stoi(token);

		getline(ss >> ws, token, ',');
		requests[i].fromUserID = stoi(token);

		getline(ss >> ws, token, ',');
		requests[i].toUserID = stoi(token);

		getline(ss >> ws, token, ',');
		requests[i].bookID = stoi(token);

		getline(ss >> ws, token, ',');
		requests[i].statusCode = stoi(token);

		getline(ss >> ws, token, ',');
		requests[i].timestampSubmited = stoi(token);
		getline(ss >> ws, token, ',');
		requests[i].timestampRecieved = stoi(token);

		getline(ss >> ws, token, ',');
		requests[i].timestampReturned = stoi(token);

	}
	// return count (-1 to skipp header)
	return n - 1;
}

Request getRequest(int id) {
	Request requests[MaxFileLines];
	int n = getAllRequests(requests);
	for (int i = 0; i < n; i++)
		if (id == requests[i].ID)
			return requests[i];

	Request empty;
	return empty;
}

int searchRequests(Request found[], Request lookFor) {
	Request requests[MaxFileLines];
	int j = 0;
	int n = getAllRequests(requests);
	for (int i = 0; i < n; i++)
	{
		if ((lookFor.ID == 0 || lookFor.ID == requests[i].ID) &&
			(lookFor.fromUserID == 0 || lookFor.fromUserID == requests[i].fromUserID) &&
			(lookFor.toUserID == 0 || lookFor.fromUserID == requests[i].fromUserID) &&
			(lookFor.bookID == 0 || lookFor.bookID == requests[i].bookID) &&
			(lookFor.statusCode == 0 || lookFor.statusCode == requests[i].statusCode) &&
			(lookFor.timestampSubmited == 0 || lookFor.timestampSubmited == requests[i].timestampSubmited) &&
			(lookFor.timestampRecieved == 0 || lookFor.timestampRecieved == requests[i].timestampRecieved) &&
			(lookFor.timestampReturned == 0 || lookFor.timestampReturned == requests[i].timestampReturned))
		{
			found[j] = requests[i];
			j++;
		}
	}
	return j;
}

void overWriteAllRequests(Request requests[], int n) {
	string headerLine = "ID,fromUserID,toUserID,bookID,statusCode,timestampSubmited,timestampRecieved,timestampReturned";
	string lines[MaxFileLines];
	lines[0] = headerLine;
	int j = 1;
	for (int i = 0; i < n; i++)
	{
		// reject empty books
		if (requests[i].ID == 0)
			continue;

		lines[j] = to_string(requests[i].ID) + "," +
			to_string(requests[i].fromUserID) + "," +
			to_string(requests[i].toUserID) + "," +
			to_string(requests[i].bookID) + "," +
			to_string(requests[i].statusCode) + "," +
			to_string(requests[i].timestampSubmited) + "," +
			to_string(requests[i].timestampRecieved) + "," +
			to_string(requests[i].timestampReturned);

		j++;

	}
	writeFileSecure(RequestsDatabaseFile, lines, j);
}

bool checkRequest(Request request) {
	if (request.fromUserID == 0)
	{
		setNotif("error", "Invalid fromUserID!");
		setNotif("warning", "Valid fromUserID must not be 0");
		return false;
	}
	if (request.toUserID == 0)
	{
		setNotif("error", "Invalid toUserID!");
		setNotif("warning", "Valid toUserID must not be 0");
		return false;
	}
	if (request.bookID == 0)
	{
		setNotif("error", "Invalid bookID!");
		setNotif("warning", "Valid bookID must not be 0");
		return false;
	}
	if (request.statusCode > 1 || request.statusCode < -1)
	{
		setNotif("error", "Invalid statusCode!");
		setNotif("warning", "Valid statusCode must either 1, 0, -1");
		return false;
	}
	if (request.timestampSubmited == 0)
	{
		setNotif("error", "Invalid timestampSubmited!");
		setNotif("warning", "Valid timestampSubmited must not be 0");
		return false;
	}
	// two other timestamp dont need check
	
	return true;
}

bool updateRequest(Request request) {
	if (!checkRequest(request))
		return false;
	Request requests[MaxFileLines];
	int n = getAllRequests(requests);
	for (int i = 0; i < n; i++)
	{
		if (requests[i].ID == request.ID)
		{
			requests[i].fromUserID = request.fromUserID;
			requests[i].toUserID = request.toUserID;
			requests[i].bookID = request.bookID;
			requests[i].statusCode = request.statusCode;
			requests[i].timestampSubmited = request.timestampSubmited;
			requests[i].timestampRecieved = request.timestampRecieved;
			requests[i].timestampReturned = request.timestampReturned;
			overWriteAllRequests(requests, n);
			setNotif("success", "Request updated successfully!");
			return true;
		}
	}

	int lastID = stoll(getCookie(lastRequestIDCookieName));
	lastID++;
	setCookie(lastRequestIDCookieName, to_string(lastID));

	requests[n].ID = lastID;
	requests[n].fromUserID = request.fromUserID;
	requests[n].toUserID = request.toUserID;
	requests[n].bookID = request.bookID;
	requests[n].statusCode = request.statusCode;
	requests[n].timestampSubmited = request.timestampSubmited;
	requests[n].timestampRecieved = request.timestampRecieved;
	requests[n].timestampReturned = request.timestampReturned;

	overWriteAllRequests(requests, n + 1);

	setNotif("success", "Request added successfully!");
	return true;
}

void addRequest(Request request) {
	request.timestampSubmited = time(0);
	updateRequest(request);
}

string timestamp2string(int timestamp) {
	if (timestamp == 0)
		return "Not Yet";

	time_t a = timestamp;
	string b = ctime(&a);
	return b.substr(0,(b.find('\n')));// to remove last \n from ctime
}

string translateRequestStatus(int status) {
	switch (status)
	{
	case 1:
		return "Accepted(1)";
		break;
	case 0:
		return "Pending(0)";
		break;
	case -1:
		return "Rejected(-1)";
		break;
	default:
		return to_string(status);
		break;
	}
}

// user related

int getUserRequests(Request found[], int userID = isSignedIn()) {
	Request requests[MaxFileLines];
	int n = getAllRequests(requests);
	int j = 0;
	for (int i = 0; i < n; i++)
		if (requests[i].toUserID == userID || requests[i].fromUserID == userID)
		{
			found[j] = requests[i];
			j++;
		}
	return j;
}

int getUserSentRequests(Request found[], int userID) {
	Request requests[MaxFileLines];
	int n = getAllRequests(requests);
	int j = 0;
	for (int i = 0; i < n; i++)
		if (requests[i].toUserID == userID)
		{
			found[j] = requests[i];
			j++;
		}
	return j;
}

int getUserReceivedRequests(Request found[], int userID) {
	Request requests[MaxFileLines];
	int n = getAllRequests(requests);
	int j = 0;
	for (int i = 0; i < n; i++)
		if (requests[i].fromUserID == userID)
		{
			found[j] = requests[i];
			j++;
		}
	return j;
}

bool isBookRequestedByUser(int bookID) {
	Request found[MaxFileLines];
	int n = getUserSentRequests(found, isSignedIn());
	for (int i = 0; i < n; i++)
		if (found[i].statusCode == 0 && found[i].bookID == bookID)
			return true;

	return false;
}

float getUserScore(int& rate, int userID = isSignedIn()) {
	Request requests[MaxFileLines];
	int n = getAllRequests(requests);
	float total = 0;
	int now = time(0);
	for (int i = 0; i < n; i++)
	{
		if (requests[i].statusCode == 1 && (requests[i].fromUserID == userID || requests[i].toUserID == userID))
		{
			int sign, days = 0;
			if (requests[i].fromUserID == userID)
				sign = 1;
			else /* if (requests[i].toUserID == userID)*/
				sign = -1;

			if (requests[i].timestampReturned == 0) // not returned yet
				days = now - requests[i].timestampRecieved;
			else // returned
				days = requests[i].timestampReturned - requests[i].timestampRecieved;
			rate += sign;
			total += sign * days;
		}
	}
	return total / 3600 / 24 + 10;// +10 for initial score
}

void sortUsersByScore(User users[],int n) {
	for (int i = 0; i < n-1; i++)
	{
		User tmp;
		int rate = 0;
		for (int j = i+1; j < n; j++)
		{
			if (getUserScore(rate, users[i].ID) < getUserScore(rate, users[j].ID))
			{
				tmp = users[i];
				users[i] = users[j];
				users[j] = tmp;
			}
		}
	}
}

// book related

int getBookRequests(Request found[], int bookID) {
	Request requests[MaxFileLines];
	int n = getAllRequests(requests);
	int j = 0;
	for (int i = 0; i < n; i++)
		if (requests[i].bookID == bookID)
		{
			found[j] = requests[i];
			j++;
		}
	return j;
}

int getBookOpenRequests(Request found[], int bookID) {
	Request requests[MaxFileLines];
	int n = getAllRequests(requests);
	int j = 0;
	for (int i = 0; i < n; i++)
		if (requests[i].bookID == bookID && requests[i].statusCode == 0)
		{
			found[j] = requests[i];
			j++;
		}
	return j;
}

int getBookOpenRequestCount(int bookID) {
	Request requests[MaxFileLines];
	return getBookOpenRequests(requests, bookID);
}

bool checkBookRequest(int bookID) {
	if (getBook(bookID).ID == 0)
	{
		setNotif("error", "Action blocked!");
		setNotif("warning", "The book doesn't exist!");
		return false;
	}
	else if (getBook(bookID).ownerUserID == isSignedIn())
	{
		setNotif("error", "Action blocked!");
		setNotif("warning", "This book is already yours!");
		return false;
	}
	if (isBookRequestedByUser(bookID))
	{
		setNotif("error", "Action blocked!");
		setNotif("warning", "You have already requested for this book!");
		return false;
	}
	if (getBook(bookID).statusCode == -1)
	{
		setNotif("error", "Action blocked!");
		setNotif("warning", "This book is already in use of another user!");
		return false;
	}
	int rate = 0;
	if (getUserScore(rate) <= 0)
	{
		setNotif("error", "Action blocked!");
		setNotif("warning", "insufficient score to request for a book!");
		return false;
	}
	return true;
}

bool attempRequestBook(int bookID) {
	if (!checkBookRequest(bookID))
		return false;

	Request request;
	request.fromUserID = getBook(bookID).ownerUserID;
	request.toUserID = isSignedIn();
	request.bookID = bookID;
	addRequest(request);
	return true;
}

bool attempAcceptBookRequest(int bookID, int reqID) {
	Request request = getRequest(reqID);
	if (request.fromUserID != isSignedIn())
	{
		setNotif("error", "No access!");

		if (request.toUserID == isSignedIn())
			setNotif("warning", "You can't accept/reject your own request!");
		
		return false;
	}

	Request requests[MaxFileLines];
	int n = getAllRequests(requests);
	int now = time(0);
	bool couldFind = false;
	for (int i = 0; i < n; i++)
	{
		if (requests[i].statusCode == 1 && requests[i].bookID == bookID &&
			requests[i].timestampRecieved != 0 && requests[i].timestampReturned == 0)
		{
			requests[i].timestampReturned = now;
		}
		else if (requests[i].statusCode == 0 && requests[i].bookID == bookID)
		{
			if (requests[i].ID == reqID)
			{
				requests[i].timestampRecieved = now;
				requests[i].statusCode = 1;
				couldFind = true;
			}
			else
				requests[i].statusCode = -1;
		}
	}
	if (!couldFind)
	{
		setNotif("error", "Request doen't exist!");
		return false;
	}
	overWriteAllRequests(requests, n);
	Book book = getBook(bookID);
	book.ownerUserID = getRequest(reqID).toUserID;
	book.statusCode = -1;
	updateBook(book);
}

bool attempRejectBookRequest(int reqID) {
	Request request = getRequest(reqID);
	if (request.fromUserID != isSignedIn())
	{
		setNotif("error", "No access!");

		if (request.toUserID == isSignedIn())
			setNotif("warning", "You can't accept/reject your own request!");

		return false;
	}

	Request requests[MaxFileLines];
	int n = getAllRequests(requests);
	bool couldFind = false;
	for (int i = 0; i < n; i++)
	{
		if (requests[i].ID == reqID && requests[i].statusCode == 0)
		{
			requests[i].statusCode = -1;
			couldFind = true;
			break;
		}
	}
	if (!couldFind)
	{
		setNotif("error", "Request doen't exist!");
		return false;
	}
	overWriteAllRequests(requests, n);
}