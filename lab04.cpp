#include <iostream>
#include <string>
//#include <vector>
//#include <algorithm>
#include "custom.hpp"
#include "Exception.hpp"
#include "Hotel.hpp"
#include "SmartMenu.hpp"
#include <conio.h>
#include <cstdlib>

using namespace std;
using namespace hot;
using namespace menu;

const int arrowUp = 72;
const int arrowDown = 80;
const int arrowLeft = 75;
const int arrowRight = 77;
const int keyReturn = '\r'; //'\n'
const int keyEsc = 27;
const int keyBspace = 8;

void MenuRoutine(Menu& menu, Hotel& hotel, unsigned maxSwitch=-1) {
	hotel.print();
	menu.print();
	cout << "\nControls: [^|v] - Option [<|>] - Menu [Esc] - Prev [<-] - Exit\n"<<endl;
	vector<unsigned> id_stack{ menu.getPanelId() };
	char ch;
	while (ch = _getch()) {
		Panel& cur = menu.getPanel(menu.getPanelId());
		switch (ch) {
		case(arrowUp): {
			cur.setActiveId(cur.getActiveId()-1);
			break;
		}
		case(arrowDown): {
			cur.setActiveId(cur.getActiveId() +1);
			break;
		}
		case(arrowLeft): {
			menu.setPanelId(menu.getPanelId() - 1);
			break;
		}
		case(arrowRight): {
			if (menu.getPanelId() >= maxSwitch) break;
			menu.setPanelId(menu.getPanelId() + 1);
			break;
		}
		case(keyReturn): {
			cur.run();
			break;
		}
		case(keyEsc): {
			if(id_stack.size() <= 1)
				return;
			id_stack.erase(id_stack.end() - 1);
			menu.setPanelId(*(id_stack.end() - 1));
			break;
		}
		case(keyBspace): {
			return;
			break;
		}
		default: {
			break;
		}
		}
		
		if (menu.getPanelId() != id_stack.at(id_stack.size() - 1)) {
			id_stack.push_back(menu.getPanelId());
		}
		system("cls");
		hotel.print();
		menu.print();
		cout << "\nControls: [^|v] - Option [<|>] - Menu [Esc] - Prev [<-] - Exit\n" << endl;
	}
}


void AddRoom(Hotel& hotel) {
	try{
	shared_ptr<Room> room;
	cout << "Type of new room:\n[s] - Single\n[l] - Lux\n[m] - Multi\n>> ";
	string str;
	bool fl = true;
	while (fl) {
		cin >> str;
		if (str == "s") {
			room = make_shared<Single>();
			fl = false;
		}
		else if (str == "l") {
			room = make_shared<Lux>();
			fl = false;
		}
		else if (str == "m") {
			room = make_shared<Multi>();
			fl = false;
		}
		else {
			cout<< "Wrong input!" << endl;
		}
	}
	cout << "Fee of new room ($ per day): ";
	unsigned fee;
	cin >> fee;
	room->setFee(fee);
	hotel.addRoom(room);
	return;
	}
	catch (Exception e) {
		cout << "Oops! Sh*t happened: " + e.msg;
		_getch();
		return;
	}
}


void DelRoom(Hotel& hotel) {
	try {
		unsigned id;
		cout << "ID of room to delete: ";
		cin >> id;
		hotel.delRoom(id);
	}
	catch (Exception e) {
		cout << "Oops! Sh*t happened: " + e.msg;
		_getch();
		return;
	}
}

void ManageRoom(Hotel& hotel) {
	try {
		unsigned id;
		cout << "ID of room to manage: ";
		cin >> id;
		shared_ptr<Room> room = hotel.getRoom(id);
		cout << "Type: " << room->getType() << endl;
		cout << "Fee: " << room->getFee()<<"$ per day" << endl;
		cout << "Busy: " << (room->getBusy() ? "yes" : "no") << endl;
		cout << "Guests:" << endl;
		int count = 0;
		for (auto it = room->getGuests().begin(); it != room->getGuests().end(); it++) {
			cout <<"\t" <<count << ") " << (*it).name << endl;
			cout << "\t\t" << "Age: "<<(*it).age<<endl;
			cout << "\t\t" << "Register date: " << (*it).date.toString() << endl;
			cout << "\t\t" << "Booked days: " << (*it).days << endl;
			count++;
		}
		cout << "Change fee of room? [y/n]\n>> ";
		string str;
		cin >> str;
		if (str == "y") {
			cout << "New fee of this room ($ per day): ";
			unsigned fee;
			cin >> fee;
			room->setFee(fee);
		}
		return;
	}
	catch (Exception e) {
		cout << "Oops! Sh*t happened: " + e.msg;
		_getch();
		return;
	}
}

void AddGuest(Hotel& hotel) {
	try {
		unsigned id;
		cout << "ID of room to move in: ";
		cin >> id;
		shared_ptr<Room> room = hotel.getRoom(id);
		cout << "New guest "<< endl;
		cout << "\tName: ";
		string name;
		cin >> name;
		cout << "\tAge: ";
		unsigned age;
		cin >> age;
		cout << "\tDate (<dd> <mm> <yyyy>): " ;
		unsigned d, m, y;
		cin >> d >> m >> y;
		cout << "\tBooking days: ";
		unsigned days;
		cin >> days;
		room->addGuest(Person(name, age, Date(y,m,d), days));
	}
	catch (Exception e) {
		cout << "Oops! Sh*t happened: " + e.msg;
		_getch();
		return;
	}
}

void DelGuest(Hotel& hotel) {
	try {
		unsigned id;
		cout << "ID of room to free: ";
		cin >> id;
		shared_ptr<Room> room = hotel.getRoom(id);
		unsigned guestid=0;
		if (room->getType() == "Multi") {
			cout << "ID of guest: ";
			cin >> guestid;
		}
		cout << "Total charge: " << room->free(guestid) << "$" << endl;
		_getch();
	}
	catch (Exception e) {
		cout << "Oops! Sh*t happened: " + e.msg;
		_getch();
		return;
	}
}

int main() {
	Hotel hotel = Hotel("Gym");

	Person p1 = Person("Billy Herrington", 30, Date(2022, 9, 1), 1);
	Person p2 = Person("Van Darkholme", 40, Date(2022, 12, 10), 12);
	Person p3 = Person("Ricardo Millos", 35, Date(2022, 3, 1), 365);
	Person p4 = Person("Mark Wolf", 37, Date(2022, 6, 4), 200);
	Person p5 = Person("Hillbilly Brad", 28, Date(2022, 9, 1), 15);
	Person p6 = Person("Steve Rambo", 29, Date(2022, 10, 1), 21);
	Person p7 = Person("Kazya", 32, Date(2022, 12, 1), 7);

	shared_ptr<Room> sh = make_shared<Single>(10);
	shared_ptr<Room> sh1 = make_shared<Lux>(300);
	shared_ptr<Room> sh2 = make_shared<Multi>(6);
	shared_ptr<Room> sh3 = make_shared<Single>(12);
	shared_ptr<Room> sh4 = make_shared<Lux>(100);

	hotel.addRoom(sh);
	hotel.addRoom(sh1);
	hotel.addRoom(sh2);
	hotel.addRoom(sh3);
	hotel.addRoom(sh4);

	hotel.addGuest(p1, 1);
	hotel.addGuest(p2, 1);
	hotel.addGuest(p3, 0);
	hotel.addGuest(p4, 3);
	hotel.addGuest(p5, 2);
	hotel.addGuest(p6, 2);
	hotel.addGuest(p7, 2);

	Menu menu = Menu();

	Panel rooms = Panel("----< Rooms >----");
	Panel guests = Panel("----< Guests >----");

	Option rooms_option_man = Option("Manage room", [&]() {ManageRoom(hotel); });
	Option rooms_option_add = Option("Add room", [&]() {AddRoom(hotel); });
	Option rooms_option_del = Option("Delete room", [&]() {DelRoom(hotel); });

	Option guests_option_add = Option("New guest", [&]() {AddGuest(hotel); });
	Option guests_option_del = Option("Free room", [&]() {DelGuest(hotel); });

	vector<Option> rooms_opts{ rooms_option_man ,rooms_option_add, rooms_option_del };
	rooms.setOptions(rooms_opts);

	vector<Option> guests_opts{ guests_option_add , guests_option_del };
	guests.setOptions(guests_opts);

	vector<Panel> panels{ rooms , guests};
	menu.setPanels(panels);


	MenuRoutine(menu, hotel, 1);
}
