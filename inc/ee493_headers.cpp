#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <unistd.h>
#include <signal.h>
#include <algorithm>
#include <vector>
#include <chrono>

#define PI 3.14159
using namespace cv;
using namespace std;



Mat detectTriangles(Mat image);
void dispImage(Mat image, String title, int loc);

typedef std::chrono::high_resolution_clock Clock;


int colorFront = 'B';
//pos = 1 left
//pos = -1 rightd
int pos = 1;



float resizeRatio = 0.5;
bool ArduinoConnected = true;

int top_cam = 1;
int bot_cam = 1 - top_cam;

time_t start, end_t;
Mat newFrame;
string positionText = "";

VideoWriter writer;
VideoCapture camera;
int iLowH, iHighH, iLowS, iHighS, iLowV, iHighV;
int filterRatio = 1;
int dilateIter = 2;
bool object_exist = false;
int largest_area = 0;
float circleRadius;
float angle;
float slopeLine;
double FPSCounter = 1;
int method, erode_val, dilate_val, filter, threshold_bw;
double k_p = 0.11;
double k_d = 0.12;

int max_contour_area = 0;
int max_contour_area_limit = 900; // Buna gelince duruyor
int contour_area_limit_margin = 550; // Buna gelince gideceği yere dönüyor.

// When to stop where to go. How much of a margin is needed?
int x_margin = 50; //needs to be changed.
int y_height_stop = 34;
int y_height_change_dir = 28;
int max_average_height = 0;
// Triangle coordinates.
vector<int> trianglesDetected;
vector<int> trianglesSelected;

vector<int> order_from_left;
vector<int> order_from_right;

double angle_of_object;


int camError = 00;


int number_of_last_seen_triangles=0;




Point forDebug;

// Initializers for PID.
int goTowardsSlotRasitLastError = 1000;
int allignSlotRasitLastError = 1000;


///
int averageHeightForAllignSlot = 0;
int heightAtWhichAllignSlotStops = 15;

///
bool trianglesFromLeftDeparted = false;
bool trianglesFromRightDeparted = false;


/// 
Mat maskForSecondPart;
Point2f pointForSecondPart;


// Make pos
int x_difference_due_to_angle = pos*4;


/// Board
int areaLimitForColor = 3000;

int elevatorUpValue = 400;
int elevatorDownValue = 400;