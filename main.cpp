#include <iostream>
#include <Windows.h>

using namespace std;

const int width=130;
const int height=40;
INPUT input[2];
POINT mouse;

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);

SMALL_RECT s = { 0, 0, 1, 1 };
CHAR_INFO* screen = new CHAR_INFO[width * height];

INPUT_RECORD in[32];
DWORD events = 0;

int mouse_x;
int mouse_y;

enum Color{MAVI=1,YESIL,TURKUAZ,KIRMIZI,MOR,SARI,BEYAZ,GRI,ACIKMAVI};
Color color;

void curserColor() {
	if (mouse_x < 45 && mouse_y < 1) {
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
		}
	}

	//cout << "x: " << mouse_x << "y: " << mouse_y << endl;
}
void Screen() {
	// þimdi bütün konsolun icini doldurucaz
	for (int i = 0; i < width * height; i++) {
		screen[i].Char.UnicodeChar = 0x2588;
		if (i < 5)
			screen[i].Attributes = 0x0001; // MAVÝ
		else if (i < 10)
			screen[i].Attributes = 0x0002; // YESIL
		else if (i < 15)
			screen[i].Attributes = 0x0003; // TURKUAZ
		else if (i < 20)
			screen[i].Attributes = 0x0004; // KIRMIZI 
		else if (i < 25)
			screen[i].Attributes = 0x0005; // MOR
		else if (i < 30)
			screen[i].Attributes = 0x0006; // SARI
		else if (i < 35)
			screen[i].Attributes = 0x0007; // BEYAZ
		else if (i < 40)
			screen[i].Attributes = 0x0008; // GRÝ
		else if (i < 45)
			screen[i].Attributes = 0x0009; // ACIK MAVI
		else
			screen[i].Attributes = 0x0000;
	}
}
void Setup() {

	mouse_x = 0;
	mouse_y = 0;
	color = BEYAZ;

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

	s = { 0, 0, short(width-1), short(height-1) };
	SetConsoleWindowInfo(hOut, TRUE, &s); // pencerenin boyutunu ayarlýyor

	// þimdi bütün konsolun icini doldurucaz
	Screen();
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
		}

		else if (in[i].Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
			screen[mouse_y * width + mouse_x].Attributes = 0x0000;
		}

		if (GetAsyncKeyState(VK_SPACE)) {	
			Screen();
		}
	}
}


int main() {
	Setup();
	while (1) {
		Input();
		Logic();
		WriteConsoleOutput(hOut, screen, { short(width), short(height) }, { 0, 0 }, &s);
	}
	return 0;
}