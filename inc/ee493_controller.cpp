bool searchColor(Mat img, int color);
void goTowardsObjectMethod(int color, int y_threshold, int speed, int control_divider, int cam_index,  int direction);
void searchColorMethod(int color, int cam_index, int turn_direction);
bool goTowardsSlot(int base_speed, Mat img, int trig_index, int second_slot_index, bool ArduinoConnected, int y_threshold, int turn_rate_divider);
bool goTowardsObject(int base_speed, Mat img, int colorFront, bool ArduinoConnected, int y_threshold, double turn_rate_divider, int direction);
void centerSlotMethod(int first_slot_index, int second_slot_index, int speed, int control_divider, int cam_index);

bool goTowardsObjectRasit(int base_speed, Mat img, int colorFront, bool ArduinoConnected, int y_threshold, double turn_rate_divider, int direction, auto startTime);
void goTowardsObjectMethodRasit(int color, int y_threshold, int speed, int control_divider, int cam_index, int direction);
typedef std::chrono::high_resolution_clock Clock;
bool goTowardsSlotRasit(int base_speed, Mat img, int trig_index, int second_slot_index, bool ArduinoConnected, int y_threshold, int turn_rate_divider);
bool goTowardsSlotRasitAllign(int base_speed, Mat img, int trig_index, int trig_index_2 , bool ArduinoConnected, int y_threshold, int turn_rate_divider );
void goTowardsSlotMethodRasit(int first_slot_index, int second_slot_index, int y_threshold, int speed, int control_divider, int cam_index);
void goTowardsSlotMethodRasitAllign(int first_slot_index, int second_slot_index, int y_threshold, int speed, int control_divider, int cam_index);

int last_error=1000;

void searchTriangleMethod(int first_slot_index, int second_slot_index);


void searchColorMethod(int color, int cam_index, int turn_direction)
{
    switchToCamera(cam_index);
    object_exist = searchColor(newFrame, color);

    while (!object_exist)
    {
        camera >> newFrame;
        resize(newFrame, newFrame, Size(), resizeRatio, resizeRatio, INTER_LINEAR);
        txArduino(driveMotor(turn_direction * 30, turn_direction * -30));
        object_exist = searchColor(newFrame, color);

        //dispImage(newFrame, "search", 0);
    }
    driveMotorForSeconds(0.1, turn_direction * -40, turn_direction * 40);
    txArduino(driveMotor(0, 0));
}

void searchTriangleMethod(int first_slot_index, int second_slot_index)
{
    getFrameFromCamera();
    Point2f trianglePoint = boardSlotRasit(newFrame, first_slot_index, second_slot_index, false, true);
    string turn_direction = "left";
    cout<<"X value for allignment: "<<trianglePoint.x+x_margin<<endl;
    trianglePoint = Point2f(trianglePoint.x- (newFrame.cols / 2) , trianglePoint.y);
    cout<<"X value for allignment: "<<trianglePoint.x+x_margin<<endl;
    if ( (trianglePoint.x +x_margin)>60 ) {
        turn_direction = "left";}
       else if ( (trianglePoint.x +x_margin) < 60 ) {
        turn_direction = "right";}

    int speed = 15;
    while ( abs(trianglePoint.x +x_margin)>60 )
    {
    	cout<<turn_direction<<endl;
        if ( (trianglePoint.x +x_margin)>60 ) {
        turn_direction = "left";
        txArduino(driveMotor(-speed, speed));}
        else if ( (trianglePoint.x +x_margin) < -60 ) {
        turn_direction = "right";
        txArduino(driveMotor(speed, -speed));}

        getFrameFromCamera();
        trianglePoint = boardSlotRasit(newFrame, first_slot_index, second_slot_index, false, false);
        trianglePoint = Point2f(trianglePoint.x- (newFrame.cols / 2) , trianglePoint.y);
    	cout<<"X value for allignment: "<<trianglePoint.x+x_margin<<endl;
        //dispImage(newFrame, "search", 0);
    }
    if (turn_direction == "right"){driveMotorForSeconds(0.1, -40, 40);}
    if (turn_direction == "left"){driveMotorForSeconds(0.1, 40, -40);}
    
    txArduino(driveMotor(0, 0));
}


void goTowardsSlotMethod(int first_slot_index, int second_slot_index, int y_threshold, int speed, int control_divider, int cam_index)
{

    switchToCamera(cam_index);
    //getFrameFromCamera(resizeRatio, true, true);
   
    getFrameFromCamera();
    
    boardSlotRasit(newFrame, first_slot_index, second_slot_index, false, true);

    object_exist = true;
    
    while (object_exist)
    {
        getFrameFromCamera();
        //params 50, 100, 10
        object_exist = goTowardsSlot(speed, removeColor(newFrame, 'Y'), first_slot_index, second_slot_index, ArduinoConnected, y_threshold, control_divider);

    }
}

void goTowardsSlotMethodRasit(int first_slot_index, int second_slot_index, int y_threshold, int speed, int control_divider, int cam_index)
{

    switchToCamera(cam_index);
    //getFrameFromCamera(resizeRatio, true, true);
   
    getFrameFromCamera();
    
    Point slots = boardSlotRasit(newFrame, first_slot_index, second_slot_index, false, true);

    object_exist = true;
    
    while (object_exist and (max_average_height<y_height_stop) )
    {
        getFrameFromCamera();
        //params 50, 100, 10
        object_exist = goTowardsSlotRasit(speed, removeColor(newFrame, 'Y'), first_slot_index, second_slot_index, ArduinoConnected, y_threshold, control_divider);
    }
    max_contour_area = 0;
    txArduino(driveMotor(0, 0));
}




void goTowardsSlotMethodRasitAllign(int first_slot_index, int second_slot_index, int y_threshold, int speed, int control_divider, int cam_index)
{

    switchToCamera(cam_index);
    //getFrameFromCamera(resizeRatio, true, true);
   
    getFrameFromCamera();
    
    Point slots = boardSlotRasit(newFrame, first_slot_index, second_slot_index, false, true);

    object_exist = true;
    
    while (object_exist and (max_average_height<y_height_stop) )
    {
        getFrameFromCamera();
        //params 50, 100, 10
        object_exist = goTowardsSlotRasitAllign(speed, removeColor(newFrame, 'Y'), first_slot_index, second_slot_index, ArduinoConnected, y_threshold, control_divider);
    }
    max_contour_area = 0;
    txArduino(driveMotor(0, 0));
}


void centerSlotMethod(int first_slot_index, int second_slot_index, int speed, int control_divider, int cam_index)
{
    switchToCamera(cam_index);
    //getFrameFromCamera(resizeRatio, true, true);
    int count_until_center = 0;

    getFrameFromCamera();
    
    boardSlotRasit(newFrame, first_slot_index, second_slot_index, false, true);

    //calibrate this value
    while (count_until_center != 30)
    {
        getFrameFromCamera();
        //params 50, 100, 10
        goTowardsSlot(speed, newFrame, first_slot_index, second_slot_index,  ArduinoConnected, 120, control_divider);
        count_until_center++;
    }
    txArduino(driveMotor(0, 0));
}




void goTowardsObjectMethod(int color, int y_threshold, int speed, int control_divider, int cam_index, int direction)
{
    switchToCamera(cam_index);
    object_exist = true;
    while (object_exist)
    {
        /*camera >> newFrame;
        resize(newFrame, newFrame, Size(), resizeRatio, resizeRatio, INTER_LINEAR);
        */
        getFrameFromCamera();
        object_exist = goTowardsObject(speed, newFrame, color, ArduinoConnected, y_threshold, control_divider, direction);
    }
    driveMotorForSeconds(0.3, 50, 50);

    /*
    if (angle_average > 0)
    {
        driveMotorForSeconds(0.5, 0, 70);
    }

    else if (angle_average < 0)
    {
        driveMotorForSeconds(0.5, 70, 0);
    }
    */
}



void goTowardsObjectMethodRasit(int color, int y_threshold, int speed, int control_divider, int cam_index, int direction)
{
    switchToCamera(cam_index);
    object_exist = true;
	auto startTime = Clock::now();
	Mat deneme;
    while (object_exist)
    {
        /*camera >> newFrame;
        resize(newFrame, newFrame, Size(), resizeRatio, resizeRatio, INTER_LINEAR);
        */
        getFrameFromCamera();
        object_exist = goTowardsObjectRasit(speed, newFrame, color, ArduinoConnected, y_threshold, control_divider, direction, startTime);
    }
    driveMotorForSeconds(0.3, 50, 50);


    /*
    if (angle_average > 0)
    {
        driveMotorForSeconds(0.5, 0, 70);
    }

    else if (angle_average < 0)
    {
        driveMotorForSeconds(0.5, 70, 0);
    }
    */
}


bool goTowardsObjectRasit(int base_speed, Mat img, int colorFront, bool ArduinoConnected, int y_threshold, double turn_rate_divider, int direction, auto startTime){
	//tracks object with an image taken from camera
    //uses global angle variable
    
	long long int tenMs = 10E6; // tenMs
	double seconds=5;
	long long int totalSeconds = seconds*1E9;
	
    Point2f center = drawCenterLine(img, colorFront);
    //bool object_exist = true;
    int mid_y = img.rows / 2;
    int mid_x = img.cols / 2;
    cout<<"cols:"<<img.cols<<endl;
    Point2f mid(mid_x, mid_y);
    center = center - mid;

    //imshow("img",img);
    double k_d=0.2;
    int error = center.x;
    int addMargin = 70;

    auto currentTime=Clock::now();
    long long int counter=0;
    double logicCounter=0;
    double extra_speed = 0;
    double speed = 0;
    cout<<(std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - startTime).count() )<<" ns has passed."<<endl;
    while( (totalSeconds-counter) > (std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - startTime).count())   ){
    	counter += tenMs;
    	logicCounter++;
    }
    //logicCounter=100-logicCounter;
    if (logicCounter>(seconds*10/4) && logicCounter<(100*seconds)){
    	/*
    	double fraction = logicCounter/(100*seconds)*200; // max of this is always 200.
    	extra_speed = (-10+fraction*0.1) ;
    	cout<<extra_speed<<": extra"<<endl;
		cout<<logicCounter<<": logicCounter"<<endl;
		speed = 0;
		*/
		error=center.x+addMargin;
		//speed = (center.x+addMargin) / turn_rate_divider;// May need to add this. + (error-last_error)*k_d;
		speed = (center.x+addMargin) / turn_rate_divider + (error-last_error)*k_d;
    } else {
    		speed = (center.x) / turn_rate_divider + (error-last_error)*k_d;

    }
    last_error = error;

    //string txString;
    
    if (abs(center.x) < 550)
    {
    	if (center.x<0){
			txArduino(driveMotor(base_speed + speed + extra_speed, base_speed - speed ));
			//txArduino(driveMotor(base_speed + speed, base_speed - speed + extra_speed));
    	}
    	else{
        	txArduino(driveMotor(base_speed + speed + extra_speed, base_speed - speed));
    	}
    }

    else
    {
        txArduino(driveMotor(0, 0));
    }
    cout << endl
         << center.y << endl;

    if ( (direction == 1 ) && center.y > y_threshold)
    {
        //driveMotorForSeconds(1.5, 100, 100);
        txArduino(driveMotor(0, 0));
        object_exist = false;
    }

    if ( (direction == -1 ) && center.y < y_threshold)
    {
        //driveMotorForSeconds(1.5, 100, 100);
        txArduino(driveMotor(0, 0));
        object_exist = false;
    }


    return object_exist;
}

int goTowardsSlotLastError = 1000;
bool goTowardsSlot(int base_speed, Mat img, int trig_index, int trig_index_2 , bool ArduinoConnected, int y_threshold, int turn_rate_divider )
{
    
    
    //Point2f center = getBoardSlot(img, trig_index, trig_index_2);
    Point2f center = boardSlotRasit(img, trig_index, trig_index_2, false, false);
    double k_d = 0.1;
    //  dispImage(img, "trig", 0);
    int mid_y = img.rows / 2;
    int mid_x = img.cols / 2;
    Point2f test(mid_x, mid_y);
    center = center - test;
    string txString;
    int speed = 0;
    int error = center.x;
    if (goTowardsSlotLastError == 1000){
    //speed = (center.x) / turn_rate_divider;
    speed = (center.x)*k_p;
	} else {
		//speed = (center.x) / turn_rate_divider + k_d*(error-last_error);
		speed = (center.x)*k_p + k_d*(error-last_error);
	}
	cout<<"error= "<<error<<"last_error= "<<last_error<<endl;
	last_error=error;

    if (abs(center.x) < 550)
    {
        txArduino(driveMotor(base_speed + speed, base_speed - speed));
    }
    else
    {
        txArduino(driveMotor(0, 0));
    }

    cout <<"y value: "<< center.y <<endl;
    if ( center.y > y_threshold)
    {
        //driveMotorForSeconds(1.5, 100, 100);
        txArduino(driveMotor(0, 0));
        object_exist = false;
    }
    return object_exist;
}

bool goTowardsSlotRasit(int base_speed, Mat img, int trig_index, int trig_index_2 , bool ArduinoConnected, int y_threshold, int turn_rate_divider )
{
    
    //y_threshold = -20;
    //Point2f center = getBoardSlot(img, trig_index, trig_index_2);
    Point2f center = boardSlotRasit(img, trig_index, trig_index_2, false, false);
    forDebug = Point2f(center.x,center.y);
    int camError = 10;
	//double k_d = 0.1;
    //  dispImage(img, "trig", 0);
    int mid_y = img.rows / 2;
    int mid_x = img.cols / 2;
    Point2f test(mid_x, mid_y);
    center = center - test;
    string txString;
    int speed = 0;
    int error = center.x + camError;
    if (max_average_height<y_height_change_dir) {error += x_margin;cout<<"margin added."<<endl;}
    else {cout<<"changed directions"<<endl;}


    if (goTowardsSlotLastError == 1000){
    //speed = (center.x) / turn_rate_divider;
    speed = (error)*k_p;
	} else {
		//speed = (center.x) / turn_rate_divider + k_d*(error-last_error);
		speed = (error)*k_p + k_d*(error-last_error);
	}
	cout<<"error= "<<error<<"last_error= "<<last_error<<endl;
	last_error=error;

    if (abs(center.x) < 550)
    {
        txArduino(driveMotor(base_speed + speed, base_speed - speed));
    }
    else
    {
        txArduino(driveMotor(0, 0));
    }

    cout <<"y value: "<< center.y <<endl;
    if ( center.y > y_threshold)
    {
        //driveMotorForSeconds(1.5, 100, 100);
        txArduino(driveMotor(0, 0));
        object_exist = false;
    }

    //if ( abs(error)<10 ) object_exist = false;


    return object_exist;
}


bool goTowardsSlotRasitAllign(int base_speed, Mat img, int trig_index, int trig_index_2 , bool ArduinoConnected, int y_threshold, int turn_rate_divider )
{
    double k_p = 0.06;
	double k_d = 0.1;
    //y_threshold = -20;
    //Point2f center = getBoardSlot(img, trig_index, trig_index_2);
    Point2f center = boardSlotRasit(img, trig_index, trig_index_2, false, false);
    forDebug = Point2f(center.x,center.y);
    
	//double k_d = 0.1;
    //  dispImage(img, "trig", 0);
    int mid_y = img.rows / 2;
    int mid_x = img.cols / 2;
    Point2f test(mid_x, mid_y);
    center = center - test;
    string txString;
    int speed = 0;
    int error = center.x + camError;
    if (max_average_height<y_height_change_dir) {error += x_margin;cout<<"hey"<<endl;}
    else {cout<<"changed directions"<<endl;}


    if (goTowardsSlotLastError == 1000){
    //speed = (center.x) / turn_rate_divider;
    speed = (error)*k_p;
	} else {
		//speed = (center.x) / turn_rate_divider + k_d*(error-last_error);
		speed = (error)*k_p + k_d*(error-last_error);
	}
	cout<<"error= "<<error<<"last_error= "<<last_error<<endl;
	last_error=error;

    if (abs(center.x) < 550)
    {
    	if (abs(center.x) < 0){ txArduino(driveMotor(-15,15));}
    	else if (abs(center.x) > 0){ txArduino(driveMotor(15,-15));}
    }
    else
    {
        txArduino(driveMotor(0, 0));
    }

    cout <<"y value: "<< center.y <<endl;
    if ( center.y > y_threshold)
    {
        //driveMotorForSeconds(1.5, 100, 100);
        txArduino(driveMotor(0, 0));
        object_exist = false;
    }

    if ( abs(error)<20 ) object_exist = false;


    return object_exist;
}



bool searchColor(Mat img, int color)
{
    //tracks object with an image taken from camera
    //uses global angle variable

    Point2f center = drawCenterLine(img, color);

    bool object_exist = false;
    int mid_y = img.rows / 2;
    int mid_x = img.cols / 2;
    Point2f test(mid_x, mid_y);
    center = center - test;

    if ((center.x > -80 and center.x < 80) and (center.y < 80 and center.y > -20))
    {
        object_exist = true;
    }
    return object_exist;
}

bool goTowardsObject(int base_speed, Mat img, int colorFront, bool ArduinoConnected, int y_threshold, double turn_rate_divider, int direction)
{

    //tracks object with an image taken from camera
    //uses global angle variable

    Point2f center = drawCenterLine(img, colorFront);
    //bool object_exist = true;
    int mid_y = img.rows / 2;
    int mid_x = img.cols / 2;
    Point2f mid(mid_x, mid_y);
    center = center - mid;

    //string txString;
    double speed = (center.x) / turn_rate_divider;
    if (abs(center.x) < 550)
    {
        txArduino(driveMotor(base_speed + speed, base_speed - speed));
    }

    else
    {
        txArduino(driveMotor(0, 0));
    }
    cout << endl
         << center.y << endl;

    if ( (direction == 1 ) && center.y > y_threshold)
    {
        //driveMotorForSeconds(1.5, 100, 100);
        txArduino(driveMotor(0, 0));
        object_exist = false;
    }

    if ( (direction == -1 ) && center.y < y_threshold)
    {
        //driveMotorForSeconds(1.5, 100, 100);
        txArduino(driveMotor(0, 0));
        object_exist = false;
    }
    return object_exist;
}