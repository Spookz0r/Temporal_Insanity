#include "icon.hpp"

/*!
    * \brief Constructor
    *

    
    */
Icon::Icon(){}


/*!
    * \brief Constructor with name of icon
    *
    * Write something about the function such as input/output, what it does
    * etc...
    * \n input:
    * \n output: 
    
    */
Icon::Icon(GUI & gui, std::string name){

	my_gui = &gui;
	_name = name;
	my_gui->refBuilder->get_widget(_name, icon);
	icon->set_from_icon_name(icon_error,icon_size);
}


Icon::Icon(GUI & gui, std::vector<Icon> icon_vector){


	my_gui = &gui;
	_name = "icons";
	icons = icon_vector;
}
Icon::~Icon(){}



bool Icon::on_timeout(int timer_number){

	//std::cerr << "icon timer" << std::endl;

	for(int i = 0; i < icons.size(); ++i){
		if(my_gui->status[i]){
			icons[i].set_icon(icon_ok);
			my_gui->status[i] = false;
		}
		else{
			icons[i].set_icon(icon_error);
		}
	}
	
	return true;
}

void Icon::set_icon(std::string icon_name){
	//std::cerr << "updating icon for: " << _name << std::endl;
	icon->set_from_icon_name(icon_name,icon_size);
}
