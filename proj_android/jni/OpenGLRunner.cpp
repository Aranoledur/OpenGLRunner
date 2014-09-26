#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../source/Game.h"

EasyGodzilla::Game game;

bool setupGraphics(int w, int h) {

    glViewport(0, 0, w, h);
    EasyGodzilla::checkGlError("glViewport");

    glClearColor(1.f, 1.f, 0.f, 1.f);

    game.Release();
    game.Init();
    return true;
}

const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
        0.5f, -0.5f };

void renderFrame() {
    game.DrawFrame();
}

extern "C" {
    JNIEXPORT void JNICALL Java_com_easygodzilla_openglrunner_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_easygodzilla_openglrunner_GL2JNILib_step(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_easygodzilla_openglrunner_GL2JNILib_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_easygodzilla_openglrunner_GL2JNILib_step(JNIEnv * env, jobject obj)
{
    renderFrame();
}
