#pragma once
#include <string>

struct Exception : std::exception {
	int id;
	std::string msg;
	Exception(int newid, std::string newmsg = "") {
		id = newid;
		msg = newmsg;
	}
};