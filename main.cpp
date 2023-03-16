#include <iostream>
#include <Windows.h>
#include <stack>
#include <vector>

using namespace std;

const int width = 130;
const int height = 40;

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

SMALL_RECT s = { 0, 0, 1, 1 };

INPUT_RECORD in[32];
DWORD events = 0;

CHAR_INFO* screen = new CHAR_INFO[width * height];
vector<CHAR_INFO> temp;
stack<vector<CHAR_INFO> > z;

int mouse_x;
int mouse_y;

enum Color { MAVI = 1, YESIL, TURKUAZ, KIRMIZI, MOR, SARI, BEYAZ, GRI, ACIKMAVI, ACIKYESIL, ACIKTURKUAZ, ACIKKIRMIZI, ACIKMOR, ACIKSARI, ACIKBEYAZ };
Color color = BEYAZ;

bool isPressedL;
bool isPressedR;

void curserColor() {
	if (mouse_x < 70 && mouse_y < 1) {
		int temp = mouse_x / 5;
		switch (temp)
		{
		case 0:
			color = MAVI;
			break;
		case 1:
			color = YESIL;
			break;
		case 2:
			color = TURKUAZ;
			break;
		case 3:
			color = KIRMIZI;
			break;
		case 4:
			color = MOR;
			break;
		case 5:
			color = SARI;
			break;
		case 6:
			color = BEYAZ;
			break;
		case 7:
			color = GRI;
			break;
		case 8:
			color = ACIKMAVI;
			break;
		case 9:
			color = ACIKYESIL;
			break;
		case 10:
			color = ACIKTURKUAZ;
			break;
		case 11:
			color = ACIKKIRMIZI;
			break;
		case 12:
			color = ACIKMOR;
			break;
		case 13:
			color = ACIKSARI;
			break;
		case 14:
			color = ACIKBEYAZ;
			break;
		}
	}
	//cout << "x: " << mouse_x << "y: " << mouse_y << endl;
}
void Screen() {
	int n = 0;
	// þimdi bütün konsolun icini doldurucaz
	for (int i = 0; i < width * height; i++) {
		screen[i].Char.UnicodeChar = 0x2588;
		// Katolog
		if (i < 70) {
			if (i % 5 == 0)
				n++;
			screen[i].Attributes = n;
		}
		else
			screen[i].Attributes = 0x0000;

		screen[71].Char.UnicodeChar = 0x0053;
		screen[72].Char.UnicodeChar = 0x0050;
		screen[73].Char.UnicodeChar = 0x0041;
		screen[74].Char.UnicodeChar = 0x0043;
		screen[75].Char.UnicodeChar = 0x0045;

		screen[77].Char.UnicodeChar = 0x207C;

		screen[79].Char.UnicodeChar = 0x0043;
		screen[80].Char.UnicodeChar = 0x004C;
		screen[81].Char.UnicodeChar = 0x0045;
		screen[82].Char.UnicodeChar = 0x0041;
		screen[83].Char.UnicodeChar = 0x0052;

		if (screen[i].Char.UnicodeChar != 0x2588)
			screen[i].Attributes = 0x0007;
	}
}
void Setup() {
	mouse_x = 0;
	mouse_y = 0;

	// Konsol cursor ayarlarý
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hOut, &cursorInfo);
	cursorInfo.bVisible = false; // set the cursor visibility
	SetConsoleCursorInfo(hOut, &cursorInfo);

	// Konsol modlarý ayarý
	SetConsoleMode(hIn, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT);

	// Konsol büyüklük ayarlarý | Dipnot: screen buffer window size dan kücük olamýyor.Window sizde dascreen buffer dan büyük olamaz.
	SetConsoleActiveScreenBuffer(hOut); // ekraný seciyor
	COORD buffer = { width, height };
	SetConsoleScreenBufferSize(hOut, buffer); // ara belleðin boyutunu ayarlýyor

	s = { 0, 0, short(width - 1), short(height - 1) };
	SetConsoleWindowInfo(hOut, TRUE, &s); // pencerenin boyutunu ayarlýyor

	// þimdi bütün konsolun icini doldurucaz
	Screen();

	for (int i = 0; i < width * height; i++) {

		temp.push_back(screen[i]);
	}

	z.push(temp);
}
void Input() {
	GetNumberOfConsoleInputEvents(hIn, &events);

	if (events > 0)
		ReadConsoleInput(hIn, in, events, &events);
}
void Logic() {
	for (DWORD i = 0; i < events; i++) {

		if (in[i].Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {
			mouse_x = in[i].Event.MouseEvent.dwMousePosition.X;
			mouse_y = in[i].Event.MouseEvent.dwMousePosition.Y;
			curserColor();
		}

		if (in[i].Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
			screen[mouse_y * width + mouse_x].Attributes = color;
			isPressedL = true;
		}
		else if (isPressedL) {
			for (int i = 0; i < width * height; i++) {

				temp[i] = screen[i];
			}

			z.push(temp);
			isPressedL = false;
		}

		if (in[i].Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
			screen[mouse_y * width + mouse_x].Attributes = 0x0000;
			isPressedR = true;
		}
		else if (isPressedR) {
			for (int i = 0; i < width * height; i++) {

				temp[i] = screen[i];
			}

			z.push(temp);
			isPressedR = false;
		}

		if (GetAsyncKeyState(VK_SPACE)) {
			Screen();
			for (int i = 0; i < z.size()-1; i++) {
				z.pop();
			}
		}

		if (GetAsyncKeyState(0x11) && GetAsyncKeyState(0x5A) && z.size() > 1) {
			z.pop();
			for (int i = 0; i < width * height; i++) {
				screen[i].Char.UnicodeChar = z.top()[i].Char.UnicodeChar;
				screen[i].Attributes = z.top()[i].Attributes;
			}
			Sleep(120);
		}
	}
}



int main() {
	Setup();
	while (1) {
		Input();
		Logic();
		//cout << z.size() << endl;
		WriteConsoleOutput(hOut, screen, { short(width), short(height) }, { 0, 0 }, &s);
	}
	return 0;
}