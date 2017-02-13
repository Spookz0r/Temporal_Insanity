#include "button.hpp"
#include <sstream>
#include <iostream>

/*!
    * \brief Constructor
    *
    
    */
Buttons::Buttons(){
	
}

/*!
    * \brief Constructor with inputs
    *
    * Creates a pointer to the button object in the GUI and assigns an event handler when clicked
    * 
    * \n input: GUI reference, Name of the button in the GUI, message value to publish when pressed
    * \n output: 
    
    */
Buttons::Buttons(GUI & new_gui,std::string new_name, int button_message){
	my_gui = &new_gui;
	name = new_name;
	my_gui->refBuilder->get_widget(name,my_button);
	int_message.data = button_message;
	my_button->signal_clicked().connect( sigc::mem_fun(*this, &Buttons::on_button_clicked) );
	//init_action();
}

/*!
    * \brief Constructor for toggle button
    *
    * Creates a pointer to the button object in the GUI and assigns an event handler when clicked 
    * and if it is a toggle button.
    * \n input: GUI reference, Name of the button in the GUI, message value to publish when pressed
    * \n output: 
    
    */
Buttons::Buttons(GUI & new_gui,std::string new_name, int button_message,bool toggle ){
	my_gui = &new_gui;
	name = new_name;
	my_gui->refBuilder->get_widget(name,my_button);
	int_message.data = button_message;
	my_button->signal_clicked().connect( sigc::mem_fun(*this, &Buttons::on_button_clicked) );
	isToggle = toggle;
	//init_action();
	

}

Buttons::Buttons(GUI & new_gui,std::string new_name, int button_message,int direction_value ){
	my_gui = &new_gui;
	name = new_name;
	my_gui->refBuilder->get_widget(name,my_button);
	int_message.data = button_message;
	my_button->signal_clicked().connect( sigc::mem_fun(*this, &Buttons::on_button_clicked) );

	direction_choice.data = direction_value;
	hasDirection = true;
	//init_action();
	

}
/*!
    * \brief Destructor
    *

    
    */
Buttons::~Buttons(){

}

/*!
    * \brief Action when button pressed
    *
    * Depending on the name it publishes the message of the button. 
    * Also prints in the GUI that the button has been pressed.
    * 
    * \n input: 
    * \n output: 
    
    */
void Buttons::on_button_clicked(){

	
	// MESSAGE TO LOG
	std::ostringstream os;
	//os << this->name << " was pushed.";
	//message = os.str();
		
	//std::cerr << message << std::endl;
	//my_gui->print_to_log(message);
	if(this->name == "button_image_switch"){
		++my_gui->image_to_show;
		//if(my_gui->image_to_show > my_gui->images.size()) my_gui->image_to_show = 0;
		if(my_gui->image_to_show > 2) my_gui->image_to_show = 0;
		my_gui->right_image_label->set_label(my_gui->image_labels[my_gui->image_to_show]);
		os << "Switched view to: " << my_gui->image_labels[my_gui->image_to_show] << "!" ;
		
	}
	if(has_publisher){
		if(isToggle && this->name != "leftsignal"){
			if(int_message.data ==1) int_message.data = 0;
			else if(int_message.data ==0) int_message.data = 1;
		}
		else{
			std_msgs::String msg2;
		    std::stringstream ss;
		    ss << name;
		    msg2.data = ss.str();
		 	//button_pub.publish(msg2);
		 	
	 	}
	 	if(name == "accelerate_button" || name == "reverse_button"){
	 		if(my_gui->manual_control_msg.data == 1 ){
		 		
		 		my_gui->_motor.data += int_message.data;
		 		if(my_gui->_motor.data > my_gui->max_grade) my_gui->_motor.data = my_gui->max_grade;
		 		if(my_gui->_motor.data < -my_gui->max_grade) my_gui->_motor.data = -my_gui->max_grade;
		 		button_pub.publish(my_gui->_motor);
		 		if(name == "accelerate_button") os << "Sending accelerate to motor. Value: " << static_cast<int16_t>(my_gui->_motor.data);
		 		if(name == "reverse_button") os << "Sending reverse/slow down to motor. Value: " << static_cast<int16_t>(my_gui->_motor.data);
			}

	 	}
	 	else if(name == "stop_button"){
	 		my_gui->_motor.data = 0;
	 		button_pub.publish(my_gui->_motor);
	 		os << "Sending stop to motor.";
	 	}
	 	else if(name == "left_button" || name == "right_button"){
	 		if(my_gui->manual_control_msg.data == 1 ){

		 		my_gui->_servo.data += int_message.data;
		 		//std::cerr << "value is: " << static_cast<int16_t>(my_gui->_servo.data )<< std::endl;
		 		if(my_gui->_servo.data > my_gui->max_grade) my_gui->_servo.data = my_gui->max_grade;
		 		if(my_gui->_servo.data < -my_gui->max_grade) my_gui->_servo.data = -my_gui->max_grade;
		 		std::cerr << "Sending to servo: " << static_cast<int16_t>(my_gui->_servo.data )<< std::endl;
		 		os << "Sending to servo: " << static_cast<int16_t>(my_gui->_servo.data );
		 		button_pub.publish(my_gui->_servo);
		 	}
	 	}

	 	else{
	 		button_pub.publish(int_message);
	 		os << "Publishing " << static_cast<int16_t>(int_message.data);
	 	
	 	}
	 	if(hasDirection){
	 		 pub_direction_choice.publish(direction_choice);
	 		 std::cerr << "Publishing direction choice: " << direction_choice << std::endl;
	 		}
	 	//std::cerr << "published message :" << name << std::endl;
	 }

	 message = os.str();
	 if(message.length() > 0){
		 my_gui->print_to_log(message);
		}
}


void Buttons::chatterCallback(const std_msgs::String::ConstPtr& msg){
	my_gui->print_to_log(msg->data.c_str());

	
}