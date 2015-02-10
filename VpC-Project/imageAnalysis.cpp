//
//  imageAnalysis.cpp
//  VC-Projeto
//
//  Created by Hélder Gonçalves on 22/12/14.
//  Copyright (c) 2014 Hélder Gonçalves. All rights reserved.
//

#include "imageAnalysis.hpp"

#define SIZE 1000


FILE *falpha = fopen("register.data", "w");
FILE *fdelta = fopen("derivate.data", "w");

float alphaHistory[SIZE];

float delta_AlphaHistory[SIZE];

int frame = 1;
int fActual=0, faux;


void add_alpha(float alpha){
    
    faux=fActual;
    fActual=frame%SIZE;
    
    alphaHistory[fActual]=alpha;
    fprintf(falpha, "%d\t%f\n",frame,alpha);
    
    if(frame==0){
        delta_AlphaHistory[0]=0;
        fprintf(fdelta, "0 0.0\n");
    }
    else{
        delta_AlphaHistory[fActual]=alpha - alphaHistory[faux] + delta_AlphaHistory[faux];
        fprintf(fdelta,"%d\t%f\n",frame,delta_AlphaHistory[fActual]);
    }
    frame++;
}

void add_points(float x, float y){
    fprintf(falpha, "%f\t%f\n",x,y);
}

int get_nframes(){return frame;}







