#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include<conio.h>
#include<ctime>
#include<time.h>
#include<fstream>
#include<windows.graphics.h>
#define UP 1
#define Down 2
#define Left 3
#define Right 4
int const Vertical_porus_end = 35;
int const Vertical_porus_strt =12;
int const inside_vert_porus_end = 26;
int const inside_vert_porus_strt = 20;
int const inside_horil_porus_strt = 20;
int const inside_horil_porus_end = 25;
int const inside_horiR_porus_strt = 55;
int const inside_horiR_porus_end = 60;
int const updwn_hori_porus_strt = 15;
int const updwn_hori_porus_end = 70;
int const sml_vert_porus_strt = 3;
int const sml_vert_porus_end = 6;
int const inside_hori_porus_strt = 30;
int const inside_hori_porus_end = 50;
using namespace std;
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
struct position
{
	int ri, ci;
};
struct snake
{
	position* Ps;
	char Ssym;
	char Shsym;
	int size;
	int direction;
	int score;
	int rows;
	int cols;
	int Rkey, Lkey, Upkey, Downkey;
};
void snakeinit(snake& S,ifstream& rdr, int rows, int cols)
{
	char a;
	int b;
	rdr>>S.direction;
	rdr >> S.size ;
	rdr >> S.score ;
	rdr >> b;
	S.Ssym = b;
	rdr >> b;
	S.Shsym = b;
	rdr >> a;
    S.Lkey=a ;
	rdr >> a;
    S.Downkey=a ;
	rdr >> a;
    S.Upkey=a ;
	rdr >> a;
	S.Rkey=a ;
	S.Ps = new position[S.size];
	rdr >> S.rows;
	rdr >> S.cols;
	for (int i = 0; i < S.size; i++)
	{
		S.Ps[i].ri = S.rows / 2;
		S.Ps[i].ci = S.cols / 2 - i;
	}
}
void init(snake& S,ifstream& rdr, int rows, int cols, position& fpos, char& fsym)
{
	snakeinit(S,rdr, rows, cols);
	fpos.ri = 10;
	fpos.ci = 10;
	fsym = 'O';
}
void display_snake(snake& S)
{
	gotoRowCol(S.Ps[0].ri, S.Ps[0].ci);
	cout << S.Shsym;
	for (int i = 01; i < S.size; i++)
	{
		gotoRowCol(S.Ps[i].ri, S.Ps[i].ci);
		cout << S.Ssym;
	}
}
void erase_snake(snake& S)
{
	gotoRowCol(S.Ps[0].ri, S.Ps[0].ci);
	cout << " ";
	for (int i = 01; i < S.size; i++)
	{
		gotoRowCol(S.Ps[i].ri, S.Ps[i].ci);
		cout << " ";
	}
}
void change_direction(char ch, snake& S)
{
	if ((ch == S.Rkey) && !(S.direction == Left))
		S.direction = Right;
	if ((ch == S.Lkey) && !(S.direction == Right))
		S.direction = Left;
	if ((ch == S.Upkey) && !(S.direction == Down))
		S.direction = UP;
	if ((ch == S.Downkey) && !(S.direction == UP))
		S.direction = Down;

}
void snakemove(snake& S, int row, int cols)
{
	for (int i = S.size - 1; i > 0; i--)
		S.Ps[i] = S.Ps[i - 1];
	switch (S.direction)
	{
	case UP:
		S.Ps[0].ri--;
		if (S.Ps[0].ri == -1)
			S.Ps[0].ri = row - 1;
		break;
	case Down:
		S.Ps[0].ri++;
		if (S.Ps[0].ri == row)
			S.Ps[0].ri = 0;
		break;
	case Left:
		S.Ps[0].ci--;
		if (S.Ps[0].ci == -1)
			S.Ps[0].ci = cols - 1;
		break;
	case Right:
		S.Ps[0].ci++;
		if (S.Ps[0].ci == cols)
			S.Ps[0].ci = 0;
		break;
	}
}
bool foodeat(snake& S, position fpos)
{
	return S.Ps[0].ri == fpos.ri && S.Ps[0].ci == fpos.ci;
}
void snakeexpansion(snake& S)
{
	position* HA = new position[S.size + 1];
	for (int i = 0; i < S.size; i++)
		HA[i] = S.Ps[i];
	HA[S.size] = HA[S.size - 1];
	delete[] S.Ps;
	S.Ps = HA;
	S.size++;
	S.score++;
}
void specialsnakeexpansion(snake& S)
{
	position* HA = new position[S.size + 1];
	for (int i = 0; i < S.size; i++)
		HA[i] = S.Ps[i];
	HA[S.size] = HA[S.size - 1];
	delete[] S.Ps;
	S.Ps = HA;
	S.size++;
	S.score+=10;
}
void damagefoodexpansion(snake& S)
{
	position* HA = new position[S.size + 1];
	for (int i = 0; i < S.size; i++)
		HA[i] = S.Ps[i];
	HA[S.size] = HA[S.size - 1];
	delete[] S.Ps;
	S.Ps = HA;
	S.size++;
	S.score -= 10;
}
void displayfood(position fpos, char fsym)
{
	gotoRowCol(fpos.ri, fpos.ci);
	cout << fsym;
}
bool snaketouchitself(snake* S,int num_of_plyr)
{
	for (int index = 0; index < num_of_plyr; index++)
	{
		for (int i = 1; i < S[index].size; i++)
		{
			if (S[index].Ps[0].ri == S[index].Ps[i].ri && S[index].Ps[0].ci == S[index].Ps[i].ci)
			{
				return true;
			}
		}
	}
	return false;
}
bool snaketouchsidewall(snake* S,int cols,int num_of_plyr)
{
	for (int index = 0; index < num_of_plyr; index++)
	{
		for (int i = 1; i < cols; i++)
		{
			if ((S[index].Ps[0].ri == i && S[index].Ps[0].ci == 0) || (S[index].Ps[0].ri == 0 && S[index].Ps[0].ci == i) || (S[index].Ps[0].ri == i && S[index].Ps[0].ci == 99) || (S[index].Ps[0].ri == 39 && S[index].Ps[0].ci == i))
			{
				return true;
			}
		}
	}
	return false;
}
bool snakeheadcollision(snake* S)
{
	for (int i = 0; i < 2; i++)
	{
		if (S[1].Ps[0].ri == S[0].Ps[0].ri && S[1].Ps[0].ci == S[0].Ps[0].ci)
		{
			return true;
		}
	}
	return false;
}
void snake_tailcollide(snake* S)
{
	for (int i = 1; i < S[0].size; i++)
	{
		if (S[1].Ps[0].ri == S[0].Ps[i].ri && S[1].Ps[0].ci == S[0].Ps[i].ci)
		{
			S[1].score -= 10;
		}
	}
	for (int i = 1; i < S[1].size; i++)
	{
		if (S[0].Ps[0].ri == S[1].Ps[i].ri && S[0].Ps[0].ci == S[1].Ps[i].ci)
		{
			S[0].score -= 10;
		}
	}
}
void secondreset_minaddition(time_t & sec_diff,int &min, time_t& strt_time)
{
	if (sec_diff % 60==0 && sec_diff!=0)
	{
	    strt_time = time(0);
		sec_diff = 00;
		min += 1;
	}
}
void display_score_time(snake* S,time_t& time_diff,int num_of_plyr,int& min, time_t& strt_time,int speedtotal)
{
	gotoRowCol(14, 110);
	cout << "Snake 1 score: " << "       ";
	gotoRowCol(14, 110);
	cout << "Snake 1 score: " << S[0].score;
	if (num_of_plyr > 1)
	{
		gotoRowCol(15, 110);
		cout << "Snake 2 score: " << "         ";
		gotoRowCol(15, 110);
		cout << "Snake 2 score: " << S[1].score;
	}
	gotoRowCol(14, 150);
	cout << "Speed :" << "       ";
	gotoRowCol(14, 150);
	cout << "Speed :" << speedtotal;
	secondreset_minaddition(time_diff,min, strt_time);
	gotoRowCol(17, 110);
	cout << "Time :";
	gotoRowCol(18, 110);
	cout<<'\t' << "Min :" << min << '\t' << "Seconds :" << time_diff;
}
void display_instructions()
{
	gotoRowCol(20, 110);
	cout << "------------------>>Instructions<<----------------------------";
	gotoRowCol(21, 110);
	cout << "-->Head/Tail collision results in -10 score.";
	gotoRowCol(23, 110);
	cout << "-->Head/Head collision will stop game.";
	gotoRowCol(25, 110);
	cout << "-->Snake touching its tail will stop game.";
	gotoRowCol(27, 110);
	cout << "-->Special food will appear on min mutiple of 2 and 30 secs.";
	gotoRowCol(29, 110);
	cout << "-->Special food gives 10  score.";
	gotoRowCol(31, 110);
	cout << "-->Touching hardwall result in game stop.";
	gotoRowCol(33, 110);
	cout << "-->Passing through porous wall results in -20 score.";
	gotoRowCol(35, 110);
	cout << "-->Eating damage(X) food result in -10(occur in min 2 or multiple.";
	gotoRowCol(37, 110);
	cout << "-->Any player with score 5 or its multiple will increase speed of snakes.";
}
void display_side_Walls()
{
	char a = -37;
	for (int i = 0; i <= 99; i++)
	{
		gotoRowCol(0, i);
		cout << a;
	}
	for (int i = 1; i <= 39; i++)
	{
		gotoRowCol(i, 0);
		cout << a;
	}
	for (int i = 1; i <= 99; i++)
	{
		gotoRowCol(39, i);
		cout << a;
	}
	for (int i = 1; i <= 39; i++)
	{
		gotoRowCol(i, 99);
		cout << a;
	}
}
void display_hard_walls()
{
	char a = -37;
	for (int i = 20; i <= 60; i++)
	{
		gotoRowCol(9, i);
		cout << a;
	}
	for (int i = 10; i <= 13; i++)
	{
		gotoRowCol(i, 20);
		cout << a;
	}
	for (int i = 10; i <= 13; i++)
	{
		gotoRowCol(i, 60);
		cout << a;
	}
	for (int i = 20; i <= 60; i++)
	{
		gotoRowCol(35, i);
		cout << a;
	}
	for (int i = 31; i <= 35; i++)
	{
		gotoRowCol(i, 60);
		cout << a;
	}
	for (int i = 31; i <= 35; i++)
	{
		gotoRowCol(i, 20);
		cout << a;
	}
}
void display_porus_walls()
{
	char a = 177;
	for (int i = inside_vert_porus_strt; i <= inside_vert_porus_end ; i++)
	{
		gotoRowCol(i, 20);
		cout << a;
	}
	for (int i = inside_vert_porus_strt; i <= inside_vert_porus_end ; i++)
	{
		gotoRowCol(i, 60);
		cout << a;
	}
	for (int i = Vertical_porus_strt;i<=Vertical_porus_end; i++)
	{
		gotoRowCol(i, 10);
		cout << a;
	}
	for (int i = Vertical_porus_strt; i <= Vertical_porus_end; i++)
	{
		gotoRowCol(i, 80);
		cout << a;
	}
	for (int i = inside_horil_porus_strt; i <= inside_horil_porus_end; i++)
	{
		gotoRowCol(23, i);
		cout << a;
	}
	for (int i = inside_horiR_porus_strt; i <= inside_horiR_porus_end; i++)
	{
		gotoRowCol(23, i);
		cout << a;
	}
	for (int i = updwn_hori_porus_strt; i <= updwn_hori_porus_end; i++)
	{
		gotoRowCol(3, i);
		cout << a;
	}
	for (int i = updwn_hori_porus_strt; i <= updwn_hori_porus_end; i++)
	{
		gotoRowCol(38, i);
		cout << a;
	}
	for (int i = sml_vert_porus_strt; i <= sml_vert_porus_end; i++)
	{
		gotoRowCol(i, 18);
		cout << a;
	}
	for (int i = sml_vert_porus_strt; i <= sml_vert_porus_end; i++)
	{
		gotoRowCol(i, 66);
		cout << a;
	}
	for (int i = inside_hori_porus_strt; i <= inside_hori_porus_end; i++)
	{
		gotoRowCol(23, i);
		cout << a;
	}
}
void walls_enable(char & walls,char& pwalls,char& hwalls)
{
	char op;
	cout << "Select Difficulty level. Easy/Medium/Hard :"; 
	cin >> op;
	gotoRowCol(0, 0);
	cout << "                                                 ";
	switch (op)
	{
	case 'e':
	{
		walls = 'n';
		pwalls = 'n';
		hwalls = 'n';
		break;
	}
	case 'm':
	{
		walls = 'y';
		pwalls = 'n';
		hwalls = 'y';
		break;
	}
	case 'h':
	{
		walls = 'y';
		pwalls = 'y';
		hwalls = 'y';
		break;
	}
	default:
		gotoRowCol(39, 110);
		cout << ">>>Wrong input. Easy difficulty has be set by default.";
		break;
	}
}
bool hardwall_right_vertivcal(snake S)
{
	for (int i = 9, y = 31, c = 0; c <= 4; y++, c++, i++)
	{
		if ((S.Ps[0].ri == i && S.Ps[0].ci == 60) || (S.Ps[0].ri == y && S.Ps[0].ci == 60))
		{
			return true;
		}
	}
	return false;
}
bool hardwall_left_vertivcal(snake S)
{
	for (int i = 9, y = 31, c = 0; c <= 3; y++, c++, i++)
	{
		if ((S.Ps[0].ri == i && S.Ps[0].ci == 20) || (S.Ps[0].ri == y && S.Ps[0].ci == 20))
		{
			return true;
		}
	}
	return false;
}
bool hardwall_up_down(snake S)
{
	for (int i = 20, y = 31, c = 0; c <= 40; y++, c++, i++)
	{
		if ((S.Ps[0].ri == 9 && S.Ps[0].ci == i) || (S.Ps[0].ri == 35 && S.Ps[0].ci == i))
		{
			return true;
		}
	}
	return false;
}
bool hardwalltouch(snake* S,int num_of_plyr)
{
	for (int index = 0; index < num_of_plyr; index++)
	{
			if (hardwall_right_vertivcal(S[index]))
			{
				return true;
			}
			if (hardwall_left_vertivcal(S[index]))
			{
				return true;
			}
			if (hardwall_up_down(S[index]))
			{
				return true;
			}
	}
	return false;
}
bool insideverivcalporus(snake S)
{
	for (int i = inside_vert_porus_strt; i <= inside_vert_porus_end; i++)
	{
		if ((S.Ps[0].ri == i && S.Ps[0].ci == 20) || (S.Ps[0].ri == i && S.Ps[0].ci == 60))
		return true;
	}
	return false;
}
bool verticalporus(snake S)
{
	for (int i = Vertical_porus_strt; i <= Vertical_porus_end; i++)
	{
		if ((S.Ps[0].ri == i && S.Ps[0].ci == 10) || (S.Ps[0].ri == i && S.Ps[0].ci == 80))
			return true;
	}
	return false;
}
bool inside_porus_bars(snake S)
{
	for (int i = inside_horil_porus_strt, y = inside_horiR_porus_strt; i <= inside_horil_porus_end; i++,y++)
	{
		if ((S.Ps[0].ri == 23 && S.Ps[0].ci == i) || (S.Ps[0].ri == 23 && S.Ps[0].ci == y))
			return true;
	}
	return false;
}
bool horizontalporusbar(snake S)
{
	for (int i = updwn_hori_porus_strt, y = updwn_hori_porus_strt; i <= updwn_hori_porus_end;y++, i++)
	{
		if ((S.Ps[0].ri == 38 && S.Ps[0].ci == i) || (S.Ps[0].ri == 3 && S.Ps[0].ci == y))
			return true;
	}
	return false;
}
bool hangingporus(snake S)
{
	for (int i = sml_vert_porus_strt, y = sml_vert_porus_strt; i <= sml_vert_porus_end;y++, i++)
	{
		if ((S.Ps[0].ri == y && S.Ps[0].ci == 66) || (S.Ps[0].ri == i && S.Ps[0].ci == 18))
			return true;
	}
	return false;
}
bool lowerporushoribar(snake S)
{
	for (int i = inside_hori_porus_strt; i <= inside_hori_porus_end; i++)
	{
		if ((S.Ps[0].ri == 23 && S.Ps[0].ci == i))
			return true;
	}
	return false;
}
bool hardwall_right_vertivcal_f(position fpos)
{
	for (int i = 9, y = 31, c = 0; c <= 3; y++, c++, i++)
	{
		if ((fpos.ri == i && fpos.ci == 60) || (fpos.ri == y && fpos.ci == 60))
		{
			return true;
		}
	}
	return false;
}
bool hardwall_left_vertivcal_f(position fpos)
{
	for (int i = 9, y = 31, c = 0; c <= 3; y++, c++, i++)
	{
		if ((fpos.ri == i && fpos.ci == 20) || (fpos.ri == y && fpos.ci == 20))
		{
			return true;
		}
	}
	return false;
}
bool hardwall_up_down_f(position fpos)
{
	for (int i = 20, y = 31, c = 0; c <= 40; y++, c++, i++)
	{
		if ((fpos.ri == 9 && fpos.ci == i) || (fpos.ri == 35 && fpos.ci == i))
		{
			return true;
		}
	}
	return false;
}
bool insideverivcalporus_f(position fpos)
{
	for (int i = inside_vert_porus_strt; i <= inside_vert_porus_end; i++)
	{
		if ((fpos.ri == i && fpos.ci == 20) || (fpos.ri == i && fpos.ci == 60))
			return true;
	}
	return false;
}
bool verticalporus_f(position fpos)
{
	for (int i = Vertical_porus_strt; i <= Vertical_porus_end; i++)
	{
		if ((fpos.ri == i && fpos.ci == 10) || (fpos.ri == i && fpos.ci == 80))
			return true;
	}
	return false;
}
bool inside_porus_bars_f(position fpos)
{
	for (int i = inside_horil_porus_strt, y = inside_horiR_porus_strt; i <= inside_horil_porus_end; i++, y++)
	{
		if ((fpos.ri == 23 && fpos.ci == i) || (fpos.ri == 23 && fpos.ci == y))
			return true;
	}
	return false;
}
bool horizontalporusbar_f(position fpos)
{
	for (int i = updwn_hori_porus_strt, y = updwn_hori_porus_strt; i <= updwn_hori_porus_end; y++, i++)
	{
		if ((fpos.ri == 38 && fpos.ci == i) || (fpos.ri == 3 && fpos.ci == y))
			return true;
	}
	return false;
}
bool hangingporus_f(position fpos)
{
	for (int i = sml_vert_porus_strt, y = sml_vert_porus_strt; i <= sml_vert_porus_end; y++, i++)
	{
		if ((fpos.ri == y && fpos.ci == 66) || (fpos.ri == i && fpos.ci == 18))
			return true;
	}
	return false;
}
bool lowerporushoribar_f(position fpos)
{
	for (int i = inside_hori_porus_strt; i <= inside_hori_porus_end; i++)
	{
		if ((fpos.ri == 23 && fpos.ci == i))
			return true;
	}
	return false;
}
bool food_non_wall_pos(position fpos)
{
	if (hardwall_right_vertivcal_f(fpos))
		return true;
	if (hardwall_left_vertivcal_f(fpos))
		return true;
	if (hardwall_up_down_f(fpos))
		return true;
	if (insideverivcalporus_f(fpos))
		return true;
	if (verticalporus_f(fpos))
		return true;
	if (inside_porus_bars_f(fpos))
		return true;
	if (horizontalporusbar_f(fpos))
		return true;
	if (hangingporus_f(fpos))
		return true;
	if (lowerporushoribar_f(fpos))
		return true;
	return false;
}
bool specialfoodeat(snake& S, position sfpos)
{
	return S.Ps[0].ri == sfpos.ri && S.Ps[0].ci == sfpos.ci;
}
bool validfood(snake S, position fpos)
{
	for (int i = 0; i < S.size; i++)
	{
		if (S.Ps[i].ri == fpos.ri && S.Ps[i].ci == fpos.ci || (fpos.ri == 0 || fpos.ci == 0) || (fpos.ri == 39 || fpos.ci == 99))
		{
			return false;
		}
		if (food_non_wall_pos(fpos))
		{
			return false;
		}
	}
	return true;
}
void regeneratefood(position& fpos, snake& S, int row, int cols)
{
	do
	{
		fpos.ri = rand() % row;
		fpos.ci = rand() % cols;
	} while (!validfood(S, fpos));
}
void displayspecialfood(position& fpos, char fsym,snake& S,int row,int cols)
{
	do
	{
		fpos.ri = rand() % row;
		fpos.ci = rand() % cols;
	} while (!validfood(S, fpos));
	gotoRowCol(fpos.ri, fpos.ci);
	cout << fsym;
}
bool validtimeofspecialfood(int min,time_t time_diff,bool & specialfood,int sftime)
{
	if ((min % 2 == 0 && min > 1 && (time_diff == sftime)))
	{
		specialfood = false;
		return true;
	}
	return false;
}
void poruswalltouch(snake*& S, int num_of_plyr)
{
	for (int index = 0; index < num_of_plyr; index++)
	{
		if (insideverivcalporus(S[index]))
		{
			S[index].score -= 20;
		}
		if (verticalporus(S[index]))
		{
			S[index].score -= 20;
		}
		if (inside_porus_bars(S[index]))
		{
			S[index].score -= 20;
		}
		if (horizontalporusbar(S[index]))
		{
			S[index].score -= 20;
		}
		if (hangingporus(S[index]))
		{
			S[index].score -= 20;
		}
		if (lowerporushoribar(S[index]))
		{
			S[index].score -= 20;
		}
	}
}
void allwallsdisplay(char walls,char pwalls,char hwalls)
{
	if(walls=='y')
		display_side_Walls();
	if(hwalls=='y')
	display_hard_walls();
    if(pwalls=='y')
	display_porus_walls();
}
void speed_inc_func(snake*S,int num_of_plyr,bool& speedinc,int& speed,int speedtotal)
{
	for (int index = 0; index < num_of_plyr; index++)
	{
	     if ((S[index].score % 5 == 0) && (S[index].score > 2) && speedinc)
	     {
	       	speed = 20; 
	      	if ((speedtotal - speed) > 0)
			speed = 20;
	    	else
			speed = 0;
	     	speedinc = false;
	     }
	}
}
void display_fancywalls()
{
	char a = -37;
	for (int i =105;i<185;i++ )
	{
		gotoRowCol(7, i);
		cout << a;
	}
	for (int i = 105; i < 185; i++)
	{
		gotoRowCol(12, i);
		cout << a;
	}
	for (int i = 105; i < 185; i++)
	{
		gotoRowCol(39, i);
		cout << a;
	}
	for (int i=8;i<40;i++)
	{
		gotoRowCol(i,105);
		cout << a;
	}
	for (int i = 8; i < 40; i++)
	{
		gotoRowCol(i, 184);
		cout << a;
	}
	gotoRowCol(10, 138);
	cout << "SNAKE GAME";
}
int main()
{
	int num_of_plyr = 0, row = 0, cols = 0, dftime = 15, sftime = 5, min = 0,speed=0,speedtotal=100;
	position fpos;
	position sfpos;
	position dfpos;
	sfpos.ci = 0,sfpos.ri = 0,dfpos.ci = 0,dfpos.ri = 0;
	char fsym, sfsym = 146, dfsym = 88, walls , pwalls, hwalls;
	bool specialfood = true, damagefood = true, speedinc = true;
	time_t time_diff = 0;
	time_t strt_time = time(0);
	ifstream rdr("initial attributes.txt");
	walls_enable(walls, pwalls, hwalls);
	rdr >> row;
	rdr >> cols;
	rdr >> num_of_plyr;
	snake * S = new snake[num_of_plyr];
	for(int i=1,index=0;i<=num_of_plyr;index++,i++)
	{
		init(S[index],rdr, row, cols, fpos, fsym);
	}
	display_score_time(S,time_diff,num_of_plyr,min,strt_time,speedtotal );	
	display_instructions();
	display_fancywalls();
	while (true)
	{
		speed = 0;
		allwallsdisplay(walls,pwalls,hwalls);
		displayfood(fpos, fsym);
		if (_kbhit())
		{
			char ch = _getch();
			if (ch=='a' || ch == 's'|| ch == 'd'|| ch == 'w')
			    change_direction(ch, S[0]);
			if ((ch == 'j' || ch == 'l' || ch == 'k' || ch == 'i') && num_of_plyr>1)
				change_direction(ch, S[1]);
		}
		for (int i = 1, index = 0; i <= num_of_plyr; index++, i++)
		{
			erase_snake(S[index]);
			snakemove(S[index], row, cols);
			display_snake(S[index]);
			if (foodeat(S[index], fpos))
			{
				snakeexpansion(S[index]);
				regeneratefood(fpos, S[index], row, cols);
				speedinc = true;
			}
			if (specialfoodeat(S[index], sfpos))
			{
				specialsnakeexpansion(S[index]);
				specialfood = true;
			}
			if (specialfoodeat(S[index], dfpos))
			{
				damagefoodexpansion(S[index]);
				damagefood = true;
			}
		}
		time_t end_time = time(0);
		time_t time_diff = end_time - strt_time;
		for (int i = 1, index = 0; i <= num_of_plyr; index++, i++)
		{
			if (specialfood && (validtimeofspecialfood(min, time_diff,specialfood, sftime)))
				displayspecialfood(sfpos, sfsym, S[index], row, cols);
			if (damagefood && (validtimeofspecialfood(min, time_diff, damagefood, dftime)))
				displayspecialfood(dfpos, dfsym, S[index], row, cols);
		}
		display_score_time(S, time_diff, num_of_plyr, min,strt_time,speedtotal);
		if (num_of_plyr > 1)
		{
			snake_tailcollide(S);
		}
		if (pwalls == 'y')
		{
			poruswalltouch(S, num_of_plyr);
		}
		speed_inc_func(S,num_of_plyr,speedinc,speed,speedtotal);
		speedtotal -= speed;
		Sleep(speedtotal-speed);
		if (num_of_plyr > 1 && snakeheadcollision(S))
		{
			break;
		}
		if ((hardwalltouch(S, num_of_plyr)) && hwalls=='y')
		{
			break;
		}
		if (snaketouchitself(S,num_of_plyr))
		{
			break;
		}
		if (walls == 'y')
		{
			if (snaketouchsidewall(S, cols,num_of_plyr))
				break;
		}
	}
	return _getch();
}
