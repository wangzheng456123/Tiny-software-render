#ifndef PLATFORM_H
#define PLATFORM_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <windows.h>
#include "../base/cameral.h"

struct State {
    int lpressed;
    Vector2f rotate_offset;
    int rpressed;
    Vector2f trnaslate_offset; 
    Point2f cloc;
    int dolly;
    int wpressed, apressed, spressed, dpressed;
    int kpressed, Lpressed;
    State() :
        lpressed(0), rpressed(0), wpressed(0), apressed(0),
        spressed(0), dpressed(0), kpressed(0), Lpressed(0), dolly(0) {};

};

typedef enum { KEY_A, KEY_D, KEY_S, KEY_W, KEY_K, KEY_L, KEY_SPACE, KEY_NUM } keycode_t;
typedef enum { BUTTON_L, BUTTON_R, BUTTON_NUM } button_t;

typedef struct Window Winow;

struct Callback {
    void key_callback(Window* window, keycode_t key, int pressed);
    void button_callback(Window* window, button_t button, int pressed);
    void scroll_callback(Window* window, float offset);
};

struct Window {
    HWND handle;
    HDC memory_dc;
    framebuffer* surface;
    /* common data */
    int should_close;
    char keys[KEY_NUM];
    char buttons[BUTTON_NUM];
    Callback call_back;
    void* userdata;
};

/* platform initialization */
void platform_initialize(void);
void platform_terminate(void);

/* window related functions */
Window *window_create(const char *title, int width, int height);
void window_destroy(Window *window);
int window_should_close(Window *window);
void window_set_userdata(Window *window, void *userdata);
void *window_get_userdata(Window *window);
void window_draw_buffer(Window *window, framebuffer *buffer);

/* input related functions */
void input_poll_events(void);
int input_key_pressed(Window *window, keycode_t key);
int input_button_pressed(Window *window, button_t button);
void input_query_cursor(Window *window, float *xpos, float *ypos);

/* misc platform functions */
float platform_get_time(void);

void cameral_update(Window *window, Cameral &cameral) ;

#endif
