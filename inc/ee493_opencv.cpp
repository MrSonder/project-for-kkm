
void getPanaroma();
void getTriangleArray(Mat image);
void calibrateDoubleCamera();
Mat getTriangleContours(Mat image, int trig_index_1, int trig_index_2);
Point2f getBoardSlot(Mat img, int trig_index_1, int trig_intex_2, Mat& binary_img, bool inverse = false);
Point2f boardForAllignSlot(Mat img, int trig_index_1, int trig_intex_2, bool inverse, bool first_call);

int getFPS();

//Mat detectTriangles(Mat image);

Point2f drawCenterLine(Mat imageIn, int colorFront);
Mat getObjectOfColor(Mat image, int colorFront);
Mat getLargestArea(Mat image, int object);
double fillRatio(vector<Point> contour, int object);

Mat templateExtract(Mat imageIn, int color);
Mat templateMatching(Mat img, int color);
Mat blackMask(Mat image);

Mat removeColor(Mat image, int color);


int findHeightRasit(Mat input);


Mat removeColor(Mat image, int color)
{
    //calibrateThreshold(image, 'Y');
    Mat imageColorFilter = thresholdImage(image, color, false);

    //Mat imageWhiteFilter = thresholdImage(image, 'W', false);

    imageColorFilter = getLargestArea(imageColorFilter, 'N');
    
    Mat imageColor, imageOut;
    image.copyTo(imageColor, imageColorFilter);
    
    //dispImage(imageColor, "sari", 0);
    imageOut = image - imageColor;
    
    // Rect boundRect = boundingRect(imageBlackRect);
    //calibrateThreshold('W');

    //Mat imageRect = Mat::zeros(image.size(), CV_8UC1);
    //rectangle(imageRect, boundRect.tl() + Point(10, 0), boundRect.br() + Point(-8, -8), Scalar(255, 0, 0), -1, 1, 0);
    //Mat imageMask = getLargestArea(imageRect, 'N');

    dispImage(imageOut, "removed", 2);
    //calibrateThreshold(imageOut, 'W');

    return imageOut;
}

Mat templateMatching(Mat img, int color) //not working
{
    Mat result;
    Mat templ;
    Mat rescale_result;
    if (color == 'R')
        templ = imread("red.png", CV_LOAD_IMAGE_COLOR); // Read the file
    else if (color == 'B')
        templ = imread("blue.png", CV_LOAD_IMAGE_COLOR);
    /// Create the result matrix
    int result_cols = img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;
    //result.create( result_rows, result_cols, CV_32FC1 );
    /// Do the Matching and Normalize
    matchTemplate(img, templ, result, CV_TM_CCORR_NORMED);
    //dispImage(result, "match", 4);

    result = result * 500;

    //normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
    if(color == 'R'){
    threshold(result, result, 380, 500, THRESH_BINARY);
    }
    else if (color == 'B'){
        threshold(result, result, 340, 500, THRESH_BINARY);
    }

    resize(result, rescale_result, Size(img.cols,img.rows),0,0,INTER_LINEAR );

    dispImage(rescale_result, "matching", 4);
    //waitKey(0);
    //resize(result, result, Size(round(img.cols), round(img.rows)), 0, 0, INTER_LINEAR);
    return rescale_result;
}

int getFPS()
{
    time(&end_t);
    double fps = FPSCounter / (end_t - start);
    FPSCounter++;
    return fps;
}

double fillRatio(vector<Point> contour, int object)
{
    Point_<float> circleCenter;
    RotatedRect boundRect;
    double areaCont = contourArea(contour, false);
    double fillRatio = 0;
    //area of rectange : r.width * r.height

    if (object == 'R')
    {
        boundRect = minAreaRect(contour);
        double rectEnvelopeArea = boundRect.size.width * boundRect.size.height;
        fillRatio = abs(boundRect.angle);
    }
    else if (object == 'C')
    {
        minEnclosingCircle(contour, circleCenter, circleRadius);
        double circleEnvelopeArea = PI * circleRadius * circleRadius;
        fillRatio = areaCont / (0.9 * circleEnvelopeArea);
        //angle = acos(fillRatio) * 180.0 / 3.1412;
    }

    return fillRatio;
}

Point2f drawCenterLine(Mat imageIn, int colorFront)
{
    // draws a line through the object and returns angle of the line
    Mat image1, image2;
    Mat image3, image4;

    image1 = getObjectOfColor(imageIn, 'Y');
    Point2f point_mid = findCenter(image1);

    //putText(imageIn, format("Angle : %d",angle), point_mid, FONT_HERSHEY_COMPLEX_SMALL, 0.5, Scalar(0, 0, 0));

    image2 = getObjectOfColor(imageIn, colorFront);
    Point2f point_front = findCenter(image2);
    // dispImage(image2, "purp", 0);

    image4 = Mat::zeros(imageIn.size(), CV_8UC3);
    drawStraightLine(&image4, point_mid, point_front);

    imageIn = imageIn + image4;

    return point_front;
}

Mat getObjectOfColor(Mat image, int colorFront)
{
    Mat imageContours = thresholdImage(image, colorFront, false);
    Mat imageSelected = getLargestArea(imageContours, colorFront);
    return imageSelected;
}

Mat getLargestArea(Mat image, int colorFront)
{
    Mat imageSelected = Mat::zeros(image.size(), CV_8UC1);
    vector<vector<Point>> contours; // Vector for storing contour
    vector<Vec4i> hierarchy;
    Point_<float> circleCenter; // bounding circle params
    //Rect boundRect;
    RotatedRect boundRect;
    int largest_area = 0;
    int largest_contour_index = 0;
    int largest_fillRatio = 0;
    int area_threshold = 0;

    if (colorFront == 'R' || colorFront == 'B')
    {

        area_threshold = 0;
    }
    if (colorFront == 'P')
    {

        area_threshold = 50;
        cout<<endl<< "area thresh : "<<area_threshold<<endl;
    }

    if (colorFront == 'G')
    {

        area_threshold = 50;
        cout<<endl<< "area thresh : "<<area_threshold<<endl;
    }

    findContours(image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    double a;
    for (int i = 0; i < contours.size(); i++) // iterate through each contour.
    {
        double a = contourArea(contours[i], false);
        //if (contourArea(contours[i], false)>2000){
        if (a > largest_area && a > area_threshold)
        {

            largest_area = a;
            largest_contour_index = i; // Store the index of largest contour
            minEnclosingCircle(contours[i], circleCenter, circleRadius);
            boundRect = minAreaRect(contours[i]);

        }
    }

    cout<<endl<< "area: "<<largest_area<<endl;

    drawContours(imageSelected, contours, largest_contour_index, Scalar(255, 0, 0), CV_FILLED, 8, hierarchy);

    return imageSelected;
}

void getPanaroma()
{
    startCamera(0);

    Mat pano;
    Stitcher stitcher = Stitcher::createDefault(true);
    int b = 0;

    while (true)
    {
        Mat pano;
        vector<Mat> imgs;
        int k = 0;
        while (b != 'd')
        {
           // b = waitKey(100);
            camera.read(newFrame);
            resize(newFrame, newFrame, Size(), 0.6, 0.6, INTER_LINEAR);

            if (b == 'r')
            {
                b = 0;
                cout << "save" << endl;
                imgs.push_back(newFrame);

                Stitcher::Status status = stitcher.stitch(imgs, pano);

                if (status == Stitcher::OK)
                {
                    k++;
                    dispImage(pano, "pano", 0);
                    if (k == 5)
                    {
                        break;
                    }
                }
            }
            dispImage(newFrame, "frame", 5);
        }
        b = 0;
        cout << "pano" << endl;
    }
}

Mat detectTriangles(Mat image)
{
    Mat ROI = blackMask(image);
    //dispImage(ROI, "ROI", 4);
    //calibrateThreshold( ROI, 'W');

    Mat imageOut = thresholdImage(ROI, 'W', false);

    //dispImage(imageOut, "thresh", 2);

    //while(waitKey(10)!='q')
    //dispImage(imageOut, "trig", 2);
    return imageOut;
}

Mat blackMask(Mat image)
{
    Mat imageBlackFilter = thresholdImage(image, 'S', false);

    //Mat imageWhiteFilter = thresholdImage(image, 'W', false);

    Mat imageBlackRect = getLargestArea(imageBlackFilter, 'N');
    Rect boundRect = boundingRect(imageBlackRect);
    //calibrateThreshold('W');

    Mat imageRect = Mat::zeros(image.size(), CV_8UC1);
    rectangle(imageRect, boundRect.tl() + Point(0, 75), boundRect.br() + Point(0, -3), Scalar(255, 0, 0), -1, 1, 0);
    Mat imageMask = getLargestArea(imageRect, 'N');
	//dispImage(imageMask, "S", 4);
    Mat imageOut;
    image.copyTo(imageOut, imageMask);
    //dispImage(imageOut, "W", 2);
    
    //calibrateThreshold(imageOut, 'W');

    return imageOut;
}

Point2f getBoardSlot(Mat img, int trig_index_1, int trig_intex_2, Mat& binary_img, bool inverse)
{
    
    Mat imageContours = detectTriangles(img);
    
    
    Point2f point;
    if (inverse)
    {
        flip(imageContours, imageContours, 0);
        transpose(imageContours, imageContours);
        imageContours = getTriangleContours(imageContours, trig_index_1, trig_intex_2);
 
        flip(imageContours, imageContours, 1);
        transpose(imageContours, imageContours);

        point = findCenter(imageContours);
    }
    else
    {
        flip(imageContours, imageContours, 1);
        transpose(imageContours, imageContours);
        imageContours = getTriangleContours(imageContours, trig_index_1, trig_intex_2);
        flip(imageContours, imageContours, 0);
        transpose(imageContours, imageContours);
        point = findCenter(imageContours);
    }

    binary_img = imageContours;
    
    //dispImage(imageContours, "trig", 0);
    //dispImage(imageContours, "3", 4);
    return point;
}

int findHeightRasit(Mat input){
	int length = input.checkVector(2);

  	int xmin = 0, xmax = 0, ymin = 0, ymax=0;
  	//Point ptxmin, ptxmax, ptymin, ptymax;

	const Point* pts = input.ptr<Point>();
  	Point pt = pts[0];

  	//ptxmin  = ptxmax = ptymin = pt;
    //xmin = xmax = pt.x;
    ymin = ymax = pt.y;

    for( int i = 1; i < length; i++ )
    {
        pt = pts[i];
        /*
        if( xmin > pt.x )
        {
            xmin = pt.x;
            ptxmin = pt;
        }


        if( xmax < pt.x )
        {
            xmax = pt.x;
            ptxmax = pt;
        }
		*/

        if( ymin > pt.y )
        {
            ymin = pt.y;
            //ptymin = pt;
        }

        if( ymax < pt.y )
        {
            ymax = pt.y;
            //ptymax = pt;
        }
    }
    
    
    return (ymax - ymin);
}


Mat mask;

Point2f boardSlotRasit(Mat img, int trig_index_1, int trig_intex_2, bool inverse, bool first_call){

	
    Point2f point;
    Mat imageOut = Mat::zeros(img.size(), CV_8UC1);
    Mat imageMasked;
	//Mat forMeasurement = Mat::zeros(img.size(), CV_8UC1);

    if (first_call){
        point = getBoardSlot(img, trig_index_1, trig_intex_2, imageOut, inverse);    
        mask = MaskForTriangle(imageOut, mask, 1); // 1 is used since number of contours is not 0. We don't want it to be 0. It could be anything but 0.
        initTriangles(imageOut,img);
        
    }

    else{
        Mat imageContours = detectTriangles(img);

        // Find contours and preserve matching ones.
        vector<vector<Point>> contours; // Vector for storing contour
    	vector<Vec4i> hierarchy;
    	findContours(imageContours, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    	int averageHeight = 0;

    	cout<<"Number of contours"<<contours.size()<<endl;
    	
    	//// ADDITION START ////
			// Instead of masking, pair mass centers from two back to back frames.

			/// Get the moments
  			vector<Moments> mu(contours.size() );	
  			for( int i = 0; i < contours.size(); i++ )
     		{ mu[i] = moments( contours[i], false ); }

			///  Get the mass centers:
			vector<Point2f> mc( contours.size() );
			for( int i = 0; i < contours.size(); i++ )
			{ mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }

		
		//// ADDITION START ////

		//Clear trianglesDetected
    	

    	int numberOfSelectedTriangles = 0;
    	int averageTriangleArea = 0;
    	for (int i=0;i<contours.size();i++){
			Mat test = Mat::zeros(img.size(), CV_8UC1);
    		Mat imageEmptyTest = Mat::zeros(img.size(), CV_8UC1);
    		if (contourArea(contours[i])<5){continue;}
    		if (hierarchy[i][3] != -1){continue;}
			drawContours( test, contours, i, Scalar(255,255,255), -1, 8, hierarchy, 0, Point() );
			// Save all triangles.
			



			test.copyTo(imageEmptyTest, mask);
			
			if (sum(imageEmptyTest)[0]>50){
				drawContours( imageOut, contours, i, Scalar(255,255,255), -1, 8, hierarchy, 0, Point() );
				cout<<"Selected contour areas: "<<contourArea(contours[i])<<" "<<endl;
				if (max_contour_area<contourArea(contours[i]) ) {max_contour_area=contourArea(contours[i]);}
				//cout<<"Triangle Height: "<<findHeightRasit(Mat(contours[i]))<<endl;
				averageHeight=averageHeight+findHeightRasit(Mat(contours[i]));
				averageTriangleArea=averageTriangleArea+contourArea(contours[i]);
				numberOfSelectedTriangles++;
				/*
				imshow("imageOut",imageOut);
    			cout<<"imageOut"<<endl;
				waitKey(0);
				*/
			}

    	}
    	if (numberOfSelectedTriangles!=0){
    	averageHeight = averageHeight/numberOfSelectedTriangles;
    	cout<<"averageHeight: "<<averageHeight<<endl;
    	averageTriangleArea = averageTriangleArea/numberOfSelectedTriangles;
    	cout<<"averageTriangleArea: "<<averageTriangleArea<<endl;
    	if (max_average_height<averageHeight) {max_average_height=averageHeight;cout<<"max_average_height:"<<max_average_height<<endl;}

    	}
    	else {cout<<"ERROR\nERROR\nERROR\nERROR: No triangle is detected."<<endl;}

    	/*
    	imshow("imageOut",imageOut);
    	cout<<"imageOut"<<endl;
		waitKey(0);
		*/

    	/*
    	imshow("slots",imageOut);
    	waitKey(0);
        */
        //point = getBoardSlot(imageOut, 0, 1,imageMasked, false);
        mask = MaskForTriangle(imageOut, mask, contours.size());
        //pairTriangles(imageContours);
        point = findCenter(imageOut);


        Mat forDebugMat = imageOut.clone();
        circle( forDebugMat,
         point,
         point.x/32.0,
         Scalar( 255, 255, 255 ),
         -1,
         8 ); //lineType



        dispImage(forDebugMat, "forDebugMat", 0 );
        //dispImage(imageOut, "imageOut", 0 );
    }

	//dispImage(imageOut, "imageOut", 0 );
    return point;
}

Point2f boardForAllignSlot(Mat img, int trig_index_1, int trig_intex_2, bool inverse, bool first_call){

    
    Point2f point;
    Mat imageOut = Mat::zeros(img.size(), CV_8UC1);
    //Mat imageMasked;
    //Mat forMeasurement = Mat::zeros(img.size(), CV_8UC1);

    if (first_call){
        point = getBoardSlot(img, trig_index_1, trig_intex_2, imageOut, inverse);    
        initTriangles(imageOut,img);
        // This part initializes position of selected triangles. 
        
    }

    else{
        Mat imageContours = detectTriangles(img);

        
        pairTriangles(imageContours);

        if (trianglesSelected.size()==1){point = Point2f(trianglesSelected[0],img.rows/2);} // y axis is not important.
        else if (trianglesSelected.size()==2){point = Point2f((trianglesSelected[0]+trianglesSelected[1])/2,img.rows/2);} // y axis is not important.
        else {cout<<"More than one trianles in trianglesSelected. Need to dump some of them."<<endl;}
        

        Mat forDebugMat = img.clone();
        circle( forDebugMat,
         point,
         point.x/32.0,
         Scalar( 255, 255, 255 ),
         -1,
         8 ); //lineType



        dispImage(forDebugMat, "forDebugMat", 0 );
        //dispImage(imageOut, "imageOut", 0 );
    }

    //dispImage(imageOut, "imageOut", 0 );
    return point;
}


Mat getTriangleContours(Mat image, int trig_index_1, int trig_index_2)
{
    Mat imageSelected = Mat::zeros(image.size(), CV_8UC1);
    vector<vector<Point>> contours; // Vector for storing contour
    vector<Vec4i> hierarchy;

    findContours(image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    if (contours.size() == 0)
    {
        return imageSelected;
    }

    if (contours.size() == 1)
    {
        drawContours(imageSelected, contours, trig_index_1, Scalar(255, 0, 0), CV_FILLED, 8, hierarchy);
    }
    else if (contours.size() == trig_index_2)
    {
        trig_index_1 = trig_index_1 - 1;
        trig_index_2 = trig_index_2 - 1;
    }

    else if (contours.size() == trig_index_1)
    {
        trig_index_1 = trig_index_1 - 2;
        trig_index_2 = trig_index_2 - 2;
    }

    else
    {

        for (int i = trig_index_1; i <= trig_index_2; i++) // iterate through each contour.
        {
            drawContours(imageSelected, contours, i, Scalar(255, 0, 0), CV_FILLED, 8, hierarchy);
        }
        return imageSelected;
    }
}
