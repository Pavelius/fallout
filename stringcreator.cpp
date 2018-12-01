#include "crt.h"
#include "stringcreator.h"

bool ischa(unsigned u) {
	return (u >= 'A' && u <= 'Z')
		|| (u >= 'a' && u <= 'z')
		|| (u >= 0x410 && u <= 0x44F);
}

unsigned szupper(unsigned u) {
	if(u >= 0x61 && u <= 0x7A)
		return u - 0x61 + 0x41;
	else if(u >= 0x430 && u <= 0x44F)
		return u - 0x430 + 0x410;
	return u;
}

unsigned szlower(unsigned u) {
	if(u >= 0x41 && u <= 0x5A)
		return u - 0x41 + 0x61;
	else if(u >= 0x410 && u <= 0x42F)
		return u - 0x410 + 0x430;
	return u;
}

const char* szskipcr(const char* p) {
	if(*p == '\n') {
		p++;
		if(*p == '\r')
			p++;
	} else if(*p == '\r') {
		p++;
		if(*p == '\n')
			p++;
	}
	return p;
}

const char* szskipcrr(const char* p0, const char* p) {
	if(!p)
		return 0;
	if(p0 >= p)
		return p;
	if(p[-1] == '\n') {
		p--;
		if(p0 >= p)
			return p;
		if(p[-1] == '\r')
			p--;
	} else if(p[-1] == '\r') {
		p--;
		if(p0 >= p)
			return p;
		if(p[-1] == '\n')
			p--;
	}
	return p;
}

const char* psnum16(const char* p, int& value) {
	int result = 0;
	const int radix = 16;
	while(*p) {
		char a = *p;
		if(a >= '0' && a <= '9') {
			result = result * radix;
			result += a - '0';
		} else if(a >= 'a' && a <= 'f') {
			result = result * radix;
			result += a - 'a' + 10;
		} else if(a >= 'A' && a <= 'F') {
			result = result * radix;
			result += a - 'A' + 10;
		} else
			break;
		p++;
	}
	value = result;
	return p;
}

const char* psnum10(const char* p, int& value) {
	int result = 0;
	const int radix = 10;
	while(*p) {
		char a = *p;
		if(a >= '0' && a <= '9') {
			result = result * radix;
			result += a - '0';
		} else
			break;
		p++;
	}
	value = result;
	return p;
}

// Parse string to number
const char* psnum(const char* p, int& value) {
	value = 0;
	if(!p)
		return 0;
	bool sign = false;
	// Установка знака
	if(*p == '-') {
		sign = true;
		p++;
	}
	// Перегрузка числовой системы
	if(p[0] == '0' && p[1] == 'x') {
		p += 2;
		p = psnum16(p, value);
	} else
		p = psnum10(p, value);
	if(sign)
		value = -value;
	return p;
}

// Parse string to string (from c/json format)
const char* psstr(const char* p, char* r, char end_symbol) {
	r[0] = 0;
	if(!p)
		return 0;
	while(*p) {
		if(*p == end_symbol) {
			*r++ = 0;
			return p + 1;
		} else if(*p != '\\') {
			*r++ = *p++;
			continue;
		}
		p++;
		int value;
		switch(*p) {
		case 'n':
			*r++ = '\n';
			p++;
			break;
		case 'r':
			*r++ = '\r';
			p++;
			break;
		case 't':
			*r++ = '\t';
			p++;
			break;
		case 'b':
			*r++ = '\b';
			p++;
			break;
		case 'f':
			*r++ = '\f';
			p++;
			break;
		case 'v':
			*r++ = '\v';
			p++;
			break;
			// Число в кодировке UNICODE
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			p = psnum10(p, value);
			r = szput(r, value);
			break;
			// Число в кодировке UNICODE (16-ричная система)
		case 'x':
		case 'u':
			p = psnum16(p + 1, value);
			r = szput(r, value);
			break;
		case '\n':
		case '\r':
			// Перевод строки в конце
			while(*p == '\n' || *p == '\r')
				p = szskipcr(p);
			break;
		default:
			// Любой символ, который будет экранирован ( \', \", \\)
			*r++ = *p++;
			break;
		}
	}
	return p;
}

const char* psidn(const char* p, char* r, char* re) {
	*r = 0;
	while(*p) {
		if((*p >= '0' && *p <= '9') || *p == '_' || ischa(*p)) {
			if(r < re)
				*r++ = *p;
			p++;
		} else
			break;
	}
	*r = 0;
	return p;
}

char* sznum(char* result, int num, int precision, const char* empthy, int radix) {
	char* p1 = result;
	if(num == 0) {
		if(empthy)
			zcpy(p1, empthy);
		else {
			zcpy(p1, "0");
			while(--precision > 0)
				zcat(p1, "0");
		}
		p1 = zend(p1);
	} else {
		char temp[32];
		int p = 0;
		if(num < 0) {
			*p1++ = '-';
			num = -num;
		}
		switch(radix) {
		case 16:
			while(num) {
				int a = (num%radix);
				if(a > 9)
					temp[p++] = 'A' - 10 + a;
				else
					temp[p++] = '0' + a;
				num /= radix;
			}
			break;
		default:
			while(num) {
				temp[p++] = '0' + (num%radix);
				num /= radix;
			}
			break;
		}
		while(precision-- > p)
			*p1++ = '0';
		while(p)
			*p1++ = temp[--p];
		p1[0] = 0;
	}
	return result;
}

int sz2num(const char* p1, const char** pp1) {
	int result = 0;
	bool sign = false;
	const int radix = 10;
	while(*p1 && *p1 != '-' && (*p1 < '0' || *p1 > '9'))
		p1++;
	if(*p1 == '-') {
		sign = true;
		p1++;
	}
	while(*p1) {
		char a = *p1;
		if(a < '0' || a > '9')
			break;
		result = result * radix;
		result += a - '0';
		p1++;
	}
	if(sign)
		result = -result;
	if(pp1)
		*pp1 = p1;
	return result;
}

bool szmatch(const char* text, const char* name) {
	while(*name) {
		if(*name++ != *text++)
			return false;
	}
	if(ischa(*text))
		return false;
	return true;
}

static bool szpmatch(const char* text, const char* s, const char* s2) {
	while(true) {
		register const char* d = text;
		while(s < s2) {
			if(*d == 0)
				return false;
			unsigned char c = *s;
			if(c == '?') {
				s++;
				d++;
			} else if(c == '*') {
				s++;
				if(s == s2)
					return true;
				while(*d) {
					if(*d == *s)
						break;
					d++;
				}
			} else {
				if(*d++ != *s++)
					return false;
			}
		}
		return true;
	}
}

bool szpmatch(const char* text, const char* pattern) {
	const char* p = pattern;
	while(true) {
		const char* p2 = zchr(p, ',');
		if(!p2)
			p2 = zend(p);
		if(szpmatch(text, p, p2))
			return true;
		if(*p2 == 0)
			return false;
		p = zskipsp(p2 + 1);
	}
}

void stringcreator::parseidentifier(char* result, const char* result_max, const char* identifier) {
	auto len = zlen(identifier) + 3;
	if((result_max - result) >= len) {
		zcat(result, "[-");
		zcat(result, identifier);
		zcat(result, "]");
	}
}

void stringcreator::parsevariable(char* result, const char* result_max, const char** format) {
	char temp[260];
	auto src = *format;
	int s = 0;
	if(*src == '(') {
		*format += 1;
		while(*src && *src != ')')
			src++;
		s = src - *format;
		src++;
	} else {
		while(*src) {
			const char* s1 = src;
			unsigned ch = szget(&src);
			if(!ischa(ch) && !isnum(ch) && ch != '_') {
				src = s1;
				break;
			}
		}
		s = src - *format;
	}
	temp[0] = 0;
	if(s != 0 && s<int(sizeof(temp) - 1)) {
		memcpy(temp, *format, s);
		temp[s] = 0;
	}
	*format = src;
	parseidentifier(result, result_max, temp);
}

char* stringcreator::parsenumber(char* dst, const char* result_max, unsigned value, int precision, const int radix) {
	char temp[32]; int i = 0;
	if(!value) {
		zcpy(dst, "0");
		return dst;
	}
	if(!result_max)
		result_max = dst + 32;
	while(value) {
		temp[i++] = (value % radix);
		value /= radix;
	}
	while(precision-- > i) {
		if(dst<result_max)
			*dst++ = '0';
	}
	while(i) {
		auto v = temp[--i];
		if(dst < result_max) {
			if(v < 10)
				*dst++ = '0' + v;
			else
				*dst++ = 'A' + (v-10);
		}
	}
	dst[0] = 0;
	return dst;
}

char* stringcreator::parseint(char* dst, const char* result_max, int value, int precision, const int radix) {
	if(value < 0) {
		if(dst<result_max)
			*dst++ = '-';
		value = -value;
	}
	return parsenumber(dst, result_max, value, precision, radix);
}

const char* stringcreator::parseformat(char* dst, const char* result_max, const char* src, const char* vl) {
	if(*src == '%') {
		auto sym = *src++;
		if(dst < result_max)
			*dst++ = sym;
		*dst = 0;
		return src;
	}
	*dst = 0;
	bool prefix_plus = false;
	if(*src == '+') {
		prefix_plus = true;
		src++;
	}
	if(*src >= '1' && *src <= '9') {
		// Если число, просто подставим нужный параметр
		int pn = 0, pnp = 0;
		while(isnum(*src))
			pn = pn * 10 + (*src++) - '0';
		if(src[0] == '.' && (src[1] >= '0' && src[1] <= '9')) {
			src++;
			while(*src >= '0' && *src <= '9')
				pnp = pnp * 10 + (*src++) - '0';
		}
		if(*src == 'i') {
			src++;
			auto value = ((int*)vl)[pn - 1];
			if(prefix_plus && value >= 0) {
				if(dst<result_max)
					*dst++ = '+';
			}
			dst = parseint(dst, result_max, value, pnp, 10);
		} else if(*src == 'h') {
			src++;
			dst = parsenumber(dst, result_max, (unsigned)(((int*)vl)[pn - 1]), pnp, 16);
		} else {
			if(((char**)vl)[pn - 1])
				zcpy(dst, ((char**)vl)[pn - 1], result_max - dst);
		}
	} else
		parsevariable(dst, result_max, &src);
	return src;
}

void stringcreator::printv(char* result, const char* result_maximum, const char* src, const char* vl) {
	if(!result)
		return;
	if(!src) {
		result[0] = 0;
		return;
	}
	while(true) {
		switch(*src) {
		case 0:
			*result = 0;
			return;
		case '%':
			src = parseformat(result, result_maximum, src + 1, vl);
			result = zend(result);
			if(result > result_maximum)
				result = const_cast<char*>(result_maximum);
			break;
		default:
			if(result < result_maximum)
				*result++ = *src;
			src++;
			break;
		}
	}
}

void stringcreator::print(char* result, const char* result_maximum, const char* src, ...) {
	printv(result, result_maximum, src, xva_start(src));
}

char* szprintvs(char* result, const char* result_maximum, const char* src, const char* vl) {
	stringcreator e;
	e.printv(result, result_maximum, src, vl);
	return result;
}

char* szprint(char* result, const char* result_maximum, const char* src, ...) {
	stringcreator e;
	e.printv(result, result_maximum, src, xva_start(src));
	return result;
}