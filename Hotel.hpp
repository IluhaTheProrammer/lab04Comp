#pragma once
#include <iostream>
#include <string>
//#include <vector>
//#include <algorithm>
#include <iomanip>
#include "Exception.hpp"
#include "custom.hpp"

using namespace std;

namespace hot {

	const int luxMax = 2;
	const int multiMax = 4;

	struct Date {
	private:
		unsigned year = 1970;
		unsigned month = 1;
		unsigned day = 1;
	public:
		Date() {};
		Date(unsigned newyear, unsigned newmonth, unsigned newday) {
			setYear(newyear);
			setMonth(newmonth);
			setDay(newday);
		}
		Date(long JD) {
			fromJDay(JD);
		}
		void setYear(unsigned newyear) {
			year = newyear;
		}
		void setMonth(unsigned newmonth) {
			if (newmonth < 1 || newmonth>12) {
				throw Exception(1, "Date error: Wrong Month!");
			}
			month = newmonth;
		}
		void setDay(unsigned newday) {
			if (day < 1 || day>31) {
				throw Exception(1, "Date error: Wrong Day!");
			}
			if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && newday > 31) {
				throw Exception(1, "Date error: Wrong Day!");
			}
			if ((month == 4 || month == 6 || month == 9 || month == 11) && newday > 30) {
				throw Exception(1, "Date error: Wrong Day!");
			}
			if (year % 4 != 0 && month == 2 && newday > 28) {
				throw Exception(1, "Date error: Wrong Day!");
			}
			if (year % 4 == 0 && month == 2 && newday > 29) {
				throw Exception(1, "Date error: Wrong Day!");
			}
			day = newday;
		}

		unsigned getDay() {
			return day;
		}
		unsigned getMonth() {
			return month;
		}
		unsigned getYear() {
			return year;
		}

		long toJDay() {
			long a = (14 - month) / 12;
			long y = year + 4800 - a;
			long m = month + 12 * a - 3;
			long J = day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
			return J;
		}

		void fromJDay(long JD) {
			long a = JD + 32044;
			long b = (4 * a + 3) / 146097;
			long c = a - (146097 * b) / 4;
			long d = (4 * c + 3) / 1461;
			long e = c - (1461 * d) / 4;
			long m = (5 * e + 2) / 153;
			day = e - (153 * m + 2) / 5 + 1;
			month = m + 3 - 12 * (m / 10);
			year = 100 * b + d - 4800 + (m / 10);
		}

		Date operator+ (const unsigned days) {
			long curDate = toJDay();
			Date res = Date(curDate + days);
			return res;
		}
		Date operator- (const unsigned days) {
			long curDate = toJDay();
			Date res = Date(curDate - days);
			return res;
		}

		string toString(string sep = "/") {
			string res = to_string(day) + sep + to_string(month) + sep + to_string(year);
			return res;
		}
	};

	struct Person {
		string name="";
		unsigned age=0;
		Date date;
		unsigned days=0;
		Person(){}
		Person(string _name, unsigned _age, Date _date, unsigned _days) {
			name = _name;
			age = _age;
			date = _date;
			days = _days;
		}
		Person(string _name, unsigned _age) {
			name = _name;
			age = _age;
		}
	};

	class Room {
	protected:
		bool busy = false;
		custom::vector <Person> guests;
		unsigned fee = 0;
	public:
		Room() {}
		Room(unsigned _fee, bool _busy = false) {
			busy = _busy;
			fee = _fee;
		}
		Room(const custom::vector <Person>& _guests, unsigned _fee, bool _busy = true) {
			busy = _busy;
			fee = _fee;
			guests = _guests;
		}
		void setBusy(bool _busy) {
			busy = _busy;
		}
		void setFee(unsigned _fee) {
			fee = _fee;
		}
		bool getBusy() {
			return busy;
		}
		unsigned getFee() {
			return fee;
		}

		virtual string getType() {
			return "Base";
		}

		virtual void addGuest(const Person& _guest) {}

		virtual void delGuest(unsigned id) {
			if (id >= guests.size() || id < 0) throw Exception(2, "Room exception: Cannot delete guest!");
			guests.erase(guests.begin() + id);
			if (!guests.size()) busy = false;
		}

		virtual Person& getGuest() {
			return guests.at(0);
		}

		void setGuests(const custom::vector <Person>& _guests) {
			guests = _guests;
		}
		custom::vector <Person>& getGuests() {
			return guests;
		}

		virtual unsigned free(unsigned guestid = 0) {
			if (!busy) throw Exception(2, "Room exception: Cannot free room!");
			busy = false;
			guests.clear();
			return -1;
		}

		virtual ~Room() {
			busy = false;
			guests.clear();
		}

	};

	class Single : public Room {
	public:
		Single() {};
		Single(unsigned _fee, bool _busy = false) {
			busy = _busy;
			fee = _fee;
		}
		Single(const Person& _guest, unsigned _fee, bool _busy = true) {
			busy = _busy;
			fee = _fee;
			guests.push_back(_guest);
		}
		void addGuest(const Person& _guest) {
			if (guests.size() > 0) throw Exception(2, "Room exception: Single: Cannot add guest!");
			guests.push_back(_guest);
			busy = true;
		}
		void delGuest() {
			if (guests.size() == 0) throw Exception(2, "Room exception: Single: Cannot delete guest!");
			guests.clear();
			busy = false;
		}
		Person& getGuest() {
			return guests.at(0);
		}
		string getType() {
			return "Single";
		}
		unsigned free(unsigned guestid = 0) {
			if (!busy) throw Exception(2, "Room exception: Single: Cannot free room!");
			busy = false;
			unsigned cash = fee * guests.at(0).days;
			guests.clear();
			return cash;
		}
	};

	class Lux : public Room {
	public:
		Lux() {};
		Lux(unsigned _fee, bool _busy = false) {
			busy = _busy;
			fee = _fee;
		}
		Lux(const custom::vector <Person>& _guests, Date _date, unsigned _days, unsigned _fee, bool _busy = true) {
			busy = _busy;
			fee = _fee;
			guests = _guests;
			for (auto it = _guests.begin(); it != _guests.end(); it++) {
				(*it).date = _date;
				(*it).days = _days;
			}
		}
		void addGuest(const Person& _guest) {//if room is already occupied
			if (guests.size() == luxMax) throw Exception(2, "Room exception: Lux: Cannot add guest!");
			Person guest = _guest;
			if (guests.size()) {
				guest.date = guests.at(0).date;
				guest.days = guests.at(0).days;
			}
			guests.push_back(guest);
			busy = true;
		}
		Person& getGuest(unsigned id) {
			if (id >= guests.size() || id < 0) throw Exception(2, "Room exception: Lux: Cannot get guest!");
			return guests.at(id);
		}
		string getType() {
			return "Lux";
		}

		unsigned free(unsigned guestid = 0) {
			if (!busy) throw Exception(2, "Room exception: Lux: Cannot free room!");
			busy = false;
			unsigned cash = fee * guests.at(0).days;
			guests.clear();
			return cash;
		}
	};

	class Multi : public Room {
	public:
		Multi() {}
		Multi(unsigned _fee, bool _busy = false) {
			busy = _busy;
			fee = _fee;
		}
		void addGuest(const Person& _guest) {
			if (guests.size() == multiMax) throw Exception(2, "Room exception: Multi: Cannot add guest!");
			guests.push_back(_guest);
			busy = true;
		}
		Person& getGuest(unsigned id) {
			if (id >= guests.size() || id < 0) throw Exception(2, "Room exception: Multi: Cannot get guest!");
			return guests.at(id);
		}
		string getType() {
			return "Multi";
		}

		unsigned free(unsigned id) {
			if (!busy) throw Exception(2, "Room exception: Multi: Cannot free room!");
			if (id >= guests.size() || id < 0) throw Exception(2, "Room exception: Multi: Cannot free room!");
			unsigned cash = fee * guests.at(id).days;
			delGuest(id);
			return cash;
		}
	};

	template <typename T>
	shared_ptr<Room> toShared(T room) {
		shared_ptr<Room> shared = make_shared<T>(room);
		return shared;
	}

	class Hotel {
	private:
		string name="";
		custom::vector <shared_ptr<Room>> rooms;
	public:
		Hotel(){}
		Hotel(const string _name) {
			name = _name;
		}
		Hotel(const string _name, const custom::vector <shared_ptr<Room>>& _rooms) {
			name = _name;
			rooms = _rooms;
		}
		void setName(const string _name) {
			name = _name;
		}
		void setRooms(const custom::vector <shared_ptr<Room>>& _rooms) {
			rooms = _rooms;
		}
		string getName() {
			return name;
		}
		custom::vector <shared_ptr<Room>>& getRooms() {
			return rooms;
		}

		void addRoom(shared_ptr<Room> _room) {
			rooms.push_back(_room);
		}
		shared_ptr<Room>& getRoom(unsigned id) {
			if (id >= rooms.size() || id < 0)
				throw Exception(3, "Hotel error: Cannot return room!");
			return rooms.at(id);
		}
		void delRoom(unsigned id) {
			if (id >= rooms.size() || id < 0)
				throw Exception(3, "Hotel error: Cannot delete room!");
			rooms.erase(rooms.begin() + id);
		}
		//--------------------------------------------------------------- APP funcs

		void addGuest(const Person& _guest, unsigned id) {
			if (id >= rooms.size() || id < 0)
				throw Exception(3, "Hotel error: Cannot add guest!");

			try {
				rooms.at(id)->addGuest(_guest);
			}
			catch (Exception e) {
				throw Exception(3, "Hotel error: Cannot add guest: " + e.msg);
			}
		}

		void delGuest(unsigned id, unsigned guestid = 0) {
			if (id >= rooms.size() || id < 0)
				throw Exception(3, "Hotel error: Cannot delete guest!");

			try {
				rooms.at(id)->delGuest(guestid);
			}
			catch (Exception e) {
				throw Exception(3, "Hotel error: Cannot delete guest: " + e.msg);
			}
		}

		unsigned freeRoom(unsigned id, unsigned guestid = 0) {
			if (id >= rooms.size() || id < 0)
				throw Exception(3, "Hotel error: Cannot free room!");

			try {
				return rooms.at(id)->free(guestid);
			}
			catch (Exception e) {
				throw Exception(3, "Hotel error: Cannot free room: " + e.msg);
			}
		}

		void print(ostream& out = cout) {
			int count = 0;
			out << "======================[ Hotel '" + name + "' ]======================"<<endl;
			for (auto i = rooms.begin(); i != rooms.end();i++) {
				shared_ptr<Room> room = *i;
				out << count << "  Type: " << setw(6) << left << room->getType() << "  Fee: " << setw(4) << left << to_string(room->getFee()) + "$" << " Busy: " << (room->getBusy() ? "Yes" : "No") << '\n';
				int gcount = 0;
				for (auto j = room->getGuests().begin(); j != room->getGuests().end(); j++) {
					Person guest = *j;
					Date kickout = guest.date + guest.days;
					cout << "\t |-> "<<gcount<<"  Name: " << setw(17) << left << guest.name << "  Age: " << setw(3) << left << guest.age << "  Date: " << setw(10) << left << guest.date.toString() << "  Days: " << setw(4) << left << guest.days << "  Expires: " << setw(10) << left << kickout.toString() << '\n';
					gcount++;
				}
				cout << endl;
				count++;
			}
		}


	};

}