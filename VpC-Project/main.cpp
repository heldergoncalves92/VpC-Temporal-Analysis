//
//  main.cpp
//  VC-Projeto
//
//  Created by Hélder Gonçalves on 11/12/14.
//  Copyright (c) 2014 Hélder Gonçalves. All rights reserved.
//

#include "includes.hpp"


int arraySize;
Mat magI, rho;




// rearrange the quadrants of Fourier image  so that the origin is at the image center
void fftShift(Mat image){
    int cy,cx;
    
    cx = image.cols/2;
    cy = image.rows/2;
    
    Mat q0(image, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(image, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(image, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(image, Rect(cx, cy, cx, cy)); // Bottom-Right
    
    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    
    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);
    
    normalize(image, image, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
    // viewable image form (float between values 0 and 1).
    
}


void findRho_impf(int *num, float *f){
    int i,j, r;

    for (i=0; i<rho.cols; i++) {
        for (j=0; j<rho.rows; j++) {
            r=(int)rho.at<float>(j, i);
            
            if(r>0 && r<arraySize){
				f[r] += powf(magI.at<float>(j, i), 2);
				num[r]++;
            }
        }
    }
    for (r = 1; r<arraySize; r++)
        if(num[r]!=0)
            f[r] /= num[r];
}


int main(){
    
    int m, i, j, mX, mY ;
    Mat frame, padded;
    Mat planes[2], complexI;
    Mat t;

	float alpha;
    Vec4f line;
    vector<Point2f> points;
    
    time_t timeI, timeF;
    float seconds;
    
    //VideoCapture cap(0); // open the default camera
    //VideoCapture cap("videos/approaching_lv_40ms_translate_approach.avi");
    //VideoCapture cap("videos/car2.avi");
    
    //VideoCapture cap("videos/approaching_lv_40ms_with_scatered_backgroung.avi");
    //VideoCapture cap("videos/2_bolas.avi");
    VideoCapture cap("videos/1_bola.avi");

    if(!cap.isOpened())  // check if we succeeded
        return -1;
    
    Mat edges;

	cap >> frame; // get a new frame from camera
    m = frame.rows > frame.cols ? frame.cols : frame.rows;

    int mdft = getOptimalDFTSize( m );
    setDim(mdft);
    arraySize = mdft/2+1;
	
    

    //Init arrays
    int *num = (int*)calloc(sizeof(int),arraySize);
    float *f=(float*)calloc(sizeof(float),arraySize);
    
    
	/************* Get coordinates of the power spectrum image ***********/
	Mat xx = Mat::zeros(Size(mdft, mdft), CV_32F);
	Mat yy = Mat::zeros(Size(mdft, mdft), CV_32F);
	Mat theta;
    
	mY = -mdft / 2;
	for (i = 0; i<mdft; i++) {
		mX = -mdft / 2;
		for (j = 0; j<mdft; j++) {
			xx.at<float>(i, j) = (float)mX++;
			yy.at<float>(i, j) = (float)mY;
		}
		mY++;
	}
	cartToPolar(xx, yy, rho, theta);


	//Round RHO (Checked)
    for (i = 0; i<mdft; i++)
        for (j = 0; j<mdft; j++)
			rho.at<float>(j, i) = cvRound(rho.at<float>(j, i));

    int framess=0;
    
    time(&timeI);
    /************** START CYCLE ******************/
	while (cap.isOpened()) {

        cap >> frame; // get a new frame from camera
        if(!frame.data) break;
        cvtColor(frame, edges, CV_BGR2GRAY);
        
        
        if(mdft > m){
            padded = edges(Rect((edges.cols - m) / 2, (edges.rows - m) / 2, m, m));
            resize(padded, padded, Size(mdft,mdft));
        }
        else
            padded = edges(Rect((edges.cols - mdft) / 2, (edges.rows - mdft) / 2, mdft, mdft));
        
        
        GaussianBlur(padded, padded, Size(11,11), 1);
        
        
        // Add to the expanded another plane with zeros
        planes[0] = Mat_<float>(padded);
        planes[1] = Mat::zeros(padded.size(), CV_32F);
        merge(planes, 2, complexI);
        
        dft(complexI, complexI);            // this way the result may fit in the source matrix
 
        /************************  MAGNITUDE *******************************/
        // compute the magnitude and switch to logarithmic scale
        // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
        split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
        magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
        magI = planes[0];
        
        /************************  Expand and Rearrange *******************************/
        // switch to logarithmic scale
        magI += Scalar::all(1);
        log(magI, magI);
        
        
        // crop the spectrum, if it has an odd number of rows or columns
        magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
        
        
        /************************  fftShift  *******************************/
        // rearrange the quadrants of Fourier image  so that the origin is at the image center
        fftShift(magI);
        
        /************************OBTAIN ARRAY 'F' TO DIFERENTS 'R' *******************************/
        findRho_impf(num,f);   //(Checked)

      
        for (i=1; i < arraySize;i++){
           // if(framess==10)
             //   add_points(log(i), log(f[i]));
            points.push_back(Point2f((float)log(i), (float)log(f[i])));
        }


		fitLine(points, line, CV_DIST_L2, 0, 0.01, 0.01); //0.01 would be a good default value for reps and aeps.
		alpha = 0.0f;
        alpha = line[1] / line[0];
        if(alpha<0)
            add_alpha(alpha);
        else
            add_alpha(0);
        analisa();
        
        //Reset arrays 'F' & 'points' to a new iteration
		memset(f, 0, arraySize * sizeof(float));
        memset(num, 0, arraySize * sizeof(int));
        points.clear();

        //cout<< framess++ <<endl;
        
        imshow("D", padded );
        if(waitKey(30) >= 0) break;
        
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    
	cap.release();
    time(&timeF);
    seconds = difftime(timeF, timeI);
    
    cout << "Nº frames: " << get_nframes() << endl;
    printf ("Time: %f seconds\n",seconds);
    return 1;
}
