void getBoardInfo();
vector<vector<int>> setObjectLocations(Mat img);
vector<vector<int>> gameBoardBlocks(Mat img);
vector<vector<int>> getObjectPoints(Mat img, int color);
vector<int> set_Y_Threshold(vector<int> input_vector, int comparator);
vector<int> getTrianglePoints(Mat img, int axis);
int setX(int x, int y, vector<vector<int>> input_vector);
vector<vector<int>> set_X_Threshold(Mat img);

void getBoardInfo(Mat img)
{
    auto start = std::chrono::steady_clock::now();

    setObjectLocations(img);

    auto finish = std::chrono::steady_clock::now();
    double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
    cout << elapsed_seconds;
}

vector<vector<int>> gameBoardBlocks(Mat img)
{
   
    vector<vector<int>> red_vector = getObjectPoints(img, 'R');
    vector<vector<int>> blue_vector = getObjectPoints(img, 'B');
    red_vector.insert(red_vector.end(), blue_vector.begin(), blue_vector.end());
    cout<<red_vector.size()<<endl;
    return red_vector;
}

vector<vector<int>> getObjectPoints(Mat img, int color)
{
    Mat image_blue, image_red;
    Point2f point;
    vector<vector<int>> output_vector;
    vector<int> temp, clear;
    
    Mat img_temp =templateExtract(img, color);

    Mat image = templateMatching(img_temp, color);
    

    Mat imageSelected = Mat::zeros(image.size(), CV_8UC1);
    vector<vector<Point>> contours; // Vector for storing contour
    vector<Vec4i> hierarchy;
    image.convertTo(image, CV_8UC1);

    findContours(image, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    cout<<contours.size()<<endl;
    
    
    for (int i = 0; i < contours.size(); i++) // iterate through each contour.
    {
        imageSelected = Mat::zeros(image.size(), CV_8UC1);
        drawContours(imageSelected, contours, i, Scalar(255, 0, 0), CV_FILLED, 8, hierarchy);
        point = findCenter(imageSelected);

        temp.push_back(point.x);
        temp.push_back(point.y);
        if (color == 'R')
        {
            temp.push_back(-1);
        }
        else if (color == 'B')
        {
            temp.push_back(1);
        }

        output_vector.push_back(temp);
        temp = clear;
    }

    return output_vector;
}

vector<int> getTrianglePoints(Mat img, int axis)
{
    vector<int> output_vector;
    Point2f point;
    Mat dummy;
    for (int i = 0; i <= 6; i++)
    {
        point = getBoardSlot(img, i, i, dummy );
        if (axis == 'x')
        {
            output_vector.push_back(point.x);
        }
        else if (axis == 'y')
        {
            output_vector.push_back(point.y);
        }
        //cout << output_vector[i] << endl;
    }

    sort(output_vector.begin(), output_vector.end());
    return output_vector;
}

vector<int> set_Y_Threshold(vector<int> input_vector, int comparator)
{

    sort(input_vector.begin(), input_vector.end());

    vector<int> threshold_vector;

    threshold_vector.push_back(input_vector[0] + comparator);
    for (int i = 1; i < input_vector.size(); i++)
    {
        bool threshold_test = input_vector[i] > (threshold_vector.back() + comparator);
        if (threshold_test)
        {
            threshold_vector.push_back(input_vector[i] + comparator);
        }
    }

    return threshold_vector;
}

vector<vector<int>> setObjectLocations(Mat img)
{
    int comparator = 10;
    vector<vector<int>> blocks = gameBoardBlocks(img);

    if(blocks.size()==0){
        return blocks;
    }

    cout<<blocks.size()<<endl;


    vector<int> input_vector;
    vector<vector<int>> threshold_vector_x = set_X_Threshold(img);

    for (int i = 0; i < blocks.size(); i++)
    {
        input_vector.push_back(blocks[i][1]);
    }

    vector<int> threshold_vector = set_Y_Threshold(input_vector, comparator);

    vector<int> output_vector;
    for (int i = 0; i < input_vector.size(); i++)
    {
        for (int k = 0; k < (threshold_vector.size()); k++)
        {
            bool location_test = (threshold_vector[k] < blocks[i][1]) && (blocks[i][1] < threshold_vector[k + 1]);
            if (location_test)
            {
                //output_vector.push_back(k);
                blocks[i][1] = k;
                blocks[i][0] = setX(blocks[i][0], k, threshold_vector_x)  ;
                break;
            }
            if (k == threshold_vector.size() - 1)
            {
                //output_vector.push_back(k);
                blocks[i][1] = k;
                blocks[i][0] = setX(blocks[i][0], k, threshold_vector_x) ;
            }
        }
    }

    //cout<<blocks.size()<<endl;

    for (int i = 0; i < blocks.size(); i++)
    {
        cout << blocks[i][0] << " " << blocks[i][1] << " " << blocks[i][2] << endl;
    }


    return blocks;
}

int setX(int x, int y, vector<vector<int>> input_vector)
{
    //threshold_vector.at(6 - y);
    vector<int> threshold_vector = input_vector.at(y);
/*
    cout<<"x:"<<x<<endl;
    for (int k = 0; k < (threshold_vector.size()); k++)
    {
        cout<<threshold_vector[k]<<" ";
    }
    cout<<endl;*/


    for (int k = 0; k < (threshold_vector.size()); k++)
    {


        bool location_test = (threshold_vector[k] < x) && (x < threshold_vector[k + 1]);
        
        if (location_test)
        {
            return k;
        }

        if (k == threshold_vector.size() - 1)
        {
            return k;
        }
    }
}

vector<vector<int>> set_X_Threshold(Mat img)
{
    vector<int> triangle_vector = getTrianglePoints(img, 'x');

    vector<vector<int>> threshold_vector;

    threshold_vector.push_back(triangle_vector);
    // //cout << "avg:" << (triangle_vector[0] + triangle_vector[0 + 1]) / 2 << endl;

    int initial_size = triangle_vector.size() - 2;
    for (int k = 0; k < initial_size; k++)
    {
        vector<int> temp_vector;
        //cout << "level: " << k << endl;
        for (int i = 0; i < (triangle_vector.size() - 1); i++)
        {
            temp_vector.push_back((triangle_vector[i] + triangle_vector[i + 1]) / 2);
        }

        for (int i = 0; i < temp_vector.size(); i++)
        {
            //cout << *(std::begin(temp_vector) + i) << endl;
        }

        threshold_vector.push_back(temp_vector);
        triangle_vector = temp_vector;
    }

    return threshold_vector;
}