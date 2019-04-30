#pragma once
#include <vector>

class Utils
{
public:
	template <class T>
	static bool add(std::vector<T*> &v, T* el);
	template <class T>
	static T* del(std::vector<T*> &v, T* el);
};

template <class T>
bool Utils::add(std::vector<T*> &v, T* el) {
	try {
		v.push_back(el);
		return true;
	}
	catch (...) {
		return false;
	}
}

template <class T>
T* Utils::del(std::vector<T*> &v, T* el) {
	bool found = false;
	unsigned int count = 0;
	for (typename std::vector<T*>::iterator it = v.begin(); it != v.end(); it++) {
		if (el == *it) {
			found = true;
			break;
		}
		count++;
	}
	if (found) {
		try {
			T* e = v.at(count);
			T* copy = new T(e);
			v.erase(v.begin() + count);
			return copy;
		}
		catch (...) {
			return nullptr;
		}
	}
	return nullptr;
}
