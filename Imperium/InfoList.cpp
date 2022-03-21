#include "pch.h"
#include "InfoList.h"

void InfoList::add(string key) {
	this->infos.insert(make_pair(key, ""));
}
void InfoList::set(string key, const char* value) {
	if (this->infos.find(key) == this->infos.end()) {
		this->infos.insert(make_pair(key, value));
	}
	else {
		this->infos.find(key)->second = value;
	}
}
void InfoList::set(string key, int value) {
	if (this->infos.find(key) == this->infos.end()) {
		this->infos.insert(make_pair(key, to_string(value)));
	}
	else {
		this->infos.find(key)->second = to_string(value);
	}
}