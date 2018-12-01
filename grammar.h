#pragma once

namespace grammar {
enum gender_s { Neuter, Musculine, Femine };
namespace pluar {
char* by(char* r, const char* s);
char* of(char* result, const char* result_maximum, const char* s);
}
char* by(char* result, const char* result_maximum, const char* s);
char* of(char* result, const char* result_maximum, const char* s);
char* to(char* result, const char* result_maximum, const char* s);
char* get(char* result, const char* result_maximum, const char* word, int count);
char* wh(char* result, const char* result_maximum, const char* s);
}
