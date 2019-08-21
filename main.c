
// spec: https://esolangs.org/wiki/Alphabetti_spaghetti

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "array.h"
char* readfile(char* fname, int* _size);

typedef struct
{
	int x; // grows right
	int y; // grows down
}
Pos;

typedef struct
{
	int x;
	int y;
	int val;
}
Cell;

array_of(Cell) mem;
Pos pos;

Cell* GetCell(Pos pos)
{
	Cell* cell = 0;

	for(int i=0; i<mem.count; i++)
	{
		if(mem.data[i].x == pos.x && mem.data[i].y == pos.y)
		{
			cell = &mem.data[i];
			break;
		}
	}

	if(!cell)
	{
		cell = add_elem(mem, (Cell){pos.x, pos.y});
	}

	return cell;
}

int main(int argc, char* argv[])
{
	if(argc < 2) { puts("specify filename"); return 1; }

	char* str = readfile(argv[1], 0);
	if(!str) { puts("cannot read file"); return 1; }
	
	char* p = str;

	while(*p)
	{
		char c = tolower(*p);

		switch(c)
		{
		case 'a': case 'b': pos.x++; break;
		case 'c': case 'd': pos.y++; break;
		case 'e': case 'f': pos.x--; break;
		case 'g': case 'h': pos.y--; break;
		case 'i': case 'j': GetCell(pos)->val++; break;
		case 'k': case 'l': GetCell(pos)->val--; break;
		case 'm': case 'n': {int val = 0; if(scanf("%d", &val) == 1) GetCell(pos)->val = val;} break;
		// spec does not say it, but interp probably needs to print a space after a number
		case 'o': case 'p': printf("%d", GetCell(pos)->val); break; 
		case 'q': case 'r': 
			if(GetCell(pos)->val == 0) {
				while(*p && tolower(*p) != 'j') p++;
				// spec does not specify what to do if `j` is not found
				// in this impl it just exits the program without error
				// i.e. it jumps beyond the end of the program
				if(!*p) exit(0);
				// otherwise p points to 'j' - advance p to the "instruction immediately after the next j"
				else {p++; continue;}
			}
			break;
		case 's': case 't': 
			if(GetCell(pos)->val != 0) {
				while(p >= str && tolower(*p) != 'i') p--;
				// spec does not specify what to do if `i` is not found
				// in this impl it jumps to the start of the program
				if(p < str) {p = str; continue;}
				// otherwise p points to 'i' - advance p to the "instruction immediately after previous i"
				else {p++; continue;}
			}
			break;
		case 'u': case 'v': GetCell(pos)->val *= 10; break;
		case 'w': case 'x': GetCell(pos)->val /= 10; break;
		case 'y': case 'z': printf("%s", str); break;
		}
		p++;
	}
}