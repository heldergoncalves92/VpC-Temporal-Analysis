//
//  imageAnalysis.h
//  VC-Projeto
//
//  Created by Hélder Gonçalves on 22/12/14.
//  Copyright (c) 2014 Hélder Gonçalves. All rights reserved.
//

#ifndef __VC_Projeto__imageAnalysis__
#define __VC_Projeto__imageAnalysis__

#include "includes.hpp"

void setDim(int dim);
Mat thresholdAdaptative(Mat original, Mat image);


void add_alpha(float alpha);
int get_nframes();
int analisa();

void add_points(float x, float y);


#endif /* defined(__VC_Projeto__imageAnalysis__) */
