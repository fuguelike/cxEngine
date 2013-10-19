//
//  main.m
//  cxEngineMac
//
//  Created by xuhua on 10/16/13.
//  Copyright (c) 2013 xuhua. All rights reserved.
//
#include <GLUT/glut.h>
#include <core/cxEngine.h>
#import <Cocoa/Cocoa.h>

void cxDrawFrame(void)
{
    cxEngineDraw();
    glutSwapBuffers();
}

void cxMouseButton(int button, int state, int x, int y)
{
    CX_RETURN(button != GLUT_LEFT_BUTTON);
    cxVec2f pos = cxVec2fv(x, y);
    if(state == GLUT_DOWN){
        cxEngineFireTouch(cxTouchTypeDown, pos);
    }else if(state == GLUT_UP){
        cxEngineFireTouch(cxTouchTypeUp, pos);
    }else{
        cxEngineFireTouch(cxTouchTypeCancel, pos);
    }
}

void cxMouseMotion(int x, int y)
{
    cxVec2f pos = cxVec2fv(x, y);
    cxEngineFireTouch(cxTouchTypeMove, pos);
}

void cxEngineIdle()
{
    glutPostRedisplay();
}

void cxReshape(GLint width, GLint height)
{
    cxEngineLayout(width, height);
}

int main(int argc, char **argv)
{
    glutInit (&argc, argv);
    glutInitWindowSize (480, 640);
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA | GLUT_STENCIL);
    glutCreateWindow("cxEngine for Mac");

    if(!cxEngineInit(cxEngineInstance())){
        CX_ERROR("cxEngineInit return false,app exit");
        exit(-1);
    }
    
    glutDisplayFunc(cxDrawFrame);
    glutReshapeFunc(cxReshape);
    glutMouseFunc(cxMouseButton);
    glutMotionFunc(cxMouseMotion);
    glutIdleFunc(cxEngineIdle);
    
    glutMainLoop ();
    cxEngineDestroy();
    return 0;
}













