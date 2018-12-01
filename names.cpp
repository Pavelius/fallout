#include "main.h"

static struct nameinfo {
	gender_s		gender;
	const char*		name;
} objects[] = {{Male, "�����"},
{Male, "����"},
{Male, "������"},
{Female, "�����"},
{Male, "�����"},
{Male, "���"},
{Male, "�������"},
{Male, "�����"},
{Female, "�������"},
};

static int* select(int* result, gender_s gender) {
	int* p = result;
	for(int i = 1; i < sizeof(objects) / sizeof(objects[0]); i++) {
		if(objects[i].gender != gender)
			continue;
		*p++ = i;
	}
	*p = 0;
	return result;
}