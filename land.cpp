#include "main.h"

short unsigned land_info::random() const {
	auto r = central[0];
	if(central[1]
		&& (rand() % 100) < percent) {
		for(auto i = 1; i < 4 && central[i]; i++) {
			r = central[i];
			if((rand() % 100) < 50)
				break;
		}
	}
	return r;
}