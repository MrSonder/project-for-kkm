#include "inc/ee493_headers.cpp"
#include "inc/ee493_game_algo.cpp"
#include "inc/ee493_arduino.cpp"
#include "inc/ee493_opencv_lowlevel.cpp"
#include "inc/ee493_opencv.cpp"
#include "inc/ee493_controller.cpp"
#include "inc/ee493_board.cpp"
#include "inc/ee493_future.cpp"
#include <typeinfo>

int bot_cam_index = 1;
int rear_cam_index = 0;
int top_cam_index = 2;
int new_cam_index = 2;

int y_threshold;
int speed;
int turn_ratio;


/* NOTES:
    In goTowardsSlot, goTowardsSlotLastError is checked but not changed.
    This is true for all of these functions.
    As a result, d is not utilized.
    Fix this after implementing first part.

*/


/* check before start:
    camera index
    camera lens
    arduino connection ( ls /dev/ttyUSB0)
    color filters
*/


//typedef std::chrono::high_resolution_clock Clock;



typedef std::chrono::high_resolution_clock Clock;



int getAverageAngle(int color);
bool isPickedUp();
void getBoardInfo();
void makeAMove(int pos, int color);
void setObject(int object);
void birElOyna(int color, int pos);
void birElOynaRasit(int color, int pos);
void birElOynaRasitTam(int color, int pos);

void setTriangle(int triangle);

int main(int argc, char *argv[])
{
    time(&start);
    signal(SIGINT, exiting);
    cout << "\033[2J"
         << "\033[1;31mArduinoConnected: " << ArduinoConnected << endl;

    //cout<< arg<<endl;
    int colorFront = 'B';
    //pos = 1 left
    //pos = -1 rightd
    int pos = 1;
    /*
    auto t1 = Clock::now();
    Mat image = imread("img/red.jpg");
    //Mat mask = imread("yellow_mask.png", CV_LOAD_IMAGE_GRAYSCALE);
    imshow("hll",image);
    cout << typeid(t1).name() << endl;
    waitKey(0);


    auto t2 = Clock::now();
    std::cout << "Delta t2-t1: " 
              << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()
              << " nanoseconds" << std::endl;
            
    long int b1= 1E9;
              
    std::cout<< (b1>std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() )<<endl;
    */
if (argc ==2){
char inputChar = argv[1][0];
if (inputChar == 'c'){
    cout<<"arg entered."<<endl;
    for (int i=0;i<3;i++){
        while(true){
            switchToCamera(i);
            getFrameFromCamera();
            imshow("new",newFrame);
            if (waitKey(1)==27){break;}
        }
    }
    return 0;
}
    
    
}


if (argc ==2){
    
    int inputInt = atoi(argv[1]);
    cout<<"arg int entered."<<endl;
    //k_p = inputInt/100.0;
    //cout<<"k_p is: "<<k_p<<endl;
    cout<<"x_margin is: "<<inputInt<<endl;
    x_margin=inputInt;
    switchToCamera(new_cam_index);

    birElOynaRasit('B', pos);
    
    
    cout<<"arg int entered."<<endl;
    return 0;
}



while(true){
switchToCamera(new_cam_index);
/*
    calibrateThreshold(newFrame, 'B');

    return 0;
*/
    birElOynaRasit('B', pos);
    break;
}



    return 0;
}

void birElOynaRasit(int color, int pos){
    
    /*
    setObject('O');
    searchColorMethod(color, bot_cam_index, -1*pos );
    goTowardsObjectMethodRasit(color, y_threshold, speed, turn_ratio, bot_cam_index, 1);
    */
    
    while(true){
    
    //txArduino(driveStepper(500, 'U', '1'));
    //cout<<"sleep for 6 sec"<<endl;
    //usleep(6E6);
    setObject('T');
    int x_axis=0;
    int x_axis_2=1;
    
    if (x_axis == 0  and x_axis_2==1) {setTriangle(0);}
    else if (x_axis ==1 and x_axis_2==1) {setTriangle(1);}
    else if (x_axis ==1 and x_axis_2==2) {setTriangle(2);}
    else if (x_axis ==2 and x_axis_2==2) {setTriangle(3);}
    else if (x_axis ==2 and x_axis_2==3) {setTriangle(4);}
    else if (x_axis ==3 and x_axis_2==3) {setTriangle(5);}
    else if (x_axis ==3 and x_axis_2==4) {setTriangle(6);}
    else if (x_axis ==4 and x_axis_2==4) {setTriangle(7);}
    else if (x_axis ==4 and x_axis_2==5) {setTriangle(8);}
    else if (x_axis ==5 and x_axis_2==5) {setTriangle(9);}
    else if (x_axis ==5 and x_axis_2==6) {setTriangle(10);}
    
    

    //Do it in two pieces!
    allignSlotMethod(x_axis, x_axis_2, y_threshold, speed, turn_ratio, new_cam_index);

    goTowardsSlotAfterAllignSlotMethod(x_axis, x_axis_2, y_threshold, speed, turn_ratio, new_cam_index); // used to be -1 * speed

    cout<<"DURDUM!"<<endl;
    driveMotorForSeconds(1.8, 40, 40);
    cout<<"Kör gittim!"<<endl;

    break;

    }
    
}


class exponentialAdd{

};


class myTimer {
    public:
        
        void start(auto startingTime) {
            auto startTime = Clock::now();
        }

        auto elapsedTime(auto startingTime) {
            return std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now() - startingTime).count();
        }

        bool isTimeout(auto seconds, auto startingTime) {
            return (seconds >= (elapsedTime(startingTime)) );
        }
};





int getAverageAngle(int color)
{
    int total_angle = 0;
    int avg_ang;
    angle = 0;
    switchToCamera(top_cam_index);
    int i = 0;
    while (i < 5)
    {
        getFrameFromCamera(resizeRatio, true, false);
        drawCenterLine(newFrame, color);
        total_angle = total_angle + angle;
        i++;
    }
    avg_ang = total_angle / i;
    cout << avg_ang << endl;
    return avg_ang;
}


void birElOynaRasitTam(int color, int pos){

    driveMotorForSeconds(0.2, -50 * pos, 50 * pos);

    do
    {

        setObject('O');
        searchColorMethod(color, bot_cam_index, -1 *pos);
        goTowardsObjectMethod(color, y_threshold, speed, turn_ratio, bot_cam_index, 1);


        txArduino(driveStepper(100, 'U', '1'));
        setObject('F');
        searchColorMethod('P', rear_cam_index, -1 * pos);

        goTowardsObjectMethod('P', y_threshold, -1 * speed, turn_ratio, rear_cam_index, 1);

        txArduino(driveStepper(0, 'U', '0'));

    } while (!(isPickedUp()));

    usleep(0.5 * 1e6);
    txArduino(driveStepper(100, 'U', '1'));
    usleep(1.5 * 1e6);
    setObject('G');

    driveMotorForSeconds(0.7, -70 * pos, 70 * pos);
    goTowardsObjectMethod('G', y_threshold, -1 * speed, turn_ratio, rear_cam_index, 1);
    

    driveMotorForSeconds(0.7, 68* pos, -68 * pos);

    txArduino(driveStepper(400, 'U', '1'));
    usleep(1E6);
    switchToCamera(new_cam_index);

    getFrameFromCamera(resizeRatio, true, false);
    Point loc = GiveNextMove(setObjectLocations(newFrame));
    cout << loc << endl;

    //txArduino(driveStepper(0, 'U', '0'));
    //usleep(5 * 1e6);
    //txArduino(driveStepper(700 + loc.y * 200, 'U', '1'));
    //txArduino(driveStepper(400, 'U', '1'));
    //usleep(3.5 * 1e6);

    int x_axis;
    int x_axis_2;

    if (loc.y % 2 == 0)
    {
        x_axis = loc.x + loc.y;
        x_axis_2 = x_axis + 1;
    }

    else if (loc.y % 2 == 1)
    {
        x_axis = loc.x + loc.y;
        x_axis_2 = x_axis;
    }

    //x_axis = 0;
    //x_axis_2 = 1;
    setTriangle(0);
    x_margin = 20;

    //added
	if (x_axis == 0  and x_axis_2==1) {setTriangle(0);}
    else if (x_axis ==1 and x_axis_2==1) {setTriangle(1);}
    else if (x_axis ==1 and x_axis_2==2) {setTriangle(2);}
    else if (x_axis ==2 and x_axis_2==2) {setTriangle(3);}
    else if (x_axis ==2 and x_axis_2==3) {setTriangle(4);}
    else if (x_axis ==3 and x_axis_2==3) {setTriangle(5);}
    else if (x_axis ==3 and x_axis_2==4) {setTriangle(6);}
    else if (x_axis ==4 and x_axis_2==4) {setTriangle(7);}
    else if (x_axis ==4 and x_axis_2==5) {setTriangle(8);}
    else if (x_axis ==5 and x_axis_2==5) {setTriangle(9);}
    else if (x_axis ==5 and x_axis_2==6) {setTriangle(10);}




    setObject('T');
    y_height_stop = 9;
    speed = 28;
    goTowardsSlotMethodRasit(x_axis, x_axis_2, y_threshold, speed, turn_ratio, new_cam_index);
    setObject('T');
    y_height_stop = 36;
    goTowardsSlotMethodRasit(x_axis, x_axis_2, y_threshold, speed, turn_ratio, new_cam_index);



    cout<<"DURDUM!"<<endl;
    driveMotorForSeconds(2, 40, 40);
    cout<<"Kör gittim!"<<endl;


    txArduino(driveStepper(400, 'D', '1'));
    usleep(6 * 1e6);
    driveMotorForSeconds(1.5, -70, -70);
    txArduino(driveStepper(0, 'D', '0'));

    setObject('F');
    searchColorMethod('P', rear_cam_index, 1*pos);
    goTowardsObjectMethod('P', y_threshold, -1 * speed, turn_ratio, rear_cam_index, 1);

}

void birElOyna(int color, int pos)
{
    driveMotorForSeconds(0.2, -50 * pos, 50 * pos);

    do
    {
        setObject('A');
        searchColorMethod(color, bot_cam_index, -1*pos );
        goTowardsObjectMethod(color, y_threshold, speed, turn_ratio, bot_cam_index, 1);

        int obj_ang = getAverageAngle(color);

        setObject('O');
        searchColorMethod(color, bot_cam_index, -1 *pos);
        goTowardsObjectMethod(color, y_threshold, speed, turn_ratio, bot_cam_index, 1);

        if (obj_ang < 75 && obj_ang > 0)
        {
            driveMotorForSeconds(0.25, -50, 50);
            driveMotorForSeconds(0.6, 70, 70);
        }
        else if (obj_ang > -75 && obj_ang < 0)
        {
            driveMotorForSeconds(0.25, 50, -50);
            driveMotorForSeconds(0.6, 70, 70);
        }

        txArduino(driveStepper(100, 'U', '1'));
        setObject('F');
        searchColorMethod('P', rear_cam_index, -1 * pos);

        goTowardsObjectMethod('P', y_threshold, -1 * speed, turn_ratio, rear_cam_index, 1);

        txArduino(driveStepper(0, 'U', '0'));

    } while (!(isPickedUp()));

    usleep(0.5 * 1e6);
    txArduino(driveStepper(100, 'U', '1'));
    usleep(1.5 * 1e6);
    setObject('G');

    driveMotorForSeconds(0.7, -70 * pos, 70 * pos);
    goTowardsObjectMethod('G', y_threshold, -1 * speed, turn_ratio, rear_cam_index, 1);
    
    
    
    driveMotorForSeconds(0.7, -65* pos, 65 * pos);

    switchToCamera(rear_cam_index);

    getFrameFromCamera(resizeRatio, true, false);
    Point loc = GiveNextMove(setObjectLocations(newFrame));
    cout << loc << endl;

    txArduino(driveStepper(0, 'U', '0'));
    usleep(1.5 * 1e6);
    txArduino(driveStepper(700 + loc.y * 200, 'U', '1'));
    usleep(3.5 * 1e6);

    int x_axis;
    int x_axis_2;

    if (loc.y % 2 == 0)
    {
        x_axis = loc.x + loc.y;
        x_axis_2 = x_axis + 1;
    }

    else if (loc.y % 2 == 1)
    {
        x_axis = loc.x + loc.y;
        x_axis_2 = x_axis;
    }

    setObject('T');
    goTowardsSlotMethod(x_axis, x_axis_2, y_threshold, -1 * speed, turn_ratio, rear_cam_index);


    driveMotorForSeconds(1.50, 60, -60);
    driveMotorForSeconds(1.45, 60, 60);
    txArduino(driveStepper(600, 'D', '1'));
    usleep(6 * 1e6);
    driveMotorForSeconds(1.5, -70, -70);
    txArduino(driveStepper(0, 'D', '0'));

    setObject('F');
    searchColorMethod('P', rear_cam_index, 1*pos);
    goTowardsObjectMethod('P', y_threshold, -1 * speed, turn_ratio, rear_cam_index, 1);

}


bool isPickedUp()
{
    switchToCamera(bot_cam_index);
    getFrameFromCamera();
    Mat mask = imread("yellow_mask.png", CV_LOAD_IMAGE_GRAYSCALE);
    //cout<<img.Size()<<endl;
    resize(mask, mask, Size(newFrame.cols, newFrame.rows), 0, 0, INTER_LINEAR);

    Mat img_out;

    int i = 0;
    int counter = 0;
    while (i < 10)
    {
        getFrameFromCamera();
        newFrame.copyTo(img_out, mask);
        Mat thresh = thresholdImage(img_out, 'Y', false);
        dispImage(thresh, "mask", 0);
        double ratio = sum(thresh)[0] / sum(mask)[0];

        if (ratio > 0.6)
        {
            counter++;
        }
        i++;
    }

    if (counter > 6)
    {
        cout << endl
             << "picked" << endl;
        return true;
    }

    return false;
}

void setObject(int object)
{
    switch (object)
    {
    case int('O'):
        y_threshold = 93;
        speed = 70;
        turn_ratio = 7.5;
        break;

    case int('T'):
        y_threshold = 65;
        speed = 33; //60
        turn_ratio = 15;
        break;

    case int('K'):
        y_threshold = 65;
        speed = 45;
        turn_ratio = 15;
        break;

    case int('M'):
        y_threshold = 100;
        speed = 45;
        turn_ratio = 16;
        break;

    case int('C'): //center triangles
        y_threshold = 70;
        speed = 40;
        turn_ratio = 10;
        break;

    case int('F'):
        y_threshold = 100;
        speed = 80;
        turn_ratio = 7;
        break;

    case int('G'):
        y_threshold = 110;
        speed = 70;
        turn_ratio = 7;
        break;

    case int('R'):
        y_threshold = 100;
        speed = 50;
        turn_ratio = 9;
        break;

    case int('A'):
        y_threshold = 35;
        speed = 70;
        turn_ratio = 7.5;
        break;
    }
}
//void setTriangle(int triangle){x_margin=triangle*5;}

void setTriangle(int triangle){
    switch (triangle){
        case 0: 
            x_margin = 20;
            break;
        case 1:
            x_margin = 20;
            break;
        case 2:
            x_margin=25;
            break;
        case 3:
            x_margin=30;
            break;
        case 4:
            x_margin=40;
            break;
        case 5:
            x_margin=35;
            break;
        case 6:
            x_margin=45;
            break;
        case 7:
            x_margin=50;
            break;
        case 8:
            x_margin=55;
            break;
        case 9:
            x_margin=65;
            break;
        case 10:
            x_margin=75;
            break;
    }
}
