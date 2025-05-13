#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "console.h"
#include "user.h"
#include "book.h"
#include "request.h"
#include "cookie.h"
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

using namespace std;

// todo
// 15. ensure needed cookies are availabel even ifff

namespace intro {
	void header(string page = "Home") {
		clearConsole();

		while (!ensureConsoleSize())
		{
			string toShow = "Your window size doesn't match our requirements";
			int xPos = (getConsoleColumns() - toShow.length()) / 2;
			int yPos = (getConsoleRows()) / 2 - 2;
			gotoxy(xPos, yPos);
			cout << toShow;

			toShow = "Expand window little bit or zoom out";
			xPos = (getConsoleColumns() - toShow.length()) / 2;
			yPos = (getConsoleRows()) / 2 - 1;
			gotoxy(xPos, yPos);
			setConsoleColor(LIGHTCYAN);
			cout << toShow;
			setConsoleColor(WHITE);

			toShow = "Must be at least 130 X 32";
			xPos = (getConsoleColumns() - toShow.length()) / 2;
			yPos = (getConsoleRows()) / 2;
			gotoxy(xPos, yPos);
			cout << toShow;

			toShow = "Yours is " + to_string(getConsoleColumns()) + " X " + to_string(getConsoleRows());
			xPos = (getConsoleColumns() - toShow.length()) / 2;
			yPos = (getConsoleRows()) / 2 + 1;
			gotoxy(xPos, yPos);
			cout << "Yours is ";
			if (getConsoleColumns() >= 130)
				setConsoleColor(LIGHTGREEN);
			else
				setConsoleColor(LIGHTRED);
			cout << getConsoleColumns();

			setConsoleColor(WHITE);
			cout << " X ";

			if (getConsoleRows() >= 32)
				setConsoleColor(LIGHTGREEN);
			else
				setConsoleColor(LIGHTRED);
			cout << getConsoleRows();

			
			int col = getConsoleColumns();
			int row = getConsoleRows();
			while (col == getConsoleColumns() && row == getConsoleRows())
			{
				// halt if dimentions didnt change
				Sleep(10);
			}
			
			setConsoleColor(WHITE);
			clearConsole();
		}

		setConsoleColor(CYAN);
		cout << "AUTO LIB";
		setConsoleColor(WHITE);
		cout << " | Page -> " << page;
		if (isSignedIn())
		{
			setConsoleColor(LIGHTCYAN);
			cout << " | User -> " << getUser().name << " " << getUser().lastName << "!";
			setConsoleColor(LIGHTGREEN);
			int rate = 0;
			cout << " | Score -> " << getUserScore(rate);
			cout << " (" << ((rate > 0) ? "+" : "") << rate << ")";
		}
		setConsoleColor(WHITE);
		showNotif();
	}
	void home() {
		header();
		string loggedUser = getCookie(loggedUserCookieName);

		setConsoleColor(DARKGREY);
		if (loggedUser == "")
			setConsoleColor(YELLOW);

		cout << "1 - Sign in!\n\n";
		cout << "2 - Sign up!\n\n";
		setConsoleColor(DARKGREY);

		if(loggedUser != "")
			setConsoleColor(YELLOW);
		
		cout << "3 - Sign out!\n\n";
		cout << "4 - Profile!\n\n";
		cout << "5 - My books!\n\n";
		cout << "6 - Explore!\n\n";
		cout << "7 - My requests!\n\n";
		cout << "8 - Rankings!\n\n";
		setConsoleColor(YELLOW);
		cout << "9 - About!\n\n";
		setConsoleColor(LIGHTRED);
		cout << "0 - Exit\n\n";
		setConsoleColor(WHITE);
		setConsoleColor(WHITE);
	}
	void signIn() {
		header("Sign In");
	}
	void signUp() {
		header("Sign Up");
	}
	void signOut() {
		header("Sign Out");
		cout << BLINKERSTART;
		cout << "Are you sure you want to sign out?";
		setConsoleColor(YELLOW);
		cout << " (1 to confirm else to reject): ";
		setConsoleColor(WHITE);
		cout << BLINKEREND;
	}
	void profile() {
		header("Profile");

		User user = getUser();

		const int nameLen = 20;
		const int valueLen = 24;

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Student ID: ";
		setConsoleColor(WHITE);
		cout << left << setw(valueLen) << user.studentID;
		setConsoleColor(YELLOW);
		cout << "\t1 to change\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Name: ";
		setConsoleColor(WHITE);
		cout << left << setw(valueLen) << user.name;
		setConsoleColor(YELLOW);
		cout << "\t2 to change\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Last name: ";
		setConsoleColor(WHITE);
		cout << left << setw(valueLen) << user.lastName;
		setConsoleColor(YELLOW);
		cout << "\t3 to change\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Phone Number: ";
		setConsoleColor(WHITE);
		cout << left << setw(valueLen) << user.phoneNumber;
		setConsoleColor(YELLOW);
		cout << "\t4 to change\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Password: ";
		setConsoleColor(WHITE);

		int passlen = getUser().password.length();
		string passque = "";
		for (int i = 0; i < passlen; i++)
			passque += "*";

		cout << left << setw(valueLen) << passque;
		setConsoleColor(YELLOW);
		cout << "\t5 to change\n\n";

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Been user since: ";
		setConsoleColor(WHITE);
		cout << left << setw(valueLen) << timestamp2string(user.timestampAdded);
		setConsoleColor(YELLOW);
		cout << "\t! can't change\n\n";
		setConsoleColor(WHITE);

		cout << setw(nameLen + valueLen) << "";
		setConsoleColor(LIGHTCYAN);
		cout << "\t-1 to return\n\n";
		setConsoleColor(WHITE);

		cout << setw(nameLen + valueLen) << "";
		setConsoleColor(LIGHTRED);
		cout << "\t0 to exit\n\n";
		setConsoleColor(WHITE);
	}
	void renderBooks(Book books[], int n, int& page, int limit = 5) {
		setConsoleColor(LIGHTGREY);
		//cout << left << setw(5) << "NO.";
		cout << left << setw(5) << "ID";
		cout << left << setw(20) << "Title";
		cout << left << setw(20) << "Author";
		//cout << left << setw(16) << "Condition";
		//cout << left << setw(8) << "Status";
		cout << left << setw(12) << "Description";
		setConsoleColor(WHITE);
		cout << "\n";

		if (page <= 0)
			page = 1;
		page = min(ceil((float)n / limit), page);
		int start = ((page - 1) * limit >= 0) ? (page - 1) * limit : 0;
		int end = min(start + limit, n);
		for (int i = start; i < end; i++)
		{

			//setConsoleColor(LIGHTGREY);
			//cout << left << setw(5) << i+1;
			setConsoleColor(BROWN);
			cout << left << setw(5) << books[i].ID;
			setConsoleColor(YELLOW);
			cout << left << setw(20) << books[i].title.substr(0, 18);
			cout << left << setw(20) << books[i].author.substr(0, 18);
			//cout << left << setw(16) << trasnlateBookCondiotion(books[i].physicalCondition);
			//cout << left << setw(8) << books[i].statusCode;
			cout << left << setw(22) << books[i].description.substr(0, 20);

			if (isBookRequestedByUser(books[i].ID))
			{
				setConsoleColor(63);
				cout << "REQUESTED";
				setConsoleColor(WHITE);
				cout << "  ";
			}
			else if (books[i].statusCode == -1)
			{
				setConsoleColor(207);
				cout << "UNAVAILABLE";
			}
			else if (books[i].statusCode == 1)
			{
				setConsoleColor(47);
				cout << "AVAILABLE";
				setConsoleColor(WHITE);
				cout << "  ";
			}

			// req counts
			setConsoleColor(CYAN);
			cout << "  " << getBookOpenRequestCount(books[i].ID) << " IN QUEUE  ";


			// ownereship stats
			if (books[i].initOwnerUserID == isSignedIn())
			{
				setConsoleColor(143);
				cout << "OWNER";
				setConsoleColor(WHITE);
				cout << "  ";
			}
			else {
				setConsoleColor(WHITE);
				cout << "       ";
			}
			if (books[i].ownerUserID == isSignedIn())
			{
				setConsoleColor(879);
				cout << "HAVE";
				setConsoleColor(WHITE);
				cout << "  ";
			}
			else
			{
				setConsoleColor(WHITE);
				cout << "      ";
			}

			setConsoleColor(WHITE);
			cout << "\n";
		}
		int totalPageCount = ceil((float)n / limit);
		setConsoleColor(WHITE);
		if (totalPageCount == 0)// no data at all
			setConsoleColor(LIGHTRED);

		cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << "\n";
		cout << "                                                                    Page " << page << " of " << totalPageCount << "\n\n";
		setConsoleColor(WHITE);
	}
	void renderRequests(Request requests[], int n, int& page, int limit = 5) {
		setConsoleColor(LIGHTGREY);
		cout << left << setw(5) << "ID";
		cout << left << setw(20) << "User made req";
		cout << left << setw(20) << "User received req";
		cout << left << setw(20) << "Book";
		//cout << left << setw(8) << "Status";
		cout << left << setw(26) << "Date & time";
		setConsoleColor(WHITE);
		cout << "\n";

		if (page <= 0)
			page = 1;
		page = min(ceil((float)n / limit), page);
		int start = ((page - 1) * limit >= 0) ? (page - 1) * limit : 0;
		int end = min(start + limit, n);
		for (int i = start; i < end; i++)
		{

			setConsoleColor(BROWN);
			cout << left << setw(5) << requests[i].ID;
			setConsoleColor(YELLOW);

			User user = getUser(requests[i].toUserID);
			string tmp = user.name + " " + user.lastName;
			tmp = tmp.substr(0, 12);
			tmp += "(ID:" + to_string(user.ID) + ")";
			cout << left << setw(20) << tmp;

			user = getUser(requests[i].fromUserID);
			tmp = user.name + " " + user.lastName;
			tmp = tmp.substr(0, 12);
			tmp += "(ID:" + to_string(user.ID) + ")";
			cout << left << setw(20) << tmp;

			tmp = getBook(requests[i].bookID).title.substr(0,12) + "(ID:" + to_string(requests[i].bookID) + ")";
			cout << left << setw(20) << tmp;
			//cout << left << setw(8) << requests[i].statusCode;
			cout << left << setw(26) << timestamp2string(requests[i].timestampSubmited);

			if (requests[i].toUserID == isSignedIn())
			{
				setConsoleColor(879);
				cout << "YOURS";
				setConsoleColor(WHITE);
				cout << "   ";
			}
			else if (requests[i].fromUserID == isSignedIn())
			{
				setConsoleColor(143);
				cout << "OTHERS";
				setConsoleColor(WHITE);
				cout << "  ";
			}

			if (requests[i].statusCode == 1)
			{
				setConsoleColor(47);
				cout << "ACCEPTED";
			}
			else if (requests[i].statusCode == 0)
			{
				setConsoleColor(63);
				cout << "PENDING";
				setConsoleColor(WHITE);
				cout << " ";
			}
			else {
				setConsoleColor(207);
				cout << "REJECTED";
			}

			setConsoleColor(LIGHTCYAN);
			int rate = 0;
			tmp = "Score: " + to_string(getUserScore(rate, requests[i].toUserID));
			tmp += "(" + to_string(rate) + ")";
			cout << "  " << setw(16) << tmp;

			cout << "\n";
		}
		int totalPageCount = ceil((float)n / limit);
		setConsoleColor(WHITE);
		if (totalPageCount == 0)// no data at all
			setConsoleColor(LIGHTRED);
		
		cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << "\n";
		cout << "                                                                    Page " << page << " of " << totalPageCount << "\n\n";
		setConsoleColor(WHITE);
	}
	void renderUsers(User users[], int n, int& page, int limit = 5) {
		setConsoleColor(LIGHTGREY);
		cout << left << setw(5) << "No.";
		cout << left << setw(20) << "Full name";
		cout << left << setw(28) << "Date & time joined";
		cout << left << setw(10) << "Score";
		setConsoleColor(WHITE);
		cout << "\n";
		
		int yourPos = -1;

		if (page <= 0)
			page = 1;
		page = min(ceil((float)n / limit), page);
		int start = ((page - 1) * limit >= 0) ? (page - 1) * limit : 0;
		int end = min(start + limit, n);
		for (int i = start; i < end; i++)
		{
			if (users[i].ID == isSignedIn())
				yourPos = i + 1;

			setConsoleColor(BROWN);
			cout << left << setw(5) << i+1;
			setConsoleColor(YELLOW);

			string tmp = users[i].name + " " + users[i].lastName;
			cout << left << setw(20) << tmp;
			cout << left << setw(28) << timestamp2string(users[i].timestampAdded);
			int rate = 0;
			int id = users[i].ID;
			cout << left << setw(10) << getUserScore(rate, id);
			cout << right << setw(2) << "(" << ((rate > 0) ? "+" : "") << rate << ")  ";

			if (users[i].ID == isSignedIn())
			{
				setConsoleColor(47);
				cout << "YOU";
			}
			
			setConsoleColor(WHITE);
			cout << "\n";
		}
		string posEnding;
		switch (yourPos%10)
		{
		case 1:
			posEnding = "st";
			break;
		case 2:
			posEnding = "nd";
			break;
		case 3:
			posEnding = "rd";
			break;
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 0:
			posEnding = "th";
			break;
		}
		int totalPageCount = ceil((float)n / limit);
		setConsoleColor(WHITE);
		if (totalPageCount == 0)// no data at all
			setConsoleColor(LIGHTRED);

		cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << "\n";
		cout << "You are " << yourPos << posEnding << "                                                         Page " << page << " of " << ceil((float)n / limit) << "\n\n";
		setConsoleColor(WHITE);
	}
	void EditBook(int bookID) {
		header("Show/Edit Book");

		Book book = getBook(bookID);

		const int nameLen = 18;
		const int valueLen = 32;

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Title: ";
		setConsoleColor(WHITE);
		cout << left << setw(valueLen) << book.title.substr(0, valueLen);
		setConsoleColor(YELLOW);
		cout << "\t1 to change\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Author: ";
		setConsoleColor(WHITE);
		cout << left << setw(valueLen) << book.author.substr(0, valueLen);
		setConsoleColor(YELLOW);
		cout << "\t2 to change\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Condition: ";
		setConsoleColor(WHITE);
		cout << left << setw(valueLen) << trasnlateBookCondiotion(book.physicalCondition).substr(0, valueLen);
		setConsoleColor(YELLOW);
		cout << "\t3 to change\n\n";

		setConsoleColor(WHITE);
		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Status: ";
		setConsoleColor(WHITE);
		cout << left << setw(valueLen) << trasnlateBookStatus(book.statusCode).substr(0, valueLen);
		setConsoleColor(YELLOW);
		cout << "\t4 to change\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Description: ";
		setConsoleColor(WHITE);
		cout << left << setw(valueLen) << book.description.substr(0, valueLen);
		setConsoleColor(YELLOW);
		cout << "\t5 to change\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Date added: ";
		setConsoleColor(WHITE);
		cout << left << setw(valueLen) << timestamp2string(book.timestampAdded);
		setConsoleColor(YELLOW);
		cout << "\t5 to change\n\n";
		setConsoleColor(WHITE);

		cout << setw(nameLen + valueLen) << "";
		setConsoleColor(LIGHTCYAN);
		cout << "\t-1 to return\n\n";
		setConsoleColor(WHITE);

		cout << setw(nameLen + valueLen) << "";
		setConsoleColor(LIGHTRED);
		cout << "\t0 to exit\n\n";
		setConsoleColor(WHITE);
	}
	void addBook() {
		header("Add Book");
	}
	void myBooks(int& page) {
		header("My Books");

		Book books1[MaxFileLines], books2[MaxFileLines], result[MaxFileLines], lookFor1, lookFor2;

		lookFor1.initOwnerUserID = isSignedIn();
		searchBooks(books1, lookFor1);

		lookFor2.ownerUserID = isSignedIn();
		searchBooks(books2, lookFor2);

		int n = mergeBooks(books1, books2, result);

		intro::renderBooks(result, n, page);

		setConsoleColor(YELLOW);
		cout << "1 to add a new book\n";
		cout << "2 to show open requests\n";
		cout << "3 to show/edit book info & availability\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(CYAN);
		cout << "7 to go to previous page\n";
		cout << "8 to go to a custom page\n";
		cout << "9 to go to next page\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTCYAN);
		cout << "-1 to return\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTRED);
		cout << "0 to exit\n\n";
		setConsoleColor(WHITE);
	}
	void explore(int& page, Book lookFor) {
		header("Explore");
		Book books[MaxFileLines];
		int n = searchBooks(books, lookFor);
		intro::renderBooks(books, n, page);

		setConsoleColor(YELLOW);
		cout << "1 to request a book\n";
		setConsoleColor(WHITE);

		setConsoleColor(YELLOW);
		cout << "2 to search for a book/clear filters ";
		if (lookFor.ID == 0 &&
			lookFor.initOwnerUserID == 0 &&
			lookFor.title == "" &&
			lookFor.author == "" &&
			lookFor.physicalCondition == 0 &&
			lookFor.statusCode == 0 &&
			lookFor.description == "")
		{
			cout << "\n";
		}
		else {
			setConsoleColor(96);
			cout << "FILTERS INCLUDED!\n";
		}
		setConsoleColor(WHITE);

		setConsoleColor(YELLOW);
		cout << "3 to show/edit book info & availability\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(CYAN);
		cout << "7 to go to previous page\n";
		cout << "8 to go to a custom page\n";
		cout << "9 to go to next page\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTCYAN);
		cout << "-1 to return\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTRED);
		cout << "0 to exit\n\n";
		setConsoleColor(WHITE);
	}
	void termsOfUse() {
		header("Terms Of Use");

		cout << "Before going any further, you must agree with our terms of use for the first time adding/requesting book!\n\n";
		
		setConsoleColor(96);
		cout << "Terms of Use";
		setConsoleColor(WHITE);

		cout << "\n\n";

		setConsoleColor(LIGHTCYAN);
		cout << "Introduction";
		setConsoleColor(WHITE);

		cout << "\n\nBy accessing Our Project, you acknowledge that you have read, understood,\nand agree to be bound by the following terms and conditions.\n\n";

		setConsoleColor(LIGHTCYAN);
		cout << "User Responsibilities";
		setConsoleColor(WHITE);

		cout << "\n\n1. Record Keeping: Be aware that all actions and requests are logged in our databases for future reference.\n";
		cout << "2. Book Ownership: The initial owner of a book retains ownership rights and may request its return at any time.\n";
		cout << "3. Account Information: Misuse of another person's information to access this system may lead to serious consequences.\n";
		cout << "4. Purpose: This platform is designed exclusively to facilitate easier access to resources for students.\n";
		cout << "5. Rights and Usage: While Auto Lib is free to use, all rights are reserved.\n\n";

		/*setConsoleColor(LIGHTCYAN);
		cout << "Intellectual Property";
		setConsoleColor(WHITE);

		cout << "\nUnless otherwise stated, Our Project and /or its licensors own the intellectual property rights for all material on this project.\n\n";

		setConsoleColor(LIGHTCYAN);
		cout << "Usage Restrictions";
		setConsoleColor(WHITE);

		cout << "\nYou are expressly and emphatically restricted from all of the following : publishing, selling, or using this project in any manner that could damage or cause risk to the project or contravene any applicable laws.\n\n";

		setConsoleColor(LIGHTCYAN);
		cout << "User Content";
		setConsoleColor(WHITE);

		cout << "\nAuto Lib refers to any material you choose to display on this project.By displaying Your Content, you grant Our Project a non - exclusive, worldwide, irrevocable license to use, reproduce, adapt, publish, and distribute it.\n\n";

		setConsoleColor(LIGHTCYAN);
		cout << "Governing Law";
		setConsoleColor(WHITE);

		cout << "\nThese Terms are governed by and construed in accordance with the laws of[Your State], and you agree to submit to the exclusive jurisdiction of the state and federal courts in[Your State] for the resolution of any disputes.\n\n";

		setConsoleColor(LIGHTCYAN);
		cout << "Terms Modification";
		setConsoleColor(WHITE);

		cout << "\nWe reserve the right to revise these terms at any time.You are expected to review these terms periodically to stay informed of updates.\n\n";

		setConsoleColor(LIGHTCYAN);
		cout << "Contact Us";
		setConsoleColor(WHITE);

		cout << "\nIf you have any questions about these Terms, please contact us.\n\n";*/

		setConsoleColor(YELLOW);
		cout << "1 to accept\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTCYAN);
		cout << "-1 to return\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTRED);
		cout << "0 to exit\n\n";
		setConsoleColor(WHITE);
	}
	void requestBook(int bookID) {
		header("Request Book");
		cout << "Make sure that the book below is exactly what you are looking for?\n\n";

		Book book[1];
		book[0] = getBook(bookID);
		int page = 1;
		intro::renderBooks(book,1,page);

		setConsoleColor(YELLOW);
		cout << "1 to proceed & request\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTCYAN);
		cout << "-1 to return\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTRED);
		cout << "0 to exit\n\n";
		setConsoleColor(WHITE);
	}
	void searchBook(Book& lookFor) {
		header("Search Book");

		const int nameLen = 18;
		const int valueLen = 26;

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Title: ";
		if (lookFor.title != "")
		{
			setConsoleColor(WHITE);
			cout << left << setw(valueLen) << lookFor.title.substr(0, valueLen);
		}
		else {
			setConsoleColor(BROWN);
			cout << left << setw(valueLen) <<  "ANY";
		}
		setConsoleColor(YELLOW);
		cout << "\t1 to change\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Author: ";
		setConsoleColor(WHITE);
		if (lookFor.author != "")
		{
			setConsoleColor(WHITE);
			cout << left << setw(valueLen) << lookFor.author.substr(0, valueLen);
		}
		else {
			setConsoleColor(BROWN);
			cout << left << setw(valueLen) << "ANY";
		}
		setConsoleColor(YELLOW);
		cout << "\t2 to change\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Condition: ";
		setConsoleColor(WHITE);
		if (lookFor.physicalCondition != 0)
		{
			setConsoleColor(WHITE);
			cout << left << setw(valueLen) << trasnlateBookCondiotion(lookFor.physicalCondition).substr(0, valueLen);
		}
		else {
			setConsoleColor(BROWN);
			cout << left << setw(valueLen) << "ANY";
		}
		setConsoleColor(YELLOW);
		cout << "\t3 to change\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Description: ";
		setConsoleColor(WHITE);
		if (lookFor.description != "")
		{
			setConsoleColor(WHITE);
			cout << left << setw(valueLen) << lookFor.description.substr(0, valueLen);
		}
		else {
			setConsoleColor(BROWN);
			cout << left << setw(valueLen) << "ANY";
		}
		setConsoleColor(YELLOW);
		cout << "\t4 to change\n\n";
		setConsoleColor(WHITE);

		cout << setw(nameLen + valueLen) << "";
		setConsoleColor(CYAN);
		cout << "\t5 to clear all filters\n\n";
		setConsoleColor(WHITE);

		cout << setw(nameLen + valueLen) << "";
		setConsoleColor(LIGHTCYAN);
		cout << "\t-1 to return\n\n";
		setConsoleColor(WHITE);

		cout << setw(nameLen + valueLen) << "";
		setConsoleColor(LIGHTRED);
		cout << "\t0 to exit\n\n";
		setConsoleColor(WHITE);
	}
	void about() {
		header("About");

		cout << "This project, as you may have guessed, is an Automated Library.\nUnlike traditional libraries, an Automated Library doesn't physically exist.\nInstead, books are handed by one user to another and this cycle keeps going.\n";
		cout << "While the concept is straightforward, the implementation in code is quite challenging.\nNonetheless, it's a challenge I embraced with dedication and hard work.\n\n";
		cout << "This project was developed by ";
		setConsoleColor(96);
		cout << "Mohammad Valadi";
		setConsoleColor(WHITE);
		cout << ", fueled by a deep passion for learning and alot of love for coding.\nStriving for perfection, I dedicated more than four hours each day for two straight weeks to complete this project.\n";
		cout << "The journey was intense, but the knowledge and experience gained made every moment worthwhile.\nI hope this project not only showcases my coding skills but also reflects the hard work and dedication I put into it.\n\n";

		cout << left << setw(14) << "University:";
		cout << "JUNDI SHAPUR University of Technology, DEZFUL, IRAN\n";
		cout << left << setw(14) << "Lesson:";
		cout << "Advaned Programming\n";
		cout << left << setw(14) << "Professor:";
		cout << "DR. Moussa Moussazadeh\n";
		cout << left << setw(14) << "Date:";
		cout << "March 2024\n\n";
		cout << "All rights reserved!\n\n";

		setConsoleColor(LIGHTCYAN);
		cout << "-1 to return\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTRED);
		cout << "0 to exit\n\n";
		setConsoleColor(WHITE);
	}
	void bookRequests(int bookID, int& page) {
		header("Book Requests");
		Request requests[MaxFileLines];
		int n = getBookOpenRequests(requests, bookID);

		intro::renderRequests(requests, n, page);

		setConsoleColor(YELLOW);
		cout << "1 to accept request\n";
		cout << "2 to reject request\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(CYAN);
		cout << "7 to go to previous page\n";
		cout << "8 to go to a custom page\n";
		cout << "9 to go to next page\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTCYAN);
		cout << "-1 to return\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTRED);
		cout << "0 to exit\n\n";
		setConsoleColor(WHITE);
	}
	void myRequests(int& page) {
		header("My Requests");
		Request requests[MaxFileLines];
		int n = getUserRequests(requests);

		intro::renderRequests(requests, n,page);

		setConsoleColor(YELLOW);
		cout << "1 to Accept\n";
		cout << "2 to Reject\n";
		cout << "3 to show more details\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(CYAN);
		cout << "7 to go to previous page\n";
		cout << "8 to go to a custom page\n";
		cout << "9 to go to next page\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTCYAN);
		cout << "-1 to return\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTRED);
		cout << "0 to exit\n\n";
		setConsoleColor(WHITE);
	}
	void showRequestDetails(int reqID) {
		header("Request Details");

		Request request = getRequest(reqID);

		const int nameLen = 26;
		const int valueLen = 38;

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "ID: ";
		setConsoleColor(WHITE);
		cout << left << setw(valueLen) << request.ID;
		setConsoleColor(YELLOW);
		cout << "\t! can't change\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "User made request: ";
		setConsoleColor(WHITE);
		User user = getUser(request.toUserID);
		string tmp = user.name + " " + user.lastName + "(ID:" + to_string(user.ID) + ")";
		cout << left << setw(valueLen) << tmp;
		setConsoleColor(YELLOW);
		cout << "\t! can't change\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "User being asked: ";
		setConsoleColor(WHITE);
		user = getUser(request.fromUserID);
		tmp = user.name + " " + user.lastName + "(ID:" + to_string(request.bookID) + ")";
		cout << left << setw(valueLen) << tmp;
		setConsoleColor(YELLOW);
		cout << "\t! can't change\n\n";

		setConsoleColor(WHITE);
		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Book ID: ";
		setConsoleColor(WHITE);
		tmp = getBook(request.bookID).title + "(ID:" + to_string(request.bookID) + ")";
		cout << left << setw(valueLen) << tmp;
		setConsoleColor(YELLOW);
		cout << "\t! can't change\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Status: ";
		setConsoleColor(WHITE);
		cout << left << setw(valueLen) << translateRequestStatus(request.statusCode);
		setConsoleColor(YELLOW);
		cout << "\t! can't change\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Date & time submited: ";
		setConsoleColor(WHITE);
		cout << left << setw(valueLen) << timestamp2string(request.timestampSubmited);
		setConsoleColor(YELLOW);
		cout << "\t! can't change\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Date & time recieved: ";
		setConsoleColor(WHITE);
		cout << left << setw(valueLen) << timestamp2string(request.timestampRecieved);
		setConsoleColor(YELLOW);
		cout << "\t! can't change\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTGREY);
		cout << left << setw(nameLen) << "Date & time returned: ";
		setConsoleColor(WHITE);
		cout << left << setw(valueLen) << timestamp2string(request.timestampReturned);
		setConsoleColor(YELLOW);
		cout << "\t! can't change\n\n";
		setConsoleColor(WHITE);

		cout << setw(nameLen + valueLen) << "";
		setConsoleColor(LIGHTCYAN);
		cout << "\t-1 to return\n\n";
		setConsoleColor(WHITE);

		cout << setw(nameLen + valueLen) << "";
		setConsoleColor(LIGHTRED);
		cout << "\t0 to exit\n\n";
		setConsoleColor(WHITE);
	}
	void usersRankings(int& page) {
		header("My Requests");
		User users[MaxFileLines];
		int n = getAllUsers(users);
		sortUsersByScore(users, n);

		intro::renderUsers(users, n, page);

		/*setConsoleColor(YELLOW);
		cout << "! to !!!\n";
		cout << "! to !!!\n";
		cout << "! to !!!\n\n";
		setConsoleColor(WHITE);*/

		setConsoleColor(CYAN);
		cout << "7 to go to previous page\n";
		cout << "8 to go to a custom page\n";
		cout << "9 to go to next page\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTCYAN);
		cout << "-1 to return\n\n";
		setConsoleColor(WHITE);

		setConsoleColor(LIGHTRED);
		cout << "0 to exit\n\n";
		setConsoleColor(WHITE);
	}
}

namespace view {
	void signIn() {
		if (isSignedIn())
		{
			setNotif("error", "No access!");
			setNotif("warning", "Already signed in!");
			return;
		}
		intro::signIn();
		User sample;
		cout << "Enter your studentID: ";
		cin >> sample.studentID;
		cout << "Enter your password: ";
		getline(cin >> ws, sample.password);

		srand(time(0));
		int num1 = rand()%10, num2 = rand()%10;
		int sq, sqAnswer = num1 + num2;// security question
		cout << "\nTo make sure you are not a robot, please answer this security question:\n\n";
		cout << "What is " << num1 << " + " << num2 << " = ";

		cin >> sq;
		if (sq != sqAnswer)
		{
			setNotif("error", "Wrong answer to security question!");
			setNotif("warning", "We couldn't identify you as a human!");
			return;
		}

		if (attempSignIn(sample))
			setNotif("text", "Welcome back!");
	}
	void signUp() {
		if (isSignedIn())
		{
			setNotif("error", "No access!");
			setNotif("warning", "Already signed in!");
			return;
		}
		intro::signUp();
		User user;
		cout << "Please enter your student ID: ";
		cin >> user.studentID;
		cout << "Please enter your phone number: ";
		cin >> user.phoneNumber;
		cout << "Please enter your name: ";
		getline(cin >> ws, user.name);
		cout << "Please enter your last name: ";
		getline(cin >> ws, user.lastName);
		cout << "Please enter your password: ";
		getline(cin >> ws, user.password);

		srand(time(0));
		int num1 = rand() % 10, num2 = rand() % 10;
		int sq, sqAnswer = num1 + num2;// security question
		cout << "\nTo make sure you are not a robot, please answer this security question:\n\n";
		cout << "What is " << num1 << " + " << num2 << " = ";

		cin >> sq;
		if (sq != sqAnswer)
		{
			setNotif("error", "Wrong answer to security question!");
			setNotif("warning", "We couldn't identify you as a human!");
			return;
		}

		char tmp;
		cout << "Do you want to sign in after registration?(1 to confirm else to reject) ";
		cin >> tmp;
		addUser(user, ((tmp == '1') ? true : false));
	}
	void signOut() {
		if (!isSignedIn())
		{
			setNotif("error", "No access!");
			setNotif("warning", "You first have to sign in!");
			return;
		}
		intro::signOut();
		char c;
		cin >> c;
		if (c == '1')
			attempSignOut();
		else
			setNotif("warning","Sign out cancelled!");
		cin.ignore();
	}
	void profile() {
		while (true)
		{
			if (!isSignedIn())
			{
				setNotif("error", "No access!");
				setNotif("warning", "You first have to sign in!");
				return;
			}
			intro::profile();

			User user = getUser();

			int cc; // Command Code
			cout << BLINKERSTART << "Ready for your command: " << BLINKEREND;
			cin >> cc;

			// for password if needed in switch block
			string passtmp;

			switch (cc)
			{
			case 0:
				exit(0);
				break;
			case -1:
				return;
				break;
			case 1:
				cout << "Enter new student ID: ";
				cin >> user.studentID;
				updateUser(user);
				break;
			case 2:
				cout << "Enter new name: ";
				getline(cin >> ws, user.name);
				updateUser(user);
				break;
			case 3:
				cout << "Enter new last name: ";
				getline(cin >> ws, user.lastName);
				updateUser(user);
				break;
			case 4:
				cout << "Enter new phone number: ";
				cin >> user.phoneNumber;
				updateUser(user);
				break;
			case 5:
				cout << "Enter old password: ";
				getline(cin >> ws, passtmp);

				if (passtmp != user.password)
				{
					setNotif("error", "Wrong password!");
					continue;
				}

				cout << "Enter new password: ";
				getline(cin >> ws, user.password);
				updateUser(user);
				break;
			default:
				setNotif("error", "Unknown Command!!!");
				break;
			}
		}
	}
	void termsOfUse() {
		while (true)
		{
			intro::termsOfUse();

			int cc, cc2; // Command Code
			cout << BLINKERSTART << "Ready for your command: " << BLINKEREND;
			cin >> cc;
			User user;
			switch (cc)
			{
			case 0:
				exit(0);
				break;
			case -1:
				return;
				break;
			case 1:
				user = getUser();
				user.acceptedTerms = 1;
				updateUser(user);
				setNotif("success", "You accepted terms of use! You wont be asked again!");
				return;
				break;
			default:
				setNotif("error", "Unknown Command!!!");
				break;
			}
		}
	}
	void bookRequests(int bookID) {
		if (!isSignedIn())
		{
			setNotif("error", "No access!");
			return;
		}

		Book book = getBook(bookID);
		if (book.ownerUserID != isSignedIn())
		{
			setNotif("error", "No access!");
			if (book.initOwnerUserID == isSignedIn())
				setNotif("warning", "You are the initial owner but the book is not yours now!");
			
			return;
		}
		int page = 1;
		while (true)
		{
			intro::bookRequests(bookID, page);
			int cc, cc2; // Command Code
			cout << BLINKERSTART << "Ready for your command: " << BLINKEREND;
			cin >> cc;
			
			switch (cc)
			{
			case 0:
				exit(0);
				break;
			case -1:
				return;
				break;
			case 1:
				cout << "Enter the request ID you want to accept: ";
				cin >> cc2;

				if (attempAcceptBookRequest(bookID, cc2))
				{
					setNotif("success", "One request accepted and others rejected!");
					setNotif("text", "Book transfered successfully!");
					return;
				}
				else {
					setNotif("error", "Couldn't accept!");
				}
				break;
			case 2:
				cout << "Enter the request ID you want to reject: ";
				cin >> cc2;

				if (attempRejectBookRequest(cc2))
				{
					setNotif("success", "One request rejected!");
					setNotif("text", "Book transfered successfully!");
				}
				else {
					setNotif("error", "Couldn't reject!");
				}
				break;
			case 7:
				page--;
				break;
			case 8:
				cout << "Enter page number: ";
				cin >> cc2;
				page = cc2;
				break;
			case 9:
				page++;
				break;
			default:
				setNotif("error", "Unknown Command!!!");
				break;
			}
		}
	}
	void EditBook(int bookID) {
		while (true)
		{
			if (!isSignedIn())
			{
				setNotif("error", "No access!");
				return;
			}

			Book book = getBook(bookID);
			if (book.ID == 0)
			{
				setNotif("error", "No access!");
				setNotif("warning", "Book doesn't exist!");
				return;
			}

			intro::EditBook(bookID);

			int cc; // Command Code
			cout << BLINKERSTART << "Ready for your command: " << BLINKEREND;
			cin >> cc;
			if (cc > 0 || cc < -1)
			{
				if (book.ownerUserID != isSignedIn())
				{
					setNotif("error", "No access!");
					setNotif("warning", "You don't have this book!");
					continue;
				}
			}
			switch (cc)
			{
			case 0:
				exit(0);
				break;
			case -1:
				return;;
				break;
			case 1:
				cout << "Enter new title: ";
				getline(cin >> ws, book.title);
				updateBook(book);
				break;
			case 2:
				cout << "Enter new author: ";
				getline(cin >> ws, book.author);
				updateBook(book);
				break;
			case 3:
				cout << "A Valid physical conditios is a rate between 1 to 5 from worst to best\n";
				cout << "Enter new condition: ";
				cin >> book.physicalCondition;
				updateBook(book);
				break;
			case 4:
				cout << "A Valid status is a identifier either 1(available) or -1(unavailable)\n";
				cout << "When a book is unavailable others can't make request for it\n";
				cout << "Enter new status: ";
				cin >> book.statusCode;
				updateBook(book);
				break;
			case 5:
				cout << "Enter new description(0 to remove): ";
				getline(cin >> ws, book.description);				
				updateBook(book);
				break;
			default:
				setNotif("error", "Unknown Command!!!");
				break;
			}
		}
	}
	void addBook() {
		if (!isSignedIn())
		{
			setNotif("error", "No access!");
			return;
		}
		if (!hasAcceptedTerms())
		{
			view::termsOfUse();
			if (!hasAcceptedTerms())
			{
				setNotif("error", "You are not allowed to add a book/request!");
				setNotif("warning", "You've rejected our terms of use!");
				return;
			}
		}
		intro::addBook();

		Book book;

		cout << "please enter your book's title: ";
		getline(cin >> ws, book.title);
		cout << "please enter your book's Author: ";
		getline(cin >> ws, book.author);
		cout << "\nA Valid physical conditios is a rate between 1 to 5 from worst to best\n";
		cout << "please enter your book's physical condition: ";
		cin >> book.physicalCondition;
		cout << "please enter your book's description(0 to skip): ";
		getline(cin >> ws, book.description);

		addBook(book);
	}
	void myBooks() {
		if (!isSignedIn())
		{
			setNotif("error", "No access!");
			setNotif("warning", "You first have to sign in!");
			return;
		}
		int page = 1;
		while (true)
		{
			intro::myBooks(page);

			int cc, cc2; // Command Code
			cout << BLINKERSTART << "Ready for your command: " << BLINKEREND;
			cin >> cc;
			switch (cc)
			{
			case 0:
				exit(0);
				break;
			case -1:
				return;
				break;
			case 1:
				view::addBook();
				break;
			case 2:
				cout << "Enter the book ID you want to see requests: ";
				cin >> cc2;
				view::bookRequests(cc2);
				break;
			case 3:
				cout << "Enter the book ID you want to show/edit: ";
				cin >> cc2;
				view::EditBook(cc2);
				break;
			case 7:
				page--;
				break;
			case 8:
				cout << "Enter page number: ";
				cin >> cc2;
				page = cc2;
				break;
			case 9:
				page++;
				break;
			default:
				setNotif("error", "Unknown Command!!!");
				break;
			}
		}
	}
	void requestBook(int bookID) {
		if (!isSignedIn())
		{
			setNotif("error", "No access!");
			setNotif("warning", "You first have to sign in!");
			return;
		}
		if (!hasAcceptedTerms())
		{
			view::termsOfUse();
			if (!hasAcceptedTerms())
			{
				setNotif("error", "You are not allowed to request a book/request!");
				setNotif("warning", "You've rejected our terms of use!");
				return;
			}
		}
		while (true)
		{
			intro::requestBook(bookID);

			int cc, cc2; // Command Code
			cout << BLINKERSTART << "Ready for your command: " << BLINKEREND;
			cin >> cc;
			switch (cc)
			{
			case 0:
				exit(0);
				break;
			case -1:
				return;
				break;
			case 1:
				attempRequestBook(bookID);
				return;
				break;
			default:
				setNotif("error", "Unknown Command!!!");
				break;
			}
		}
	}
	void searchBook(Book& lookFor) {
		while (true)
		{
			if (!isSignedIn())
			{
				setNotif("error", "No access!");
				return;
			}
			intro::searchBook(lookFor);
			Book empty;
			int cc; // Command Code
			cout << BLINKERSTART << "Ready for your command: " << BLINKEREND;
			cin >> cc;
			switch (cc)
			{
			case 0:
				exit(0);
				break;
			case -1:
				return;;
				break;
			case 1:
				cout << "Enter the title you are looking for: ";
				getline(cin >> ws, lookFor.title);
				setNotif("success", "Filters included successfully!");
				setNotif("text", "To see the filtered results use -1 to return!");
				break;
			case 2:
				cout << "Enter the author you are looking for: ";
				getline(cin >> ws, lookFor.author);
				setNotif("success", "Filters included successfully!");
				setNotif("text", "To see the filtered results use -1 to return!");
				break;
			case 3:
				cout << "Enter the condition you are looking for: ";
				cin >> lookFor.physicalCondition;
				setNotif("success", "Filters included successfully!");
				setNotif("text", "To see the filtered results use -1 to return!");
				break;
			case 4:
				cout << "Enter the description you are looking for: ";
				getline(cin >> ws, lookFor.description);
				setNotif("success", "Filters included successfully!");
				setNotif("text", "To see the filtered results use -1 to return!");
				break;
			case 5:
				lookFor = empty;
				setNotif("success", "All filters removed successfully!");
				return;
				break;
			default:
				setNotif("error", "Unknown Command!!!");
				break;
			}
		}
	}
	void explore() {
		if (!isSignedIn())
		{
			setNotif("error", "No access!");
			setNotif("warning", "You first have to sign in!");
			return;
		}
		int page = 1;
		Book lookFor;
		while (true)
		{
			intro::explore(page, lookFor);

			int cc, cc2; // Command Code
			cout << BLINKERSTART << "Ready for your command: " << BLINKEREND;
			cin >> cc;
			switch (cc)
			{
			case 0:
				exit(0);
				break;
			case -1:
				return;
				break;
			case 1:
				cout << "Enter the book ID: ";
				cin >> cc2;
				view::requestBook(cc2);
				break;
			case 2:
				view::searchBook(lookFor);
				break;
			case 3:
				cout << "Enter the book ID you want to show/edit: ";
				cin >> cc2;
				view::EditBook(cc2);
				break;
			case 7:
				page--;
				break;
			case 8:
				cout << "Enter page number: ";
				cin >> cc2;
				page = cc2;
				break;
			case 9:
				page++;
				break;
			default:
				setNotif("error", "Unknown Command!!!");
				break;
			}
		}
	}
	void showRequestDetails(int reqID) {
		while (true)
		{
			if (!isSignedIn())
			{
				setNotif("error", "No access!");
				return;
			}

			Request request = getRequest(reqID);
			if (request.ID == 0)
			{
				setNotif("error", "No access!");
				setNotif("warning", "Request doesn't exist!");
				return;
			}

			if (request.fromUserID != isSignedIn() && request.toUserID != isSignedIn())
			{
				setNotif("error", "No access!");
				setNotif("warning", "You are not a participant in this request!");
				return;
			}

			intro::showRequestDetails(reqID);


			int cc; // Command Code
			cout << BLINKERSTART << "Ready for your command: " << BLINKEREND;
			cin >> cc;
			switch (cc)
			{
			case 0:
				exit(0);
				break;
			case -1:
				return;;
				break;
			default:
				setNotif("error", "Unknown Command!!!");
				break;
			}
		}
	}
	void myRequests() {
		if (!isSignedIn())
		{
			setNotif("error", "No access!");
			setNotif("warning", "You first have to sign in!");
			return;
		}
		if (!hasAcceptedTerms())
		{
			view::termsOfUse();
			if (!hasAcceptedTerms())
			{
				setNotif("error", "You are not allowed to add a book/request!");
				setNotif("warning", "You've rejected our terms of use!");
				return;
			}
		}
		int page = 1;
		while (true)
		{
			intro::myRequests(page);

			int cc, cc2; // Command Code
			cout << BLINKERSTART << "Ready for your command: " << BLINKEREND;
			cin >> cc;
			Request request;
			switch (cc)
			{
			case 0:
				exit(0);
				break;
			case -1:
				return;
				break;
			case 1:
				cout << "Enter the request ID you want to see details: ";
				cin >> cc2;
				request = getRequest(cc2);
				attempAcceptBookRequest(request.bookID, cc2);
				break;
			case 2:
				cout << "Enter the request ID you want to see details: ";
				cin >> cc2;
				attempRejectBookRequest(cc2);
				break;
			case 3:
				cout << "Enter the request ID you want to see details: ";
				cin >> cc2;
				view::showRequestDetails(cc2);
				break;
			case 7:
				page--;
				break;
			case 8:
				cout << "Enter page number: ";
				cin >> cc2;
				page = cc2;
				break;
			case 9:
				page++;
				break;
			default:
				setNotif("error", "Unknown Command!!!");
				break;
			}
		}
	}
	void usersRankings() {
		if (!isSignedIn())
		{
			setNotif("error", "No access!");
			setNotif("warning", "You first have to sign in!");
			return;
		}
		int page = 1;
		while (true)
		{
			intro::usersRankings(page);

			int cc, cc2; // Command Code
			cout << BLINKERSTART << "Ready for your command: " << BLINKEREND;
			cin >> cc;
			switch (cc)
			{
			case 0:
				exit(0);
				break;
			case -1:
				return;
				break;
			/*case 1:
				cout << "Enter the request ID you want to see details: ";
				cin >> cc2;
				request = getRequest(cc2);
				attempAcceptBookRequest(request.bookID, cc2);
				break;
			case 2:
				cout << "Enter the request ID you want to see details: ";
				cin >> cc2;
				attempRejectBookRequest(cc2);
				break;
			case 3:
				cout << "Enter the request ID you want to see details: ";
				cin >> cc2;
				view::showRequestDetails(cc2);
				break;*/
			case 7:
				page--;
				break;
			case 8:
				cout << "Enter page number: ";
				cin >> cc2;
				page = cc2;
				break;
			case 9:
				page++;
				break;
			default:
				setNotif("error", "Unknown Command!!!");
				break;
			}
		}
	}
	void about() {
		while (true)
		{
			intro::about();

			int cc, cc2; // Command Code
			cout << BLINKERSTART << "Ready for your command: " << BLINKEREND;
			cin >> cc;
			switch (cc)
			{
			case 0:
				exit(0);
				break;
			case -1:
				return;
				break;
			default:
				setNotif("error", "Unknown Command!!!");
				break;
			}
		}
	}
	void home() {
		while (true)
		{
			intro::home();
			int cc; // Command Code
			cout << BLINKERSTART << "Ready for your command: " << BLINKEREND;
			cin >> cc;

			switch (cc)
			{
			case 0:
				exit(0);
				break;
			case 1:
				view::signIn();
				break;
			case 2:
				view::signUp();
				break;
			case 3:
				view::signOut();
				break;
			case 4:
				view::profile();
				break;
			case 5:
				view::myBooks();
				break;
			case 6:
				view::explore();
				break;
			case 7:
				view::myRequests();
				break;
			case 8:
				view::usersRankings();
				break;
			case 9:
				view::about();
				break;
			default:
				setNotif("error", "Unknown Command!!!");
				break;
			}
		}
	}
}

int main()
{
	view::home();
	
	// color tester
	/*for (int i = 0; i < 1000; i++)
	{
		setConsoleColor(i);
		cout << "this is test" << i << endl;

	}*/
}
