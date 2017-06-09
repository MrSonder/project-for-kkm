// Game Algorithm
// Use: Point a = GiveNextMove(blocks);
// a.x and a.y will be used to extract axes.

Point GiveNextMove(vector<vector<int>> blocks);
Point RobotMove(int board[6][6]);
int GetScore(int board[6][6], int player);
int win(int board[6][6]);
Mat MaskForTriangle(Mat input, Mat mask, int no_of_contours);
void pairTriangles(Mat input);
void initTriangles(Mat input, Mat img);

int board[6][6] = {{0,0,0,0,0,0},{2,0,0,0,0,0},{2,2,0,0,0,0},{2,2,2,0,0,0},{2,2,2,2,0,0},{2,2,2,2,2,0}} ;
int no_of_branches = 0;

Point GiveNextMove(vector<vector<int>> blocks){


	// Inialize board array.
    for (int i=0; i<6;i++){
        for (int j=0; j<6;j++){
            if (i <= j){
                board[i][j] = 0;
            }
            else board[i][j] = 2;
        }
    }
	for (int i=0; i<blocks.size();i++){
		int pos_x = blocks[i][0];
		int pos_y = 5 - blocks[i][1];
		int color = blocks[i][2];
		board[pos_x][pos_y] = color;
		}
	return RobotMove(board);
}


Point RobotMove(int board[6][6]){
	//Special cases:
	int count_moves = 0;
	for (int i=0; i<6; i++){
		if (board[i][5] != 0) count_moves++;
	}
	if (count_moves == 0 or count_moves == 1){
		if (board[2][5] == 0) return Point(2,0); // it was Point(2,5)
		else return Point(3,0); // it was Point(3,5)
	}
	int player = 1;
	int move_i = -1;
	int move_j = -1;
	double score = 0;
	no_of_branches = 0;
	for (int i=0; i<6;i++){
		for (int j=0; j<6;j++){
			if (board[i][j] == 0 and (j==5 or ((board[i][j+1]!=0) and (board[i+1][j+1]!=0) ))) {
				board[i][j] = player;
				double thisScore = GetScore(board, player*-1);
				thisScore /= no_of_branches;
				no_of_branches = 0;
				//cout<<thisScore<<endl;
				if (thisScore > score){
					score = thisScore;
					move_i = i;
					move_j = j;
				}

				board[i][j] = 0;
			}

		}
	}
	//board[move_i][move_j] = player;
	return Point(move_i, 5-move_j); //It was Point(move_i, move_j)

}

int GetScore(int board[6][6], int player){

	int score = 0;
	int counter = 0;
	if (win(board)==1) { no_of_branches++;return 1;}
	else if (win(board)==-1) { no_of_branches++;return 0;}
	else
		for (int i=0; i<6 ;i++){
			for (int j=0; j<6;j++){
				if (board[i][j] == 0 and (j==5 or ((board[i][j+1]!=0) and (board[i+1][j+1]!=0) ))) {
					board[i][j] = player;
					counter++;
					score = score + GetScore(board, player*-1);
					board[i][j] = 0;
				}

			}
		}
	if (counter == 0) no_of_branches++;
	return score;

}

int win(int board[6][6]){
	int win_conditions[30][3][2] ={{{0,0},{0,1},{0,2}},
								  {{0,1},{0,2},{0,3}},
								  {{0,2},{0,3},{0,4}},
								  {{0,3},{0,4},{0,5}},
								  {{1,1},{1,2},{1,3}},
								  {{1,2},{1,3},{1,4}},
								  {{1,3},{1,4},{1,5}},
								  {{2,2},{2,3},{2,4}},
								  {{2,3},{2,4},{2,5}},
								  {{3,3},{3,4},{3,5}},
								  {{0,0},{1,1},{2,2}},
								  {{1,1},{2,2},{3,3}},
								  {{2,2},{3,3},{4,4}},
								  {{3,3},{4,4},{5,5}},
								  {{0,1},{1,2},{2,3}},
								  {{1,2},{2,3},{3,4}},
								  {{2,3},{3,4},{4,5}},
								  {{0,2},{1,3},{2,4}},
								  {{1,3},{2,4},{3,5}},
								  {{0,3},{1,4},{2,5}},
								  {{0,2},{1,2},{2,2}},
								  {{0,3},{1,3},{2,3}},
								  {{1,3},{2,3},{3,3}},
								  {{0,4},{1,4},{2,4}},
								  {{1,4},{2,4},{3,4}},
								  {{2,4},{3,4},{4,4}},
								  {{0,5},{1,5},{2,5}},
								  {{1,5},{2,5},{3,5}},
								  {{2,5},{3,5},{4,5}},
								  {{3,5},{4,5},{5,5}},
								};
	for (int x=0;x<30;x++)
	{
		if (board[win_conditions[x][0][0]][win_conditions[x][0][1]] != 0 and
			board[win_conditions[x][0][0]][win_conditions[x][0][1]] == board[win_conditions[x][1][0]][win_conditions[x][1][1]] and
			board[win_conditions[x][0][0]][win_conditions[x][0][1]] == board[win_conditions[x][2][0]][win_conditions[x][2][1]]
			) {return board[win_conditions[x][0][0]][win_conditions[x][0][1]];}
	}								
	return 0;
}



// This is for mask. 
// Input should be 1 or 2 contours only (black & white as well).
// Use: Mat mask = MaskForTriangle(input);

Mat MaskForTriangle(Mat input, Mat mask, int no_of_contours){
	// Here if input is all black mask is all screen.

	
	Mat nonZeroCoordinates;
	Mat rectangleShown = Mat::zeros(input.size(),CV_8UC1);
	Mat imageUsed = Mat::zeros(input.size(),CV_8UC1);
	double multiple_scale = 1.03;
	//double multiple_scale = 1;
	double divide_scale = 0.97;
	//double divide_scale = 1;

	if (no_of_contours==0){
		cout<<"Hiç contour göremedim."<<endl;
		imageUsed = mask.clone();
		imshow("mask",mask);
		return imageUsed;
	}
	else {imageUsed = input.clone();}


	findNonZero(imageUsed, nonZeroCoordinates);
	vector<int> points {640,0,480,0}; 
	points[0]=640; //xmin
	points[1]=0;   //xmax
	points[2]=480; //ymin
	points[3]=0;   //ymax
    for (int i = 0; i < nonZeroCoordinates.total(); i++ ) {
        if (nonZeroCoordinates.at<Point>(i).x < points[0]) points[0]=nonZeroCoordinates.at<Point>(i).x;
        if (nonZeroCoordinates.at<Point>(i).x > points[1]) points[1]=nonZeroCoordinates.at<Point>(i).x;
        if (nonZeroCoordinates.at<Point>(i).y < points[2]) points[2]=nonZeroCoordinates.at<Point>(i).y;
        if (nonZeroCoordinates.at<Point>(i).y > points[3]) points[3]=nonZeroCoordinates.at<Point>(i).y;        
    }

    vector<Point> my_points {Point(points[0]*divide_scale, points[2]*1.0001),Point( (points[1]*multiple_scale>input.cols)?input.cols:points[1]*multiple_scale, (points[1]*multiple_scale>input.rows)?input.rows:points[3]*multiple_scale )};
    Mat result = Mat::zeros(input.size(),CV_8UC1);
	Rect my_rect = boundingRect(my_points);
	rectangle( result, my_rect.tl(), my_rect.br(), Scalar(255,255,255), -1, 8, 0 );
	//rectangle( rectangleShown, my_rect.tl(), my_rect.br(), Scalar(255,255,255), 1, 8, 0 );
	//imshow("rectangleShown", rectangleShown);
    return result;
}


void pairTriangles(Mat input){
		// Input is black & white image of the triangles.
		vector<vector<Point>> contours; // Vector for storing contour
    	vector<Vec4i> hierarchy;
    	findContours(input, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    	trianglesDetected.clear();
		// Instead of masking, pair mass centers from two back to back frames.

			/// Get the moments
  			vector<Moments> mu(contours.size() );	
  			for( int i = 0; i < contours.size(); i++ )
     		{ mu[i] = moments( contours[i], false ); }

			///  Get the mass centers:
			vector<Point2f> mc( contours.size() );
			for( int i = 0; i < contours.size(); i++ )
			{ mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
			  trianglesDetected.push_back(   int(mc[i].x) );}
			///    	
			
    		sort(trianglesDetected.begin(), trianglesDetected.end());
    		cout<<"size of vector: "<<trianglesDetected.size()<<endl;
    		cout<<"Detected triangles are at ";

			for (int i = 0; i<trianglesDetected.size();i++){
				
				cout<<trianglesDetected[i]<<" ";
			}
			cout<<endl;

		// Now, pair them to ours.
		cout<<"order_from_left: "<<order_from_left.size()<<" order_from_right: "<<order_from_right.size()<<endl;
		if (contours.size()==number_of_last_seen_triangles){
			// All of them are still seen. Then select according to order from left.
			int number_of_triangles_selected = trianglesSelected.size();
			trianglesSelected.clear();
			for (int i=0; i<order_from_left.size();i++){
				trianglesSelected.push_back(trianglesDetected[order_from_left[i]]);
			}
		}	

		else { // Some of the triangles are lost.
			
			int calculated_distance_between_triangles=0;
			for (int i=0;i<(trianglesDetected.size()-1);i++){
				calculated_distance_between_triangles = calculated_distance_between_triangles + (trianglesDetected[i+1]-trianglesDetected[i]);
			}
			calculated_distance_between_triangles /= (trianglesDetected.size()-1);
			cout<<"calculated_distance_between_triangles: "<<calculated_distance_between_triangles<<endl;

			// Now find from which side triangles disappeared.

			if ( (trianglesDetected[trianglesDetected.size()-1]+calculated_distance_between_triangles)> input.cols ){
				// If this happened left side has lost some of the triangles.
				trianglesSelected.clear();
				reverse(trianglesDetected.begin(),trianglesDetected.end());
				cout<<"ters çevirilmiş: "<<endl;

				for (int i = 0; i<trianglesDetected.size();i++){
				
					cout<<trianglesDetected[i]<<" ";
				}
				cout<<endl;

				for (int i=0; i<order_from_left.size();i++){
				trianglesSelected.push_back(trianglesDetected[order_from_left[i]]);
				cout<<"Added "<<trianglesDetected[order_from_left[i]]<<endl;
				}
				reverse(trianglesDetected.begin(),trianglesDetected.end());
			}
			else if ( (trianglesDetected[0]-calculated_distance_between_triangles)< 5 ){
				// If this happened right side has lost some of the triangles.
				trianglesSelected.clear();
				reverse(trianglesDetected.begin(),trianglesDetected.end());

				
				for (int i=0; i<order_from_right.size();i++){
				trianglesSelected.push_back(trianglesDetected[order_from_right[i]]);
				}
				cout<<"trianglesSelected size: "<<trianglesSelected.size()<<endl;
				cout<<"Right loses, selected triangles are at ";

				for (int i = 0; i<trianglesSelected.size();i++){
					
					cout<<trianglesSelected[i]<<" ";
				}
				cout<<endl;
				reverse(trianglesDetected.begin(),trianglesDetected.end());
			}

			else {//No reason to debug this now.
				cout<<"Something went wrong!"<<endl;
				cout<<"Either at least one triangle from both sides departed\n or more than 7 contours are seen."<<endl;
				}

		}

		sort(trianglesSelected.begin(), trianglesSelected.end());

		order_from_right.clear();
		order_from_left.clear();
		for (int i=0; i<trianglesSelected.size();i++){
			for (int j=0; j<trianglesDetected.size();j++){
				if (abs( (trianglesSelected[i]-trianglesDetected[j])<10 )) {
					order_from_left.push_back(j);
					order_from_right.push_back(trianglesDetected.size()-1-j);
					break;
				}
			}
		}

		cout<<"After pairing, selected triangles are at ";

			for (int i = 0; i<trianglesSelected.size();i++){
				
				cout<<trianglesSelected[i]<<" ";
			}
			cout<<endl;
		
	
}


void initTriangles(Mat input, Mat img){
		// Input is black & white image of the triangles.
		vector<vector<Point>> contours; // Vector for storing contour
    	vector<Vec4i> hierarchy;
    	findContours(input, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    	trianglesSelected.clear();
		// Instead of masking, pair mass centers from two back to back frames.

		/// Get the moments
  		vector<Moments> mu(contours.size() );	
  		for( int i = 0; i < contours.size(); i++ )
 		{ mu[i] = moments( contours[i], false ); }

		///  Get the mass centers:
		vector<Point2f> mc( contours.size() );
		for( int i = 0; i < contours.size(); i++ )
		{ mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
		  trianglesSelected.push_back(   int(mc[i].x) );}
		///    	
		
		sort(trianglesSelected.begin(), trianglesSelected.end());
		//cout<<"size of vector: "<<trianglesSelected.size()<<endl;
		


		// Now see all of the triangles.
		while(true){
		
		Mat imageContours = detectTriangles(img);
		vector<int> allTriangles;

        vector<vector<Point>> contours; // Vector for storing contour
    	vector<Vec4i> hierarchy;
    	findContours(imageContours, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    	/// Get the moments
  		vector<Moments> mu(contours.size() );	
  		for( int i = 0; i < contours.size(); i++ )
 		{ mu[i] = moments( contours[i], false ); }

		///  Get the mass centers:
		vector<Point2f> mc( contours.size() );
		for( int i = 0; i < contours.size(); i++ )
		{ mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
		  allTriangles.push_back(   int(mc[i].x) );}
		///    	
		
		sort(allTriangles.begin(), allTriangles.end());
		number_of_last_seen_triangles = allTriangles.size();
		if (number_of_last_seen_triangles!=7) {cout<<"I didn't see 7 triangles."<<endl;}
		for (int i=0; i<trianglesSelected.size();i++){
			for (int j=0; j<allTriangles.size();j++){
				if (abs( (trianglesSelected[i]-allTriangles[j])<10 )) {
					order_from_left.push_back(j);
					order_from_right.push_back(allTriangles.size()-1-j);
					break;
				}
			}
		}
		cout<<"All triangles are at: ";
		for (int i=0; i<allTriangles.size();i++){
			cout<<allTriangles[i]<<" ";
		}
		cout<<"\nOur triangles are at: ";
		for (int i=0; i<trianglesSelected.size();i++){
			cout<<trianglesSelected[i]<<" ";
		}
		cout<<"\nOrder from left: ";
		for (int i=0; i<order_from_left.size();i++){
			cout<<order_from_left[i]<<" ";
		}
		cout<<"\nOrder from right: ";
		for (int i=0; i<order_from_right.size();i++){
			cout<<order_from_right[i]<<" ";
		}
		cout<<endl;


		break;
    	}
	
}
