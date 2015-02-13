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

void setDim(int dim){ Idim=dim;}

int mediaImage(Mat image){
    int i,j,G1=0,sumG1=0;
    int ss;
    for(i=0;i<Idim;i++)
        for (j=0; j<Idim; j++) {
            G1++;
            sumG1+=image.at<uchar>(j, i);
        }
    if(sumG1==0) return 1;
    return sumG1/G1;
    
}

Mat thresholdAdaptative(Mat original, Mat image){
    
    int t=0, tnew, sumG1=0, sumG2,G1=0,G2,i,j, NITER=0;
    float u1,u2;
    
    //Calcular a media
    if(frame%10 == 0)
        tnew=mediaImage(original);
    
    
    while(abs(t-tnew) >= TSTOP){
        t=tnew;
        
        sumG1=sumG2=G1=G2=0;
        for(i=0;i<Idim;i++)
            for (j=0; j<Idim; j++) {
                if(original.at<uchar>(j, i) < t){
                    G1++;
                    sumG1+=original.at<uchar>(j, i);
                }
                else{
                    G2++;
                    sumG2+=original.at<uchar>(j, i);
                }
            }
        
        //Fica com threshold já calculado
        if(G1==0 || G2==0)
            break;
        
        
        //Compute the Average
        u1=sumG1/(float)G1;
        u2=sumG2/(float)G2;
        
        //Calculate Tnew
        tnew=(u1+u2)/2.0;
        
        
        NITER++;
        
    }
    
    //Threshold
    cout << "Threshold usado: "<<tnew;
    
    for(i=0;i<Idim;i++)
        for (j=0; j<Idim; j++) {
            if(original.at<uchar>(j, i) < tnew)
                image.at<uchar>(j, i)=0;
            else
                image.at<uchar>(j, i)=255;
        }
    
    return image;
}


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

float variacao = 0.01;
float media=0.04;

int analisa(){
    float last = delta_AlphaHistory[fActual];
    float pen = delta_AlphaHistory[faux];
    float diff = last - pen;
    
    if(last > 0.2 && abs(diff)>0.5)
        printf("COLISÂO!! \t\t-> %d\n",frame);
    
    
    else if(last > 0.09 && abs(diff)>0.02)
        printf("Muito PERTO!! \t-> %d\n",frame);
    
    else if(abs(diff)>0.03)
       printf("Algum Movimento!!\t-> %d\n",frame);
    
    return 1;
}







