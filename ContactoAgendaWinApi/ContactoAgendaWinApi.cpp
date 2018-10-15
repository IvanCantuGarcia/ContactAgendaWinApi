#include "stdafx.h"
#include "Contact.h"
#include "ContactoAgendaWinApi.h"
#include <fstream>
#include <commdlg.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;

HWND hwndMainWindow;
HWND hwndMainWindowListBox;

LRESULT CALLBACK mainWindowProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK addContactWindowProc(HWND, UINT, WPARAM, LPARAM);

int validateAddWindowFields(HWND);
int getContactNewId();
Contact* getContactById(int);
void addContact(HWND);
void deleteContact(HWND);
void saveContacts(HWND);
void loadContacts();

void loadContactListBox(HWND, int);
void cleanShowContactFields(HWND);

Contact *origin, *aux;

wstring fileLocation[2];

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
			case ID_DeleteContactBtn: {

				hwndMainWindowListBox = GetDlgItem(hWnd, ID_ContactListBox);
				deleteContact(hwndMainWindowListBox);
				loadContactListBox(hwndMainWindow, ID_ContactListBox);
				cleanShowContactFields(hwndMainWindow);

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

					Contact* selectedContact = NULL;

					selectedContact = getContactById(contactId);

					if (selectedContact != NULL)
					{
						SetDlgItemText(hWnd, ID_ShowNameTxt, (selectedContact->name).c_str());
						SetDlgItemText(hWnd, ID_ShowLastNameTxt, (selectedContact->lastName).c_str());
						SetDlgItemText(hWnd, ID_ShowPhoneNumberTxt, (selectedContact->phoneNumber).c_str());
						SetDlgItemText(hWnd, ID_ShowEmailTxt, (selectedContact->email).c_str());
						SetDlgItemText(hWnd, ID_ShowStreetTxt, (selectedContact->street).c_str());
						SetDlgItemText(hWnd, ID_ShowStreetNumberTxt, (selectedContact->streetNumber).c_str());
						SetDlgItemText(hWnd, ID_ShowBetweenStreetsTxt, (selectedContact->betweenStreets).c_str());
						SetDlgItemText(hWnd, ID_ShowCPTxt, (selectedContact->CP).c_str());
						SetDlgItemText(hWnd, ID_ShowSuburbTxt, (selectedContact->suburb).c_str());
						SetDlgItemText(hWnd, ID_ShowCityTxt, (selectedContact->city).c_str());
						SetDlgItemText(hWnd, ID_ShowStateTxt, (selectedContact->state).c_str());
						SetDlgItemText(hWnd, ID_ShowCountryTxt, (selectedContact->country).c_str());

						HBITMAP bmp, bmp2;
						bmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), (selectedContact->imgPath[0].c_str()), IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
						SendDlgItemMessage(hWnd, ID_ShowcContactImage1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

						bmp2 = (HBITMAP)LoadImage(GetModuleHandle(NULL), (selectedContact->imgPath[1].c_str()), IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
						SendDlgItemMessage(hWnd, ID_ShowcContactImage2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);
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

		}
		break;
		case ID_AddContactCancelBtn: {

			DestroyWindow(hWnd);

		}
		break;
		case ID_ContactImage1Btn: {
			OPENFILENAME ofn;
			char szFileName[MAX_PATH] = "";
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = L"24 bit Bitmap(.bmp)\0*.bmp\0 16 bit Bitmap(.bmp)\0*.bmp\0 8 bit Bitmap(.bmp)\0*.bmp\0";

			ofn.lpstrFile = (LPWSTR)szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = L"bmp";
			if (GetOpenFileName(&ofn) == TRUE)
			{
				fileLocation[0] = ofn.lpstrFile;
				HBITMAP bmp;
				bmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), ofn.lpstrFile, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				SendDlgItemMessage(hWnd, ID_AddImage1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

			}
			else
				MessageBox(0, L"No eligio archivo", L"Aviso", MB_OK | MB_ICONINFORMATION);

		}
		break;
		case ID_ContactImage2Btn: {
			OPENFILENAME ofn;
			char szFileName[MAX_PATH] = "";
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = L"24 bit Bitmap(.bmp)\0*.bmp\0 16 bit Bitmap(.bmp)\0*.bmp\0 8 bit Bitmap(.bmp)\0*.bmp\0";

			ofn.lpstrFile = (LPWSTR)szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = L"bmp";
			if (GetOpenFileName(&ofn) == TRUE)
			{
				fileLocation[1] = ofn.lpstrFile;
				HBITMAP bmp;
				bmp = (HBITMAP)LoadImage(GetModuleHandle(NULL), ofn.lpstrFile, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				SendDlgItemMessage(hWnd, ID_AddImage2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
			}
			else
				MessageBox(0, L"No eligio archivo", L"Aviso", MB_OK | MB_ICONINFORMATION);

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

int validateAddWindowFields(HWND hWnd) {

	TCHAR valName[1024];
	TCHAR valLastName[1024];
	TCHAR valPhoneNumber[1024];
	TCHAR valEmail[1024];
	TCHAR valStreet[1024];
	TCHAR valBetweenStreets[1024];
	TCHAR valStreetNumber[1024];
	TCHAR valCP[1024];
	TCHAR valSuburb[1024];
	TCHAR valCity[1024];
	TCHAR valState[1024];
	TCHAR valCountry[1024];

	GetWindowText(GetDlgItem(hWnd, ID_NameTxt), valName, 1024);
	GetWindowText(GetDlgItem(hWnd, ID_LastNameTxt),valLastName, 1024);
	GetWindowText(GetDlgItem(hWnd, ID_PhoneNumberTxt), valPhoneNumber, 1024);
	GetWindowText(GetDlgItem(hWnd, ID_EmailTxt), valEmail, 1024);
	GetWindowText(GetDlgItem(hWnd, ID_StreetTxt), valStreet, 1024);
	GetWindowText(GetDlgItem(hWnd, ID_BetweenStreetsTxt), valBetweenStreets, 1024);
	GetWindowText(GetDlgItem(hWnd, ID_StreetNumberTxt), valStreetNumber, 1024);
	GetWindowText(GetDlgItem(hWnd, ID_CPTxt), valCP, 1024);
	GetWindowText(GetDlgItem(hWnd, ID_SuburbTxt), valSuburb, 1024);
	GetWindowText(GetDlgItem(hWnd, ID_CityTxt), valCity, 1024);
	GetWindowText(GetDlgItem(hWnd, ID_StateTxt), valState, 1024);
	GetWindowText(GetDlgItem(hWnd, ID_CountryTxt), valCountry, 1024);

	if (!valName || GetWindowTextLengthA(GetDlgItem(hWnd, ID_NameTxt)) <= 0 || valName == L""){
		MessageBox(hWnd, L"Ingrese un Nombre de contacto.", L"Error en creacion de contacto.", MB_ICONINFORMATION);
		return 0;
	}
	else if (!valLastName || GetWindowTextLengthA(GetDlgItem(hWnd, ID_LastNameTxt)) <= 0 || valLastName == L"") {
		MessageBox(hWnd, L"Ingrese un Apellido de contacto.", L"Error en creacion de contacto.", MB_ICONINFORMATION);
		return 0;
	}
	else if (!valPhoneNumber || GetWindowTextLengthA(GetDlgItem(hWnd, ID_PhoneNumberTxt)) <= 0 || valPhoneNumber == L"") {
		MessageBox(hWnd, L"Ingrese un Telefono de contacto.", L"Error en creacion de contacto.", MB_ICONINFORMATION);
		return 0;
	}
	else if (!valEmail || GetWindowTextLengthA(GetDlgItem(hWnd, ID_EmailTxt)) <= 0 || valEmail == L"") {
		MessageBox(hWnd, L"Ingrese un Email de contacto.", L"Error en creacion de contacto.", MB_ICONINFORMATION);
		return 0;
	}
	else if (!valStreet || GetWindowTextLengthA(GetDlgItem(hWnd, ID_StreetTxt)) <= 0 || valStreet == L"") {
		MessageBox(hWnd, L"Ingrese una Calle de contacto.", L"Error en creacion de contacto.", MB_ICONINFORMATION);
		return 0;
	}
	else if (!valStreetNumber || GetWindowTextLengthA(GetDlgItem(hWnd, ID_StreetNumberTxt)) <= 0 || valStreetNumber == L"") {
		MessageBox(hWnd, L"Ingrese un Numero de Calle de contacto.", L"Error en creacion de contacto.", MB_ICONINFORMATION);
		return 0;
	}
	else if (!valBetweenStreets || GetWindowTextLengthA(GetDlgItem(hWnd, ID_BetweenStreetsTxt)) <= 0 || valBetweenStreets == L"") {
		MessageBox(hWnd, L"Ingrese Entre calles de contacto.", L"Error en creacion de contacto.", MB_ICONINFORMATION);
		return 0;
	}
	else if (!valCP || GetWindowTextLengthA(GetDlgItem(hWnd, ID_CPTxt)) <= 0 || valCP == L"") {
		MessageBox(hWnd, L"Ingrese el Codigo Postal de contacto.", L"Error en creacion de contacto.", MB_ICONINFORMATION);
		return 0;
	}
	else if (!valSuburb || GetWindowTextLengthA(GetDlgItem(hWnd, ID_SuburbTxt)) <= 0 || valSuburb == L"") {
		MessageBox(hWnd, L"Ingrese la Colonia de contacto.", L"Error en creacion de contacto.", MB_ICONINFORMATION);
		return 0;
	}
	else if (!valCity || GetWindowTextLengthA(GetDlgItem(hWnd, ID_CityTxt)) <= 0 || valCity == L"") {
		MessageBox(hWnd, L"Ingrese la Ciudad de contacto.", L"Error en creacion de contacto.", MB_ICONINFORMATION);
		return 0;
	}
	else if (!valState || GetWindowTextLengthA(GetDlgItem(hWnd, ID_StateTxt)) <= 0 || valState == L"") {
		MessageBox(hWnd, L"Ingrese el Estado de contacto.", L"Error en creacion de contacto.", MB_ICONINFORMATION);
		return 0;
	}
	else if (!valCountry || GetWindowTextLengthA(GetDlgItem(hWnd, ID_CountryTxt)) <= 0 || valCountry == L"") {
		MessageBox(hWnd, L"Ingrese el Pais de contacto.", L"Error en creacion de contacto.", MB_ICONINFORMATION);
		return 0;
	}
	else if ( fileLocation[0].size() <= 0 || fileLocation[0] == L"") {
		MessageBox(hWnd, L"Ingrese la Imagen 1 de contacto.", L"Error en creacion de contacto.", MB_ICONINFORMATION);
		return 0;
	}
	else if (fileLocation[1].size() <= 0 || fileLocation[1] == L"") {
		MessageBox(hWnd, L"Ingrese la Imagen 2 de contacto.", L"Error en creacion de contacto.", MB_ICONINFORMATION);
		return 0;
	}

	return 1;

}

Contact* getContactById(int idContactAux) {

	aux = origin;

	while (aux != NULL)
	{
		if (aux->contactId == idContactAux)
		{
			return aux;
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

	if (validateAddWindowFields(hWnd) == 0)
	{
		return;
	}

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
		origin->imgPath[0] = fileLocation[0];
		origin->imgPath[1] = fileLocation[1];

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
		aux->imgPath[0] = fileLocation[0];
		aux->imgPath[1] = fileLocation[1];	

		aux->next = NULL;

	}

	aux = origin;
	fileLocation[0] = fileLocation[1] = L"";
	MessageBox(hWnd, L"Se ha agregado exitosamente el Contacto.", L"Guardado.", MB_ICONINFORMATION);
	DestroyWindow(hWnd);

}

void deleteContact(HWND hWnd) {

	int selectedItem = (int)SendMessage(hWnd, LB_GETCURSEL, 0, 0);
	int contactId = (int)SendMessage(hWnd, LB_GETITEMDATA, selectedItem, 0);

	if (selectedItem == -1)
	{
		MessageBox(NULL, L"Debe seleccionar un contacto para así borrarlo.", L"Eliminar contacto.", MB_ICONEXCLAMATION);
		return;
	}

	aux = getContactById(contactId);

	const int result = MessageBox(NULL, L"Desea borrar el contacto?", L"Eliminar contacto.", MB_YESNOCANCEL);

	if (result == IDYES) {

		if (aux->prev == NULL && aux->next == NULL)
		{
			delete aux;
			aux = origin = NULL;
		}
		else if (aux->prev == NULL)
		{
			origin = aux->next;
			delete aux;
			origin->prev = NULL;
			aux = origin;
		}
		else if (aux-> next == NULL)
		{
			aux->prev->next = NULL;
			delete aux;
			aux = origin;
		}
		else
		{
			aux->prev->next = aux->next;
			aux->next->prev = aux->prev;
			delete aux;
			aux = origin;
		}
		
	}
	else {
		return;
	}

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

void cleanShowContactFields(HWND hWnd) {

	SetDlgItemText(hWnd, ID_ShowNameTxt, L"");
	SetDlgItemText(hWnd, ID_ShowLastNameTxt, L"");
	SetDlgItemText(hWnd, ID_ShowPhoneNumberTxt, L"");
	SetDlgItemText(hWnd, ID_ShowEmailTxt, L"");
	SetDlgItemText(hWnd, ID_ShowStreetTxt, L"");
	SetDlgItemText(hWnd, ID_ShowStreetNumberTxt, L"");
	SetDlgItemText(hWnd, ID_ShowBetweenStreetsTxt, L"");
	SetDlgItemText(hWnd, ID_ShowCPTxt, L"");
	SetDlgItemText(hWnd, ID_ShowSuburbTxt, L"");
	SetDlgItemText(hWnd, ID_ShowCityTxt, L"");
	SetDlgItemText(hWnd, ID_ShowStateTxt, L"");
	SetDlgItemText(hWnd, ID_ShowCountryTxt, L"");

}