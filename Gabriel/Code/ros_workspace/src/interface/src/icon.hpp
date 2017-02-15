#ifndef ICON_HPP
#define ICON_HPP

#include <gtkmm/enums.h>
#include <gtkmm/image.h>
#include <glibmm/ustring.h>
#include "gui.hpp"
#include <vector>
#include <iostream>


/**
*Icon is the class which updates the icon in the GUI.

*/
class Icon {
public:
	Icon();
	Icon(GUI & , std::string );
	Icon(GUI & , std::string, std::string);
	Icon(GUI & , std::vector<Icon>);
	virtual ~Icon();

	bool on_timeout(int);
	void set_icon(std::string icon_name);
	GUI * my_gui;

	Gtk::Image * icon;
	Gtk::IconSize icon_size;

	Glib::ustring _name;

	Glib::ustring icon_ok = "gtk-apply";
	Glib::ustring icon_error = "gtk-stop";

	std::vector<Icon> icons;




};

#endif