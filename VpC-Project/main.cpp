//
//  main.cpp
//  VC-Projeto
//
//  Created by Hélder Gonçalves on 11/12/14.
//  Copyright (c) 2014 Hélder Gonçalves. All rights reserved.
//

#include "includes.hpp"

int main(){
    
    int m, n, cy, cx;
    Mat frame, padded;
    Mat planes[2], complexI;
    Mat magI;
    
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    
    Mat edges;
    namedWindow("edges",1);
    
    while (true) {
    
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, CV_BGR2GRAY);
        
        int m = getOptimalDFTSize( frame.rows );
        int n = getOptimalDFTSize( frame.cols );
        
        // on the border add zero values
        copyMakeBorder(edges, padded, 0, m - edges.rows, 0, n - edges.cols, BORDER_CONSTANT, Scalar::all(0));
        
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
        cx = magI.cols/2;
        cy = magI.rows/2;
        
        Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
        Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
        Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
        Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right
        
        Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
        q0.copyTo(tmp);
        q3.copyTo(q0);
        tmp.copyTo(q3);
        
        q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
        q2.copyTo(q1);
        tmp.copyTo(q2);
        
        normalize(magI, magI, 0, 1, CV_MINMAX); // Transform the matrix with float values into a
        // viewable image form (float between values 0 and 1).

        
        
        
     //   cvCartToPolar(, <#const CvArr *y#><#const CvArr *x#>, CvArr *magnitude)
        
        
        imshow("spectrum magnitude", magI);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    
    
    return 1;
}
