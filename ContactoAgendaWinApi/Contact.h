#pragma once
#include <string>

using namespace std;

struct Contact {

	int contactId;
	wstring phoneNumber;
	wstring name;
	wstring lastName;
	wstring email;
	wstring imgPath[2];
	wstring street;
	wstring streetNumber;
	wstring betweenStreets;
	wstring CP;
	wstring suburb;
	wstring city;
	wstring state;
	wstring country;

	Contact *next;
	Contact *prev;

};