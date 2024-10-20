#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include "user.h"
#include "cookie.h"
#include "file.h"
#include "notification.h"

#define BooksDatabaseFile "books.csv"
#define lastBookIDCookieName "lastBookID"

using namespace std;

struct Book {
	unsigned int ID = 0;
	unsigned int initOwnerUserID = 0;
	unsigned int ownerUserID = 0;
	string title = "";
	string author = "";
	int physicalCondition = 0;
	int statusCode = 0;
	string description = "";
	unsigned int timestampAdded = 0;
};

// 1 -> availabe to req
// 0 -> open req
// -1 -> unavailabel to req

int getAllBooks(Book books[]) {
	string lines[MaxFileLines];
	int n = readFile(BooksDatabaseFile, lines);

	for (int i = 0; i < n - 1; i++)
	{
		// making string stream out of line to then be seprated by coma
		stringstream ss(lines[i + 1]);// plus one to skip header line
		string token;

		getline(ss >> ws, token, ',');
		books[i].ID = stoi(token);

		getline(ss >> ws, token, ',');
		books[i].initOwnerUserID = stoi(token);

		getline(ss >> ws, token, ',');
		books[i].ownerUserID = stoi(token);

		getline(ss >> ws, token, ',');
		books[i].title = token;

		getline(ss >> ws, token, ',');
		books[i].author = token;

		getline(ss >> ws, token, ',');
		books[i].physicalCondition = stoi(token);

		getline(ss >> ws, token, ',');
		books[i].statusCode = stoi(token);

		getline(ss >> ws, token, ',');
		books[i].description = token;

		getline(ss >> ws, token, ',');
		books[i].timestampAdded = stoi(token);
	}
	// return count (-1 to skipp header)
	return n-1;
}

Book getBook(int id) {
	Book books[MaxFileLines];
	int n = getAllBooks(books);
	for (int i = 0; i < n; i++)
		if (id == books[i].ID)
			return books[i];

	Book empty;
	return empty;
}

int searchBooks(Book found[],Book lookFor) {
	Book books[MaxFileLines];
	int j = 0;
	int n = getAllBooks(books);
	for (int i = 0; i < n; i++)
	{
		if ((lookFor.ID == 0 || lookFor.ID == books[i].ID) &&
			(lookFor.initOwnerUserID == 0 || lookFor.initOwnerUserID == books[i].initOwnerUserID) &&
			(lookFor.ownerUserID == 0 || lookFor.ownerUserID == books[i].ownerUserID) &&
			(lookFor.title == "" || books[i].title.length() > to_lower(books[i].title).find(to_lower(lookFor.title))) &&
			(lookFor.author == "" || books[i].author.length() > to_lower(books[i].author).find(to_lower(lookFor.author))) &&
			(lookFor.physicalCondition == 0 || lookFor.physicalCondition == books[i].physicalCondition) &&
			(lookFor.statusCode == 0 || lookFor.statusCode == books[i].statusCode) &&
			(lookFor.description == "" || books[i].description.length() > to_lower(books[i].description).find(to_lower(lookFor.description))) &&
			(lookFor.timestampAdded == 0 || lookFor.timestampAdded == books[i].timestampAdded))
		{
			found[j] = books[i];
			j++;
		}
	}
	return j;
}

void overWriteAllBooks(Book books[], int n) {
	string headerLine = "ID,initOwnerUserID,ownerUserID,title,author,physicalCondition,statusCode,description,timestampAdded";
	string lines[MaxFileLines];
	lines[0] = headerLine;
	int j = 1;
	for (int i = 0; i < n; i++)
	{
		// reject empty books
		if (books[i].ID == 0)
			continue;

		lines[j] = to_string(books[i].ID) + "," +
			to_string(books[i].initOwnerUserID) + "," +
			to_string(books[i].ownerUserID) + "," +
			books[i].title + "," +
			books[i].author + "," +
			to_string(books[i].physicalCondition) + "," +
			to_string(books[i].statusCode) + "," +
			books[i].description + "," +
			to_string(books[i].timestampAdded);

		j++;

	}
	writeFileSecure(BooksDatabaseFile, lines, j);
}

bool checkBook(Book book) {
	// no need to check ID, initOwnerUserID, ownerUserID
	if (book.title.length() < 3 || book.title.length() > 32 || book.title != book.title.substr(0, book.title.find(',')))
	{
		setNotif("error", "Invalid title!");
		setNotif("warning", "Valid title must be 3-32 characters and containt no commas");
		return false;
	}
	if (book.author.length() < 3 || book.author.length() > 32 || book.author != book.author.substr(0, book.author.find(',')))
	{
		setNotif("error", "Invalid author!");
		setNotif("warning", "Valid author must be 3-32 characters and containt no commas");
		return false;
	}
	if (book.physicalCondition < 1 || book.physicalCondition > 5)
	{
		setNotif("error", "Invalid condition!");
		setNotif("warning", "Valid condition must be between 1 to 5");
		return false;
	}
	if (book.statusCode > 1 || book.statusCode < -1 || book.statusCode == 0)
	{
		setNotif("error", "Invalid status!");
		setNotif("warning", "Valid status can be eaither 1 or -1");
		return false;
	}
	if (book.description.length() > 32)
	{
		setNotif("error", "Invalid description!");
		setNotif("warning", "Valid description must be less that 32 characters and containt no commas");
		return false;
	}
	if (book.timestampAdded == 0)
	{
		setNotif("error", "Invalid timestampAdded!");
		setNotif("warning", "Valid timestampAdded must not be 0");
		return false;
	}
	return true;
}

bool updateBook(Book book) {
	if (!checkBook(book))
		return false;

	book.title[0] = toupper(book.title[0]);
	book.author[0] = toupper(book.author[0]);
	book.description[0] = toupper(book.description[0]);

	if (book.description == "0")
		book.description = "--------------------------------";

	Book books[MaxFileLines];
	int n = getAllBooks(books);
	for (int i = 0; i < n; i++)
	{
		if (books[i].ID == book.ID)
		{
			books[i].initOwnerUserID = book.initOwnerUserID;
			books[i].ownerUserID = book.ownerUserID;
			books[i].title = book.title;
			books[i].author = book.author;
			books[i].physicalCondition = book.physicalCondition;
			books[i].statusCode = book.statusCode;
			books[i].description = book.description;
			books[i].timestampAdded = book.timestampAdded;
			overWriteAllBooks(books, n);
			setNotif("success", "Book updated successfully!");
			return true;
		}
	}

	int lastID = stoll(getCookie(lastBookIDCookieName));
	lastID++;
	setCookie(lastBookIDCookieName, to_string(lastID));

	books[n].ID = lastID;
	books[n].initOwnerUserID = book.initOwnerUserID;
	books[n].ownerUserID = book.ownerUserID;
	books[n].title = book.title;
	books[n].author = book.author;
	books[n].physicalCondition = book.physicalCondition;
	books[n].statusCode = book.statusCode;
	books[n].description = book.description;
	books[n].timestampAdded = book.timestampAdded;

	overWriteAllBooks(books, n + 1);

	setNotif("success", "Book added successfully!");
	return true;
}

void addBook(Book book) {
	book.initOwnerUserID = isSignedIn();
	book.ownerUserID = isSignedIn();
	book.statusCode = 1;
	book.timestampAdded = time(0);
	updateBook(book);
}

string trasnlateBookCondiotion(int condition) {
	switch (condition)
	{
	case 1:
		return "Very bad!(1)";
		break;
	case 2:
		return "Bad!(2)";
		break;
	case 3:
		return "Not bad!(3)";
		break;
	case 4:
		return "Good!(4)";
		break;
	case 5:
		return "Very good!(5)";
		break;
	default:
		return to_string(condition);
		break;
	}
}

string trasnlateBookStatus(int status) {
	switch (status)
	{
	case -1:
		return "Unvailable!(-1)";
		break;
	case 1:
		return "Available!(1)";
		break;
	default:
		return to_string(status);
		break;
	}
}

int mergeBooks(Book book1[], Book book2[], Book result[]) {
	int i = 0, j = 0, z = 0;
	while (book1[i].ID != 0 && book2[j].ID != 0)
	{
		if (book1[i].ID == book2[j].ID)
		{
			result[z++] = book1[i++];
			j++;
		}
		else if (book1[i].ID < book2[j].ID)
			result[z++] = book1[i++];
		else
			result[z++] = book2[j++];

	}
	while (book1[i].ID != 0)
		result[z++] = book1[i++];
	
	while (book2[j].ID != 0)
		result[z++] = book2[j++];
	
	return z;
}