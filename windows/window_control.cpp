#include "window.h"

void Callback::button_callback(Window *window, button_t button, int pressed) {
    State *state = (State *)window_get_userdata(window);
    float x, y;
    input_query_cursor(window, &x, &y);
    Point2f cursor_loc(x, y);
    if (button == BUTTON_L) {
        if (pressed) state->lpressed = 1, state->cloc = cursor_loc;
        else state->lpressed = 0; 
    }
    else {
        if (pressed) state->rpressed = 1, state->cloc = cursor_loc;
        else state->rpressed = 0;
    }
}

void Callback::scroll_callback(Window *window, float offset) {
    State *state = (State *)window_get_userdata(window);
    state->dolly += offset;
}

void Callback::key_callback(Window* window, keycode_t key, int pressed) {
    State* state = (State*)window_get_userdata(window);
    if (key == KEY_A) state->apressed = pressed;
    else if (key == KEY_S) state->spressed = pressed;
    else if (key == KEY_D) state->dpressed = pressed;
    else if (key == KEY_W) state->wpressed = pressed;
    else if (key == KEY_K) state->kpressed = pressed;
    else if (key == KEY_L) state->Lpressed = pressed;
    return;
}

void cameral_update(Window *window, Cameral &cameral) {
    State *state = (State *)window_get_userdata(window);
    float x, y;
    input_query_cursor(window, &x, &y);
    Point2f cursor_loc(x, y);
    if (state->lpressed) 
        cameral.update_pos(1 / 800.f * (state->cloc - cursor_loc), 0);
    if (state->rpressed)
        cameral.update_look(1 / 800.f * (state->cloc - cursor_loc));
    if (state->wpressed)
        cameral.update_pos(Vector2f(0, 0.02), 0);
    if (state->spressed)
        cameral.update_pos(Vector2f(0, -0.02), 0);
    if (state->apressed)
        cameral.update_pos(Vector2f(0.02, 0), 0);
    if (state->dpressed)
        cameral.update_pos(Vector2f(-0.02, 0), 0);
    if (state->kpressed)
        cameral.update_pos(Vector2f(0, 0), -1);
    if (state->Lpressed)
        cameral.update_pos(Vector2f(0, 0), 1);
    state->cloc = cursor_loc;
}
