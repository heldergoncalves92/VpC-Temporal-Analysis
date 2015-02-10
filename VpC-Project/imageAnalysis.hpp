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




int* generateHistogram(Mat image);
void add_alpha(float alpha);
int get_nframes();

void add_points(float x, float y);


#endif /* defined(__VC_Projeto__imageAnalysis__) */
