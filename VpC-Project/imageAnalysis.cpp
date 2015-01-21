//
//  imageAnalysis.cpp
//  VC-Projeto
//
//  Created by Hélder Gonçalves on 22/12/14.
//  Copyright (c) 2014 Hélder Gonçalves. All rights reserved.
//

#include "imageAnalysis.hpp"


//Função gera histograma para array
int* generateHistogram(Mat image){
    
    int *hist=(int*)malloc(256*sizeof(int));
    int i,j, grau;
    
    //Põe a Zeros o histograma
    for (i=0; i<256; i++)
        hist[i]=0;
    
    //Verifica o gru do pixel e adiciona no histograma
    for ( i=0; i<image.rows; i++)
        for ( j=0; j<image.cols; j++) {
            
            grau=image.at<uchar>(i, j);
            hist[grau]+=1;
        }
    return hist;
}

