//
//  shapes.h
//  interpolation
//
//  Created by Jose Maria Benito on 28/10/14.
//  Copyright (c) 2014 Jose Maria. All rights reserved.
//

#ifndef interpolation_shapes_h
#define interpolation_shapes_h

#include "GLInclude.h"
#include <vector>

namespace std
{
    template <class RandomAccessIterator>
    void sort (RandomAccessIterator first, RandomAccessIterator last);
}

typedef struct {
    std::vector<VECTOR3D> P;
} CURVE;

void drawDot(VECTOR3D position, float sradius = 1, COLOUR color = grey)
{
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);

    
    VECTOR3D p[4],n[4];
    int STEP = 30;
    for (int i=0;i<360;i+=STEP) {
        for (int j=-90;j<90;j+=STEP) {
            
            p[0].x = sradius * cos(j*DTOR) * cos(i*DTOR);
            p[0].y = sradius * sin(j*DTOR);
            p[0].z = sradius * cos(j*DTOR) * sin(i*DTOR);
            n[0] = p[0];
            
            p[1].x = sradius * cos((j+STEP)*DTOR) * cos(i*DTOR);
            p[1].y = sradius * sin((j+STEP)*DTOR);
            p[1].z = sradius * cos((j+STEP)*DTOR) * sin(i*DTOR);
            n[1] = p[1];
            
            p[2].x = sradius * cos((j+STEP)*DTOR) * cos((i+STEP)*DTOR);
            p[2].y = sradius * sin((j+STEP)*DTOR);
            p[2].z = sradius * cos((j+STEP)*DTOR) * sin((i+STEP)*DTOR);
            n[2] = p[2];
            
            p[3].x = sradius * cos(j*DTOR) * cos((i+STEP)*DTOR);
            p[3].y = sradius * sin(j*DTOR);
            p[3].z = sradius * cos(j*DTOR) * sin((i+STEP)*DTOR);
            n[3] = p[3];
            
            glBegin(GL_POLYGON);
            if (i % (STEP*4) == 0)
                glColor3f(color.r,color.g,color.b);
            else
                glColor3f(color.r*0.5,color.g*0.5,color.b*0.5);
            for (int k=0;k<4;k++) {
                glNormal3f(n[k].x,n[k].y,n[k].z);
                glVertex3f(p[k].x,p[k].y,p[k].z);
            }
            glEnd();
        }
    }
    
    glPopMatrix();
}

typedef struct {
    std::vector<VECTOR3D> P;
} LINE;


void drawLine(LINE line, COLOUR color = grey, bool doDrawDots = false)
{
    if (doDrawDots)
    {
        for (VECTOR3D vector : line.P)
        {
            drawDot(vector, 0.05);
        }
    }
    
    glBegin(GL_LINE_STRIP);
    glColor3f(color.r, color.g, color.b);
    for (VECTOR3D vector : line.P)
    {
        glVertex3f(vector.x, vector.y, vector.z);
    }

    glEnd();
}

 void drawAxis()
{    
    VECTOR3D p0 {0, 0, 0};
    VECTOR3D x {1, 0, 0};
    VECTOR3D y {0, 1, 0};
    VECTOR3D z {0, 0, 1};
    
    LINE xAxis;
    xAxis.P.push_back(p0);
    xAxis.P.push_back(x);
    drawLine(xAxis, red, true);
    
    LINE yAxis;
    yAxis.P.push_back(p0);
    yAxis.P.push_back(y);
    drawLine(yAxis, green, true);
    
    LINE zAxis;
    zAxis.P.push_back(p0);
    zAxis.P.push_back(z);
    drawLine(zAxis, blue, true);
}

CURVE curbePoints(std::vector<VECTOR3D>::iterator start, std::vector<VECTOR3D>::iterator end)
{
    std::vector<VECTOR3D> points(start, end);
    CURVE resultCurve;
    resultCurve.P = points;
    return resultCurve;
}

VECTOR3D interpolate(CURVE curve, float t)
{
    if (curve.P.empty())
    {
        return {0, 0};
    }
    
    if (curve.P.size() > 1)
    {
        VECTOR3D p1 = interpolate(curbePoints(curve.P.begin(), curve.P.end() - 1), t);
        VECTOR3D p2 = interpolate(curbePoints(curve.P.begin() + 1, curve.P.end()), t);
        return Add(MultiplyWithScalar(1 - t, p1), MultiplyWithScalar(t, p2));
    }
    else
    {
        return curve.P[0];
    }
}

void drawCurve(CURVE curve, COLOUR color = grey)
{
    VECTOR3D lineResult;
    
    glBegin(GL_LINE_STRIP);
    glColor3f(color.r, color.g, color.b);
    for (float i = 0; i <= 1; i += 0.01)
    {
        lineResult = interpolate(curve, i);
        glVertex3d(lineResult.x, lineResult.y, lineResult.z);
    }
    glEnd();
}

#endif
