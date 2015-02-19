//
//  imageAnalysis.cpp
//  VC-Projeto
//
//  Created by Hélder Gonçalves on 22/12/14.
//  Copyright (c) 2014 Hélder Gonçalves. All rights reserved.
//

#include "imageAnalysis.hpp"

#define SIZE 1000
#define TSTOP 5

FILE *falpha = fopen("register.data", "w");
FILE *fdelta = fopen("derivate.data", "w");

float alphaHistory[SIZE];
float delta_AlphaHistory[SIZE];


int frame = 0;
int fActual=0, faux;
int Idim;


float variacao = 0.01;
float media=0.04;
int afastar=0, aproximar=0;

void setDim(int dim){ Idim=dim;}

/***************** ALPHA **********************/
void add_alpha(float alpha){
    
    faux=fActual;
    fActual=frame%SIZE;
    float derivate;
    
    alphaHistory[fActual]=alpha;
    fprintf(falpha, "%d %f\n",frame,alpha);
    
    if(frame==0){
        delta_AlphaHistory[0]=0;
    }else{
        derivate = alpha - alphaHistory[faux] + delta_AlphaHistory[faux];
        if(derivate < 0){
            delta_AlphaHistory[fActual]=0;
            fprintf(fdelta,"%d 0\n",frame);

        }else{
            delta_AlphaHistory[fActual]=derivate;
            fprintf(fdelta,"%d %f\n",frame,derivate);
        }
    }
    frame++;
}

void add_points(float x, float y){
    fprintf(falpha, "%f %f\n",x,y);
}

int get_nframes(){return frame;}

int analisa(){
    float last = delta_AlphaHistory[fActual];
    float pen = delta_AlphaHistory[faux];
    float diff = last - pen;
    
    int num=4,i=0;
    float alpha;
    
    if(frame>num){
        Vec4f lines;
        vector<Point2f> pointss;
        
        
        for(i=num; i>2; i--){
            pointss.push_back(Point2f((float)num-i, delta_AlphaHistory[fActual-i] ));
        }
        
        pointss.push_back(Point2f(num-2, pen));
        pointss.push_back(Point2f(num-1, last));
        
        fitLine(pointss, lines, CV_DIST_L2, 0, 0.01, 0.01); //0.01 would be a good default value for reps and aeps.
        
        alpha = lines[1] / lines[0];
        pointss.clear();
        
    }

    
    if(last > 0.1 && abs(diff)>0.3)
        printf("%d -->COLISÂO!!\n",frame);
    
    
    else if(last > 0.09 && abs(diff)>0.02)
        printf("%d -->Muito PERTO!!\n",frame);
    
    else if(abs(diff)>0.03){
        
        if(alpha > -0.001){
            aproximar++; afastar=0;
            
            if(aproximar>1) printf("%d -->Algum Movimento!! - Aproximar\n",frame);
            else printf("%d -->Algum Movimento!!\n",frame);
        }
    
        else if(alpha < -0.0018){
            afastar++; aproximar=0;
            
            if(afastar>1) printf("%d -->Algum Movimento!! - Afastar\n",frame);
            else printf("%d -->Algum Movimento!!\n",frame);

        }
        else
            printf("%d -->Algum Movimento!!\n",frame);
    }
    return 1;
}







