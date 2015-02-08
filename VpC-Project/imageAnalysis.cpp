//
//  imageAnalysis.cpp
//  VC-Projeto
//
//  Created by Hélder Gonçalves on 22/12/14.
//  Copyright (c) 2014 Hélder Gonçalves. All rights reserved.
//

#include "imageAnalysis.hpp"

#define SIZE 1000


FILE *f = fopen("register.data", "w");
float alphaHistory[SIZE];
int frame = 0;


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


void add_alpha(float alpha){
    
    fprintf(f, "%d %f\n",frame,alpha);
    
    alphaHistory[frame%SIZE]=alpha;
    frame++;
}








