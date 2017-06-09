Mat rectangleMask(Mat image, Rect boundRect);
int signOf(int number);
void motorPropController(Point2f center, int base_speed, double turn_rate_divider);
void driveToCenter(int color, int drive_method, int stop_method, int y_threshold, int cam_index);
void goToObject(int color);

Mat rectangleMask(Mat image, Rect boundRect)
{
    Mat imageRect = Mat::zeros(image.size(), CV_8UC1);
    /*
    Point2f rect_points[4];
    
    rectangle.points(rect_points);
    for (int j = 0; j < 4; j++)
        line(imageRect, rect_points[j], rect_points[(j + 1) % 4], Scalar(255, 0, 0));
*/
    //dispImage(image, "imput", 1);

    rectangle( imageRect, boundRect.tl() + Point(0, 200 ), boundRect.br(), Scalar(255, 0, 0) , 2, 8, 0 );
    
    Mat imageMask = getLargestArea(imageRect, 'N');
    dispImage(imageMask, "rectMask", 2);
    Mat imageOut;

    image.copyTo(imageOut, imageMask);
    dispImage(imageOut, "masked" ,4);
    return imageOut;
}

/*
void goToObjectBetweenTwoFlags(int colorFlag, int colorObject)
{
    //find the flag
    int y_threshold_bot_cam = 110;
    y_threshold_bot_cam += 120;
    driveToCenter(colorFlag, 's', 'b', y_threshold_bot_cam, bot_cam);
    //find the object
    driveToCenter(colorObject, 's', 'c', y_threshold_bot_cam, bot_cam);
}
*/

void goToFlag(int color)
{
    int y_threshold_bot_cam = 110;
    y_threshold_bot_cam += 120;
    driveToCenter(color, 'g', 'b', y_threshold_bot_cam, bot_cam);
}

void goToObject(int color)
{
    int y_threshold_top_cam = 105;
    int y_threshold_bot_cam = 75;
    driveToCenter(color, 'g', 'c', y_threshold_top_cam, top_cam);
    driveToCenter(color, 'g', 'c', y_threshold_bot_cam, bot_cam);
}

void driveToCenter(int color, int drive_method, int stop_method, int y_threshold, int cam_index)
{

    startCamera(cam_index);
    bool stop_cond = false;

    while (true)
    {
        getFrameFromCamera();
        Mat image = getObjectOfColor(newFrame, color);
        Rect boundRect = boundingRect(image);
        Point2f center = findCenter(image, true);

        switch (drive_method)
        {
        case 'g': //go
            switch (stop_method)
            {
            case 'c': //center
                stop_cond = center.y > y_threshold;
                break;
            case 'b': //bottom
                stop_cond = boundRect.y + boundRect.height > y_threshold;
                break;
            }
            motorPropController(center, 130, 24);
            break;

        case 's': //search
            switch (stop_method)
            {
            case 'c': //center
                stop_cond = center.x > -100 && center.x < 100 && center.y < 80 && center.y > -10;
                break;
            case 'b': //bottom
                stop_cond = center.x > -100 && center.x < 100 && (boundRect.y + boundRect.height) > 120;
                break;
            }
            txArduino(driveMotor(70, -70));
            break;
        }

        if (stop_cond)
        {
            txArduino(driveMotor(0, 0));
            break;
        }
        //rectangle bottom condition
    }

    camera.release();
}

void motorPropController(Point2f center, int base_speed, double turn_rate_divider)
{
    double speed = (center.x) / turn_rate_divider;
    if (abs(center.x) < 320)
    {
        txArduino(driveMotor(base_speed + speed, base_speed - speed));
    }
    else
    {
        txArduino(driveMotor(0, 0));
    }
}

int signOf(int number)
{
    if (number >= 0)
    {
        return 1;
    }
    else if (number < 0)
    {
        return -1;
    }
}
