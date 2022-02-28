#include "pch.h"
#include "ItemSearch.h"
#include "include.h"

const char* ItemSearch::toString() {
	stringstream buffer;
	buffer.str();
	buffer << "[bpPos: " << this->bpPos;
	buffer << " slotPos: " << this->slotPos << " ";
	buffer << " id: " << this->id << " ";
	buffer << " amount: " << this->amount << "] - ";
	return buffer.str().c_str();
}