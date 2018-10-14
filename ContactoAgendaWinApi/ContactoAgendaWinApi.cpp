#include "stdafx.h"
#include "Contact.h"
#include "ContactoAgendaWinApi.h"
#include <fstream>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;

HWND hwndMainWindow;
HWND hwndMainWindowListBox;

LRESULT CALLBACK mainWindowProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK addContactWindowProc(HWND, UINT, WPARAM, LPARAM);


int getContactNewId();
void getContactById(int, Contact&);
void addContact(HWND);
void saveContacts(HWND);
void loadContacts();

void loadContactListBox(HWND, int);

Contact *origin, *aux;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

	origin = aux = NULL;

	loadContacts();

	hwndMainWindow = CreateDialog(hInstance, MAKEINTRESOURCE(ID_ContactListDialog), 0, mainWindowProc);
	ShowWindow(hwndMainWindow, SW_SHOW);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK mainWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{

		loadContactListBox(hWnd, ID_ContactListBox);

	}
	break;
	case WM_COMMAND:
	{
		int who = LOWORD(wParam);
		int what = HIWORD(wParam);

		switch (who)
		{
			case ID_AddContactWindowBtn: {

				HWND hwndAddContactDialog = CreateDialog(hInst, MAKEINTRESOURCE(ID_AddContactDialog), 0, addContactWindowProc);
				ShowWindow(hwndMainWindow, SW_HIDE);
				ShowWindow(hwndAddContactDialog, SW_SHOW);

			}
			break;

			case ID_ExitBtn: {

				DestroyWindow(hWnd);

			}
			break;
			case ID_ContactListBox: {

				if (what == LBN_SELCHANGE)
				{
					hwndMainWindowListBox = GetDlgItem(hWnd, ID_ContactListBox);
					int selectedItem = (int)SendMessage(hwndMainWindowListBox, LB_GETCURSEL, 0, 0);
					int contactId = (int)SendMessage(hwndMainWindowListBox, LB_GETITEMDATA, selectedItem, 0);

					Contact selectedContact;
					selectedContact.contactId = -1;

					getContactById(contactId, selectedContact);

					if (selectedContact.contactId != -1)
					{
						SetDlgItemText(hWnd, ID_ShowNameTxt, (selectedContact.name).c_str());
						SetDlgItemText(hWnd, ID_ShowLastNameTxt, (selectedContact.lastName).c_str());
						SetDlgItemText(hWnd, ID_ShowPhoneNumberTxt, (selectedContact.phoneNumber).c_str());
						SetDlgItemText(hWnd, ID_ShowEmailTxt, (selectedContact.email).c_str());
						SetDlgItemText(hWnd, ID_ShowStreetTxt, (selectedContact.street).c_str());
						SetDlgItemText(hWnd, ID_ShowStreetNumberTxt, (selectedContact.streetNumber).c_str());
						SetDlgItemText(hWnd, ID_ShowBetweenStreetsTxt, (selectedContact.betweenStreets).c_str());
						SetDlgItemText(hWnd, ID_ShowCPTxt, (selectedContact.CP).c_str());
						SetDlgItemText(hWnd, ID_ShowSuburbTxt, (selectedContact.suburb).c_str());
						SetDlgItemText(hWnd, ID_ShowCityTxt, (selectedContact.city).c_str());
						SetDlgItemText(hWnd, ID_ShowStateTxt, (selectedContact.state).c_str());
						SetDlgItemText(hWnd, ID_ShowCountryTxt, (selectedContact.country).c_str());
					}

				}

			}
			break;
		
		}
	}
	break;
	case WM_DESTROY:
	{
		saveContacts(hWnd);
		PostQuitMessage(0);
	}
	break;

	default:
		break;
	}
	return 0;
}

LRESULT CALLBACK addContactWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_COMMAND:
	{
		int who = LOWORD(wParam);
		int what = HIWORD(wParam);

		switch (who)
		{
		case ID_AddContactSaveBtn: {

			addContact(hWnd);
			MessageBox(hWnd, L"Se ha agregado exitosamente el Contacto.", L"Guardado.", MB_ICONINFORMATION);
			DestroyWindow(hWnd);

		}
								   break;
		case ID_AddContactCancelBtn: {

			DestroyWindow(hWnd);

		}
									 break;
		}
	}
	break;
	case WM_DESTROY:
	{
		loadContactListBox(hwndMainWindow, ID_ContactListBox);
		ShowWindow(hwndMainWindow, SW_SHOW);
	}
	break;
	}
	return 0;
}

void getContactById(int idContactAux, Contact &contactAux) {

	aux = origin;

	while (aux != NULL)
	{
		if (aux->contactId == idContactAux)
		{
			contactAux = *aux;
			break;
		}
		aux = aux->next;
	}

}

int getContactNewId() {

	int contactNewId = 0;
	aux = origin;

	while (aux != NULL) {

		if (aux->contactId > contactNewId)
			contactNewId = aux->contactId;

		aux = aux->next;
	}

	aux = origin;

	return contactNewId + 1;
}

void addContact(HWND hWnd) {

	TCHAR buff[1024];

	aux = origin;

	int newContactId = getContactNewId();

	if (origin == NULL)
	{
		origin = new Contact;

		origin->contactId = 1;
		GetWindowText(GetDlgItem(hWnd, ID_NameTxt), buff, 1024);
		origin->name = buff;
		GetWindowText(GetDlgItem(hWnd, ID_LastNameTxt), buff, 1024);
		origin->lastName = buff;
		GetWindowText(GetDlgItem(hWnd, ID_PhoneNumberTxt), buff, 1024);
		origin->phoneNumber = buff;
		GetWindowText(GetDlgItem(hWnd, ID_EmailTxt), buff, 1024);
		origin->email = buff;
		GetWindowText(GetDlgItem(hWnd, ID_StreetTxt), buff, 1024);
		origin->street = buff;
		GetWindowText(GetDlgItem(hWnd, ID_BetweenStreetsTxt), buff, 1024);
		origin->betweenStreets = buff;
		GetWindowText(GetDlgItem(hWnd, ID_StreetNumberTxt), buff, 1024);
		origin->streetNumber = buff;
		GetWindowText(GetDlgItem(hWnd, ID_CPTxt), buff, 1024);
		origin->CP = buff;
		GetWindowText(GetDlgItem(hWnd, ID_SuburbTxt), buff, 1024);
		origin->suburb = buff;
		GetWindowText(GetDlgItem(hWnd, ID_CityTxt), buff, 1024);
		origin->city = buff;
		GetWindowText(GetDlgItem(hWnd, ID_StateTxt), buff, 1024);
		origin->state = buff;
		GetWindowText(GetDlgItem(hWnd, ID_CountryTxt), buff, 1024);
		origin->country = buff;

		origin->next = NULL;
		origin->prev = NULL;
	}
	else {

		while (aux->next != NULL)
			aux = aux->next;

		aux->next = new Contact;
		aux->next->prev = aux;
		aux = aux->next;

		aux->contactId = newContactId;
		GetWindowText(GetDlgItem(hWnd, ID_NameTxt), buff, 1024);
		aux->name = buff;
		GetWindowText(GetDlgItem(hWnd, ID_LastNameTxt), buff, 1024);
		aux->lastName = buff;
		GetWindowText(GetDlgItem(hWnd, ID_PhoneNumberTxt), buff, 1024);
		aux->phoneNumber = buff;
		GetWindowText(GetDlgItem(hWnd, ID_EmailTxt), buff, 1024);
		aux->email = buff;
		GetWindowText(GetDlgItem(hWnd, ID_StreetTxt), buff, 1024);
		aux->street = buff;
		GetWindowText(GetDlgItem(hWnd, ID_BetweenStreetsTxt), buff, 1024);
		aux->betweenStreets = buff;
		GetWindowText(GetDlgItem(hWnd, ID_StreetNumberTxt), buff, 1024);
		aux->streetNumber = buff;
		GetWindowText(GetDlgItem(hWnd, ID_CPTxt), buff, 1024);
		aux->CP = buff;
		GetWindowText(GetDlgItem(hWnd, ID_SuburbTxt), buff, 1024);
		aux->suburb = buff;
		GetWindowText(GetDlgItem(hWnd, ID_CityTxt), buff, 1024);
		aux->city = buff;
		GetWindowText(GetDlgItem(hWnd, ID_StateTxt), buff, 1024);
		aux->state = buff;
		GetWindowText(GetDlgItem(hWnd, ID_CountryTxt), buff, 1024);
		aux->country = buff;
		aux->next = NULL;

	}

	aux = origin;

}

void loadContacts() {

	aux = origin;
	fstream myFile;
	myFile.open("myContacts.bin", ios::binary | ios::in | ios::ate);
	if (myFile.is_open()) {
		int size = myFile.tellg();
		for (int i = 0; i < size / sizeof(Contact); i++)
		{
			if (origin == NULL)
			{
				origin = new Contact;
				Contact *temp = new Contact;
				myFile.seekg(0);
				myFile.read(reinterpret_cast<char*>(temp), sizeof(Contact));
				origin->contactId = temp->contactId;
				origin->name = temp->name;
				origin->lastName = temp->lastName;
				origin->phoneNumber = temp->phoneNumber;
				origin->email = temp->email;
				origin->street = temp->street;
				origin->streetNumber = temp->streetNumber;
				origin->betweenStreets = temp->betweenStreets;
				origin->CP = temp->CP;
				origin->suburb = temp->suburb;
				origin->city = temp->city;
				origin->state = temp->state;
				origin->country = temp->country;
				origin->prev = NULL;
				origin->next = NULL;
				delete reinterpret_cast<char*>(temp);
				continue;
			}
			aux = origin;

			while (aux->next != NULL)
				aux = aux->next;

			aux->next = new Contact;
			Contact *temp = new Contact;
			myFile.seekg(i * sizeof(Contact));
			myFile.read(reinterpret_cast<char*>(temp), sizeof(Contact));
			aux->next->prev = aux;
			aux->next->next = NULL;
			aux = aux->next;
			aux->contactId = temp->contactId;
			aux->name = temp->name;
			aux->lastName = temp->lastName;
			aux->phoneNumber = temp->phoneNumber;
			aux->email = temp->email;
			aux->street = temp->street;
			aux->streetNumber = temp->streetNumber;
			aux->betweenStreets = temp->betweenStreets;
			aux->CP = temp->CP;
			aux->suburb = temp->suburb;
			aux->city = temp->city;
			aux->state = temp->state;
			aux->country = temp->country;

			delete reinterpret_cast<char*>(temp);
		}
		myFile.close();
		return;
	}
	return;

}
void saveContacts(HWND hWnd) {

	aux = origin;
	fstream myFile;
	myFile.open("myContacts.bin", ios::binary | ios::trunc | ios::out);
	if (myFile.is_open())
	{
		while (aux != NULL)
		{
			myFile.write(reinterpret_cast<char *>(aux), sizeof(Contact));
			aux = aux->next;
		}
		myFile.close();
		return;
	}
	else {
		MessageBox(hWnd, L"Ocurrio un error al crear el archivo de los contactos.", L"Error", MB_ICONERROR);
	}


	return;

}

void loadContactListBox(HWND hWnd, int contactListBoxId) {

	aux = origin;
	hwndMainWindowListBox = GetDlgItem(hWnd, contactListBoxId);

	SendMessage(hwndMainWindowListBox, LB_RESETCONTENT, 0, 0);

	while (aux != NULL) {

		int pos = (int)SendMessage(hwndMainWindowListBox, LB_ADDSTRING, 0, (LPARAM)(aux->name + aux->lastName).c_str());

		SendMessage(hwndMainWindowListBox, LB_SETITEMDATA, pos, (LPARAM)aux->contactId);
		aux = aux->next;
	}

	SetFocus(hwndMainWindowListBox);

}