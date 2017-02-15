#include "image_show.hpp"


Image_show::Image_show(){};

Image_show::Image_show(GUI & new_gui, std::string name){
	//set the image to the one in the GUI
	my_gui = &new_gui;
	my_gui->refBuilder->get_widget(name, drawingImage);
	//load default image
	_image = Gdk::Pixbuf::create_from_file("src/interface/src/images/no_signal.jpg");
	
	my_gui->images[0] = _image; // original
	drawingImage->set(_image);
	drawingImage->queue_draw();

	dispatcher.connect(sigc::mem_fun(*this,&Image_show::dispatch_notification));

}

Image_show::~Image_show(){

}

void Image_show::set_image(std::string name){

	_image = Gdk::Pixbuf::create_from_file(name);
	drawingImage->set(_image);
	drawingImage->queue_draw();

	//my_slot= sigc::bind(sigc::mem_fun(*this, &Image_show::on_timeout), m_timer_number);
	//conn = Glib::signal_timeout().connect(my_slot, timeout_value);
}


void Image_show::imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
	// If got message set the camera status icon to OK;
	if(counter >= 1){
	try{
	if(msg->data.size()> 0){
		//my_gui->status[2] = true;

		// start reading image
		cv_bridge::CvImagePtr cv_ptr;
	  	cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
	  	cv_ptr->image.copyTo(my_gui->camera_image);  //Copy the image to gui for storage
		cv::cvtColor(cv_ptr->image, image_1, CV_BGR2RGB);
		cv::resize(image_1,image_0,cv::Size(640,480));
		_image = Gdk::Pixbuf::create_from_data(image_0.data, Gdk::COLORSPACE_RGB, false, 8, image_0.cols, image_0.rows,image_0.step);

		my_gui->images[image_id] = _image;
		dispatcher.emit();
		}
	}
	catch(const Gio::ResourceError& ex)
	  {
	    std::cerr << "ResourceError: " << ex.what() << std::endl;
	  }
	catch(const Gdk::PixbufError& ex)
	  {
	    std::cerr << "PixbufError: " << ex.what() << std::endl;
	  }
	  counter = 0;
	}
	++counter;
	
	

}


bool Image_show::on_timeout(int timer){
	Glib::RefPtr<Gdk::Pixbuf> tempBuf = _image;
	std::cerr << "###############timer" << std::endl;
	myMutex.lock();	
		drawingImage->set(tempBuf);
		drawingImage->queue_draw();
	myMutex.unlock();


	return true;
}

void Image_show::dispatch_notification(){
	
		myMutex.lock();
		

		if(switchable){

		 	drawingImage->set(my_gui->images[my_gui->image_to_show]);

		}
		else{
			if(_old_image != _image){
				drawingImage->set(_image);
				_old_image = _image;
			}
		}
		drawingImage->queue_draw();

		myMutex.unlock();
}
