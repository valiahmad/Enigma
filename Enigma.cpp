//Enigma.cpp
/*
*
* Encryption Machine
* =============PROJECT============
* Copyright (c) vali ahmad rahmani
* Date Aug,3,2020
* name ENIGMA
*
*/
#include <iostream>
#include <conio.h>
#include <Windows.h>
using namespace std;
#define fast 0
#define medium 1
#define slow 2
#define Royal 18
#define Flags 6
#define Wave 23
#define Kings 11
#define Above 1
char royal[] = "Royal.dat", flags[] = "Flags.dat", wave[] = "Wave.dat",
kings[] = "Kings.dat", above[] = "Above.dat";
static int slow_ = 1, medium_ = 1, fast_ = 1;
const char Up = 'W', Down = 'S', Left = 'A', Right = 'D';
const int roll_num = 26, upside = 0, downside = 0, leftside = 41, rightside = 61;
//*********************************************************************************FUNCTIONS
void GotoXY(int x, int y)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cursorCoord;
	cursorCoord.X = x;
	cursorCoord.Y = y;
	SetConsoleCursorPosition(consoleHandle, cursorCoord);
}
//
/////////////////////////////////////////////////////////////////////SOCKET
class Socket {
	int  wire;
	int socket_name;
	friend class Plugboard;
public:
	void nomination(int _socket_name) {
		socket_name = _socket_name;
	}
	int name_socket(int code) {
		if (code == socket_name)return 0;
		else return 1;
	}
	int set_wire(int couple, int option) {
		if (option == 1)
			wire = couple;
		else if (option == -1)
			return wire;
	}
};
//////////////////////////////////////////////////////////////////////PLUGBOARD
class Plugboard {
	Socket socket[20];
	int i;
	friend class Enigma;
public:
	Plugboard() {
		for (i = 0; i < 20; i++)
			socket[i].nomination(i + 69);
	}
	int plug_coding(int word) {
		if (find_plugboard(word) == -1)return word;
		else return find_plugboard(word);
	}
	bool chek_code(int code) {
		if (code > 68 && code < 89)return true;
		else return false;
	}
	void set_plugboard(int couple1, int couple2) {
		if (!chek_code(couple1))return;
		if (!chek_code(couple2))return;
		socket[couple1 - 69].set_wire(couple2, 1);
		socket[couple2 - 69].set_wire(couple1, 1);
	}
	int find_plugboard(int code) {
		if (!chek_code(code))return -1;
		for (i = 0; i < 20; i++) {
			if (!socket[i].name_socket(code)) {
				code = socket[i].set_wire(0, -1);
				return code;
			}
		}
	}
};
////////////////////////////////////////////////////////////////////ROTOR
class Rotor {
	int i, Element_int[26], Element_char[26];
	int* connect[26][2];
	int* roll;
	friend class Rotors;
public:
	Rotor() {
		for (i = 0; i < 26; i++) {
			Element_char[i] = i + 65;
			connect[i][0] = &Element_char[i];
			connect[i][1] = &Element_int[i];
		}
	}
	void RollUp() {//must be void Roll()
		roll = connect[0][0];
		for (i = 0; i < 26; i++) {
			connect[i][0] = connect[i + 1][0];
		}
		connect[25][0] = roll;
	}
	void RollDown() {//must be void Roll()
		roll = connect[25][0];
		for (i = 25; i > 0; i--) {
			connect[i][0] = connect[i - 1][0];
		}
		connect[0][0] = roll;
	}
	int rotor_encoding(int code) {
		for (i = 0; i < 26; i++) {
			if (*connect[i][1] == code)return *connect[i][0];
		}
	}
	int rotor_decoding(int code) {
		for (i = 0; i < 26; i++) {
			if (*connect[i][0] == code)return *connect[i][1];
		}
	}
};
////////////////////////////////////////////////////////////////////////MANAGE ROTOR
class Rotors {
	Rotor rotors[3];
	int setting[3];
	int kind_fast, kind_medium, kind_slow;
	int S, M, F, set;
	friend class Enigma;
public:
	Rotors() {
		set = 1;
	}
	void Roll_Clock() {
		static int S, M, F;
		if (set)set_SMF(S, M, F);
		F = circular_counter(F);
		rotors[fast].RollUp();
		if (F == kind_fast) {
			M = circular_counter(M);
			rotors[medium].RollUp();
		}
		if (F == kind_fast && M == kind_medium) {
			S = circular_counter(S);
			rotors[slow].RollUp();
		}
	}
	/******************************************/
	void set_SMF(int& s, int& m, int& f) {
		set = 0;
		if (F > 0)f = F + 1;
		else if (F < 0)f = F + 27;
		else f = 1;

		if (M > 0)m = M + 1;
		else if (M < 0)m = M + 27;
		else m = 1;

		if (S > 0)s = S + 1;
		else if (S < 0)s = S + 27;
		else s = 1;

	}
	void set_kind(int kind, int _which) {
		switch (_which)
		{
		case fast:
			switch (kind)
			{
			case 1:kind_fast = Royal; break;
			case 2:kind_fast = Flags; break;
			case 3:kind_fast = Wave; break;
			case 4:kind_fast = Kings; break;
			case 5:kind_fast = Above; break;
			default:
				break;
			}
			break;
		case medium:
			switch (kind)
			{
			case 1:kind_medium = Royal; break;
			case 2:kind_medium = Flags; break;
			case 3:kind_medium = Wave; break;
			case 4:kind_medium = Kings; break;
			case 5:kind_medium = Above; break;
			default:
				break;
			}
			break;
		case slow:
			switch (kind)
			{
			case 1:kind_slow = Royal; break;
			case 2:kind_slow = Flags; break;
			case 3:kind_slow = Wave; break;
			case 4:kind_slow = Kings; break;
			case 5:kind_slow = Above; break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
	void set_rotors() {
		cout << "\nChoose Kind Of Rotors : #1.Royal(I) 2#.Flags(II) #3.Wave(III) #4.Kings(IV) #5.Above(V)";
		cout << "\nFor Fast : "; set_kind(_getche() - 48, fast);
		cout << "\nFor Medium : "; set_kind(_getche() - 48, medium);
		cout << "\nFor Slow : "; set_kind(_getche() - 48, slow);
		file();

		int p;
		while (true) {
			system("cls");
			print(1);
			p = _getch();

			if (p == 'Q') { rotors[slow].RollUp(); S++; }
			if (p == 'A') { rotors[slow].RollDown(); S--; }

			if (p == 'W') { rotors[medium].RollUp(); M++; }
			if (p == 'S') { rotors[medium].RollDown(); M--; }

			if (p == 'E') { rotors[fast].RollUp(); F++; }
			if (p == 'D') { rotors[fast].RollDown(); F--; }

			if (p == 27)break;
		}
		setting[fast] = *rotors[fast].connect[0][0];

		setting[medium] = *rotors[medium].connect[0][0];

		setting[slow] = *rotors[slow].connect[0][0];
	}
	int reflector(int t) {//COMPLETED
		if (t == 'A')return 69; if (t == 'H')return 88; if (t == 'O')return 81; if (t == 'U')return 80;
		if (t == 'B')return 74; if (t == 'I')return 86; if (t == 'P')return 85; if (t == 'V')return 73;
		if (t == 'C')return 77; if (t == 'J')return 66; if (t == 'Q')return 79; if (t == 'W')return 75;
		if (t == 'D')return 90; if (t == 'K')return 87; if (t == 'R')return 78; if (t == 'X')return 72;
		if (t == 'E')return 65; if (t == 'L')return 70; if (t == 'S')return 84; if (t == 'Y')return 71;
		if (t == 'F')return 76; if (t == 'M')return 67; if (t == 'T')return 83; if (t == 'Z')return 68;
		if (t == 'G')return 89; if (t == 'N')return 82;
	}
	int rotors_coding(int woco_code) {//COMPLETED
		int tcode;
		tcode = rotors[slow].rotor_encoding(rotors[medium].rotor_encoding(rotors[fast].rotor_encoding(woco_code)));
		tcode = reflector(tcode);
		return rotors[fast].rotor_decoding(rotors[medium].rotor_decoding(rotors[slow].rotor_decoding(tcode)));
	}
	int circular_counter(int num) {
		if (num == 26)return 1;
		else return ++num;
	}
	void file() {
		char ch;
		char* add = 0;
		FILE* file;
		//read for fast rotor
		set_add(fast, add);
		fopen_s(&file, add, "r");
		for (int i = 0; i < 26; i++) {
			ch = getc(file);
			rotors[fast].Element_int[i] = (int)ch;
		}
		//read for medium rotor
		set_add(medium, add);
		fopen_s(&file, add, "r");
		for (int i = 0; i < 26; i++) {
			ch = getc(file);
			rotors[medium].Element_int[i] = (int)ch;
		}
		//read for slow rotor
		set_add(slow, add);
		fopen_s(&file, add, "r");
		for (int i = 0; i < 26; i++) {
			ch = getc(file);
			rotors[slow].Element_int[i] = (int)ch;
		}
		fclose(file);
	}
	void set_add(int kind, char* (&add)) {
		//set address
		switch (kind)
		{
			//set address for fast rotor
		case fast:
			switch (kind_fast)
			{
			case 18:add = royal; break;
			case 6:add = flags; break;
			case 23:add = wave; break;
			case 11:add = kings; break;
			case 1:add = above; break;
			default:
				break;
			}
			break;
			//set address for medium rotor
		case medium:
			switch (kind_medium)
			{
			case 18:add = royal; break;
			case 6:add = flags; break;
			case 23:add = wave; break;
			case 11:add = kings; break;
			case 1:add = above; break;
			default:
				break;
			}
			break;
			//set address for medium rotor
		case slow:
			switch (kind_slow)
			{
			case 18:add = royal; break;
			case 6:add = flags; break;
			case 23:add = wave; break;
			case 11:add = kings; break;
			case 1:add = above; break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
	void print(int key) {
		GotoXY(0, 12);
		cout << "SLOW\t\tMEDIUM\t\tFAST\n";
		for (int k = 0; k < 5; k++) {
			cout << " " << *rotors[slow].connect[k][0] - 64;
			cout << "\t\t" << *rotors[medium].connect[k][0] - 64;
			cout << "\t\t" << *rotors[fast].connect[k][0] - 64 << "\n";
		}
		GotoXY(0, 15); cout << "|";
		GotoXY(3, 15); cout << "|";
		GotoXY(15, 15); cout << "|";
		GotoXY(18, 15); cout << "|";
		GotoXY(31, 15); cout << "|";
		GotoXY(34, 15); cout << "|";
		if (key) {
			GotoXY(50, 10); cout << "For Move Up The Slow Press     : Q ";
			GotoXY(50, 11); cout << "For Move Down The Slow Press   : A ";
			GotoXY(50, 12); cout << "For Move Up The Medium Press   : W ";
			GotoXY(50, 13); cout << "For Move Down The Medium Press : S ";
			GotoXY(50, 14); cout << "For Move Up The Fast Press     : E ";
			GotoXY(50, 15); cout << "For Move Down The Fast Press   : D ";
			GotoXY(50, 16); cout << "For Go To Next Setting Press \"esc\" ";
		}
	}
	void reset() {
		for (int i = 0; i < 26; i++) {
			if (setting[fast] == *rotors[fast].connect[0][0])
				break;
			rotors[fast].RollUp();
		}
		for (int i = 0; i < 26; i++) {
			if (setting[medium] == *rotors[medium].connect[0][0])
				break;
			rotors[medium].RollUp();
		}
		for (int i = 0; i < 26; i++) {
			if (setting[slow] == *rotors[slow].connect[0][0])
				break;
			rotors[slow].RollUp();
		}
		set = 1;
	}
};
/////////////////////////////////////////////////////////////////////////////ENIGMA
class Enigma {
	Rotors rotors;
	Plugboard plugboard;
	int Repeat[20];
public:
	Enigma() {//setting Enigma
		cout << "\nTURN ON THE CAPS LOCK AND SET THE ENIGMA \nPRESS ENTER TO CONTINUE " << endl;
		_getch();
		system("cls");
		rotors.set_rotors();
		set_plugboard();
	}
	int Code_Go(int woco) {//CODE GO
		woco = plugboard.plug_coding(rotors.rotors_coding(plugboard.plug_coding(woco)));
		rotors.Roll_Clock();
		return woco;
	}
	void set_plugboard() {
		int A, B;
	loop:
		system("cls");
		cout << "\nSET PLUGBOARD " << endl;
		cout << "Explain : You Just Can Combine Two Characters Togather! (E-W) " << endl;
		for (int i = 0; i < 10; i++) {
			//GotoXY(8 * i, 10);
			cout << "<";
			A = _getche();
			if (!(A > 68 && A < 89)) {
				cout << "\nYou Entered A Wrong Character!\nPress Any Key To Set Again"; _getch(); goto loop;
			}
			cout << ",";
			B = _getche();
			if (!(B > 68 && B < 89)) {
				cout << "\nYou Entered A Wrong Character!\nPress Any Key To Set Again"; _getch(); goto loop;
			}
			cout << ">   ";
			for (int j = 0; j < i * 2; j++) {
				if (A == Repeat[j]) {
					cout << "\nYou Entered A Wrong Character!\nPress Any Key To Set Again"; _getch(); goto loop;
				}
				if (B == Repeat[j]) {
					cout << "\nYou Entered A Wrong Character!\nPress Any Key To Set Again"; _getch(); goto loop;
				}
			}
			Repeat[i * 2] = A;
			Repeat[i * 2 + 1] = B;
			plugboard.set_plugboard(A, B);
		}
	}
	void reset() {
		rotors.reset();
		system("cls");
		cout << "Your Settings Is : ";
		rotors.print(0);
	}
};
/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
int main()
{
	Enigma enigma;
loop:
	system("cls");
	cout << "\nFOR RESET SETTING PRESS 0\nFOR EXIT PRESS 1\nWrite Code:\n";
	int a, i = 0;
	while (true) {
		GotoXY(i, 5);
		a = _getche();
		GotoXY(i, 6);
		if (a >= 65 && a <= 90) {
			cout << (char)enigma.Code_Go(a);
			i++;
		}
		else cout << (char)32;
		if (a == 49)break;
		if (a == 48) {
			enigma.reset();
			cout << "\n\n\n\nRESET DONE!\nPress Any Key To Continue";
			_getch();
			goto loop;
		}
	}

	return 0;
}