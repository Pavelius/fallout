#include "main.h"

const char* formula::get(char* result, const char* result_maximum) const {
	if(modifier)
		szprint(result, result_maximum, "%1i", modifier);
	if(a1.multiplier == a2.multiplier
		&& a1.divider == a2.divider
		&& (a1.divider>1 || a1.multiplier>1)) {
		if(result[0])
			result = zend(szprint(zend(result), result_maximum, "+"));
		if(a1.divider == 0)
			szprint(zend(result), result_maximum, "(%2+%3)x%1i", a1.multiplier,
				ability_data[a1.ability].name_abr,
				ability_data[a2.ability].name_abr);
		else
			szprint(zend(result), result_maximum, "(%2+%3)/%1i", a1.divider,
				ability_data[a1.ability].name_abr,
				ability_data[a2.ability].name_abr);
	} else {
		a1.get(result, result_maximum);
		a2.get(result, result_maximum);
	}
	return result;
}

const char* formula::term::get(char* result, const char* result_maximum) const {
	if(!multiplier)
		return result;
	if(result[0])
		result = zend(szprint(zend(result), result_maximum, "+"));
	if(divider == 0) {
		if(multiplier>1)
			result = zend(szprint(result, result_maximum, "%1ix", multiplier));
		result = zend(szprint(result, result_maximum, ability_data[ability].name_abr));
	} else {
		result = zend(szprint(result, result_maximum, ability_data[ability].name_abr));
		result = zend(szprint(result, result_maximum, "/%1i", divider));
	}
	return result;
}