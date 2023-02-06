#pragma once
//#ifndef _WIN32
//#error SmartMenu library only compatible with WIN32 systems
//#endif //!_WIN32
#include <conio.h>
#include <iostream>
#include <vector>
#include <string>
#include <functional>

namespace menu {

	using namespace std;

	struct Option {
		string str="";
		//template <typename Ret, typename ... Args>
		function<void()> func;
		Option(const string newstr, function<void()> newfunc) {
			str = newstr;
			func = newfunc;
		}
	};

	class Panel {
	private:
		string title = "";
		vector<Option> options;
		unsigned activeId = 0;
	public:
		Panel(string newtitle = "Title") {
			title = newtitle;
		}
		Panel(string newtitle, const vector<Option>& newoptions) {
			title = newtitle;
			options = newoptions;
		}
		void setOptions(const vector<Option>& newoptions) {
			options = newoptions;
		}
		unsigned getSize() {
			return options.size();
		}
		void addOption(const Option& option) {
			options.push_back(option);
		}
		void insertOption(unsigned pos, const Option& option) {
			options.insert(options.begin() + pos, option);
		}
		void delOption(unsigned id) {
			options.erase(options.begin() + id);
		}
		unsigned getOptionId(const string str) {
			unsigned counter = 0;
			for (auto it = options.begin(); it != options.end(); it++) {
				if (it->str == str) return counter;
				counter++;
			}
			return counter;
		}
		unsigned getActiveId() {
			return activeId;
		}
		void setActiveId(unsigned newid) {
			if (newid >= options.size()) return;
			activeId = newid;
		}
		string getTitle() {
			return title;
		}
		void setTitle(const string& newtitle) {
			title = newtitle;
		}
		void run() {
			options.at(activeId).func();
		}
		void print(ostream& out = cout, const string sel="-> ", const string def="") {
			out << title << endl;
			int count = 0;
			for (auto it = options.begin(); it != options.end(); it++) {
				out << (count == activeId ? sel : def) << it->str << endl;
				count++;
			}
		}
	};

	class Menu {
	private:
		string title = "Menu";
		vector<Panel> panels;
		unsigned panelId = 0;
	public:
		Menu(string newtitle="Menu") {
			title = newtitle;
		}
		Menu(string newtitle, const vector<Panel>& newpanels) {
			title = newtitle;
			panels = newpanels;
		}
		Panel& getPanel(unsigned id) {
			return panels.at(id);
		}
		void setPanels(const vector<Panel>& newpanels) {
			panels = newpanels;
		}
		unsigned getSize() {
			return panels.size();
		}
		void addPanel(const Panel& panel) {
			panels.push_back(panel);
		}
		void insertPanel(unsigned pos, const Panel& panel) {
			panels.insert(panels.begin() + pos, panel);
		}
		void delPanel(unsigned id) {
			panels.erase(panels.begin() + id);
		}
		unsigned getPanelId(const string title) {
			unsigned counter = 0;
			for (auto it = panels.begin(); it != panels.end(); it++) {
				if (it->getTitle() == title) return counter;
				counter++;
			}
			return counter;
		}
		unsigned getPanelId() {
			return panelId;
		}
		void setPanelId(unsigned newid) {
			if (newid >= panels.size() || newid<0) return;
			panelId = newid;
		}
		string getTitle() {
			return title;
		}
		void run() {
			panels.at(panelId).run();
		}
		void setTitle(const string& newtitle) {
			title = newtitle;
		}
		void print(ostream& out = cout, const string sel = "-> ", const string def = "") {
			if (!panels.size()) {
				out << "<Empty>" << endl;
			}
			panels.at(panelId).print(out, sel, def);
		}

	};

}