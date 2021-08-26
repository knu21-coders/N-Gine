#pragma once
#include "framework.h"

class NgineMap {
private:
	void* _map[10] = { 0 };
public: 
	void Add(const WCHAR* unicode_name, void** _target) {
		if(unicode_name[0] == '\0') return;


	}
};