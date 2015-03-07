#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <rgui/rgui.h>
#include <glre/utils/window.h>
#include <GLFW/glfw3.h>

rgui::Keycode FromGLFW[348] = {
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(rgui::RGUI_SPACE),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(rgui::RGUI_APOSTROPHE),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(rgui::RGUI_COMMA),
static_cast<rgui::Keycode>(rgui::RGUI_MINUS),
static_cast<rgui::Keycode>(rgui::RGUI_PERIOD),
static_cast<rgui::Keycode>(rgui::RGUI_SLASH),
static_cast<rgui::Keycode>(rgui::RGUI_0),
static_cast<rgui::Keycode>(rgui::RGUI_1),
static_cast<rgui::Keycode>(rgui::RGUI_2),
static_cast<rgui::Keycode>(rgui::RGUI_3),
static_cast<rgui::Keycode>(rgui::RGUI_4),
static_cast<rgui::Keycode>(rgui::RGUI_5),
static_cast<rgui::Keycode>(rgui::RGUI_6),
static_cast<rgui::Keycode>(rgui::RGUI_7),
static_cast<rgui::Keycode>(rgui::RGUI_8),
static_cast<rgui::Keycode>(rgui::RGUI_9),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(rgui::RGUI_SEMICOLON),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(rgui::RGUI_EQUALS),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(rgui::RGUI_A),
static_cast<rgui::Keycode>(rgui::RGUI_B),
static_cast<rgui::Keycode>(rgui::RGUI_C),
static_cast<rgui::Keycode>(rgui::RGUI_D),
static_cast<rgui::Keycode>(rgui::RGUI_E),
static_cast<rgui::Keycode>(rgui::RGUI_F),
static_cast<rgui::Keycode>(rgui::RGUI_G),
static_cast<rgui::Keycode>(rgui::RGUI_H),
static_cast<rgui::Keycode>(rgui::RGUI_I),
static_cast<rgui::Keycode>(rgui::RGUI_J),
static_cast<rgui::Keycode>(rgui::RGUI_K),
static_cast<rgui::Keycode>(rgui::RGUI_L),
static_cast<rgui::Keycode>(rgui::RGUI_M),
static_cast<rgui::Keycode>(rgui::RGUI_N),
static_cast<rgui::Keycode>(rgui::RGUI_O),
static_cast<rgui::Keycode>(rgui::RGUI_P),
static_cast<rgui::Keycode>(rgui::RGUI_Q),
static_cast<rgui::Keycode>(rgui::RGUI_R),
static_cast<rgui::Keycode>(rgui::RGUI_S),
static_cast<rgui::Keycode>(rgui::RGUI_T),
static_cast<rgui::Keycode>(rgui::RGUI_U),
static_cast<rgui::Keycode>(rgui::RGUI_V),
static_cast<rgui::Keycode>(rgui::RGUI_W),
static_cast<rgui::Keycode>(rgui::RGUI_X),
static_cast<rgui::Keycode>(rgui::RGUI_Y),
static_cast<rgui::Keycode>(rgui::RGUI_Z),
static_cast<rgui::Keycode>(rgui::RGUI_LEFTBRACKET),
static_cast<rgui::Keycode>(rgui::RGUI_BACKSLASH),
static_cast<rgui::Keycode>(rgui::RGUI_RIGHTBRACKET),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(rgui::RGUI_GRAVE),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(rgui::RGUI_ESCAPE),
static_cast<rgui::Keycode>(rgui::RGUI_RETURN),
static_cast<rgui::Keycode>(rgui::RGUI_TAB),
static_cast<rgui::Keycode>(rgui::RGUI_BACKSPACE),
static_cast<rgui::Keycode>(rgui::RGUI_INSERT),
static_cast<rgui::Keycode>(rgui::RGUI_DELETE),
static_cast<rgui::Keycode>(rgui::RGUI_RIGHT),
static_cast<rgui::Keycode>(rgui::RGUI_LEFT),
static_cast<rgui::Keycode>(rgui::RGUI_DOWN),
static_cast<rgui::Keycode>(rgui::RGUI_UP),
static_cast<rgui::Keycode>(rgui::RGUI_PAGEUP),
static_cast<rgui::Keycode>(rgui::RGUI_PAGEDOWN),
static_cast<rgui::Keycode>(rgui::RGUI_HOME),
static_cast<rgui::Keycode>(rgui::RGUI_END),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(rgui::RGUI_CAPSLOCK),
static_cast<rgui::Keycode>(rgui::RGUI_SCROLLLOCK),
static_cast<rgui::Keycode>(rgui::RGUI_NUMLOCKCLEAR),
static_cast<rgui::Keycode>(rgui::RGUI_PRINTSCREEN),
static_cast<rgui::Keycode>(rgui::RGUI_PAUSE),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(rgui::RGUI_F1),
static_cast<rgui::Keycode>(rgui::RGUI_F2),
static_cast<rgui::Keycode>(rgui::RGUI_F3),
static_cast<rgui::Keycode>(rgui::RGUI_F4),
static_cast<rgui::Keycode>(rgui::RGUI_F5),
static_cast<rgui::Keycode>(rgui::RGUI_F6),
static_cast<rgui::Keycode>(rgui::RGUI_F7),
static_cast<rgui::Keycode>(rgui::RGUI_F8),
static_cast<rgui::Keycode>(rgui::RGUI_F9),
static_cast<rgui::Keycode>(rgui::RGUI_F10),
static_cast<rgui::Keycode>(rgui::RGUI_F11),
static_cast<rgui::Keycode>(rgui::RGUI_F12),
static_cast<rgui::Keycode>(rgui::RGUI_F13),
static_cast<rgui::Keycode>(rgui::RGUI_F14),
static_cast<rgui::Keycode>(rgui::RGUI_F15),
static_cast<rgui::Keycode>(rgui::RGUI_F16),
static_cast<rgui::Keycode>(rgui::RGUI_F17),
static_cast<rgui::Keycode>(rgui::RGUI_F18),
static_cast<rgui::Keycode>(rgui::RGUI_F19),
static_cast<rgui::Keycode>(rgui::RGUI_F20),
static_cast<rgui::Keycode>(rgui::RGUI_F21),
static_cast<rgui::Keycode>(rgui::RGUI_F22),
static_cast<rgui::Keycode>(rgui::RGUI_F23),
static_cast<rgui::Keycode>(rgui::RGUI_F24),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(rgui::RGUI_KP_0),
static_cast<rgui::Keycode>(rgui::RGUI_KP_1),
static_cast<rgui::Keycode>(rgui::RGUI_KP_2),
static_cast<rgui::Keycode>(rgui::RGUI_KP_3),
static_cast<rgui::Keycode>(rgui::RGUI_KP_4),
static_cast<rgui::Keycode>(rgui::RGUI_KP_5),
static_cast<rgui::Keycode>(rgui::RGUI_KP_6),
static_cast<rgui::Keycode>(rgui::RGUI_KP_7),
static_cast<rgui::Keycode>(rgui::RGUI_KP_8),
static_cast<rgui::Keycode>(rgui::RGUI_KP_9),
static_cast<rgui::Keycode>(rgui::RGUI_KP_DECIMAL),
static_cast<rgui::Keycode>(rgui::RGUI_KP_DIVIDE),
static_cast<rgui::Keycode>(rgui::RGUI_KP_MULTIPLY),
static_cast<rgui::Keycode>(rgui::RGUI_KP_MINUS),
static_cast<rgui::Keycode>(rgui::RGUI_KP_PLUS),
static_cast<rgui::Keycode>(rgui::RGUI_KP_ENTER),
static_cast<rgui::Keycode>(rgui::RGUI_KP_EQUALS),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(0),
static_cast<rgui::Keycode>(rgui::RGUI_LSHIFT),
static_cast<rgui::Keycode>(rgui::RGUI_LCTRL),
static_cast<rgui::Keycode>(rgui::RGUI_LALT),
static_cast<rgui::Keycode>(rgui::RGUI_LGUI),
static_cast<rgui::Keycode>(rgui::RGUI_RSHIFT),
static_cast<rgui::Keycode>(rgui::RGUI_RCTRL),
static_cast<rgui::Keycode>(rgui::RGUI_RALT),
static_cast<rgui::Keycode>(rgui::RGUI_RGUI) };
/*
rgui::Keycode FromGLFW[348] = {
0,0,0,0,0,0,0,0,           // 7
0,0,0,0,0,0,0,0,           // 15
0,0,0,0,0,0,0,0,           // 23
0,0,0,0,0,0,0,0,           // 31
rgui::RGUI_SPACE,          // 32
0,0,0,0,0,0,               // 38
rgui::RGUI_APOSTROPHE,     // 39
0,0,0,0,                   // 43
rgui::RGUI_COMMA,          // 44
rgui::RGUI_MINUS,          // 45
rgui::RGUI_PERIOD,         // 46
rgui::RGUI_SLASH,          // 47
rgui::RGUI_0,              // 48
rgui::RGUI_1,              // 49
rgui::RGUI_2,              // 50
rgui::RGUI_3,              // 51
rgui::RGUI_4,              // 52
rgui::RGUI_5,              // 53
rgui::RGUI_6,              // 54
rgui::RGUI_7,              // 55
rgui::RGUI_8,              // 56
rgui::RGUI_9,              // 57
0,                         // 58
rgui::RGUI_SEMICOLON,      // 59
0,
rgui::RGUI_EQUALS,         // 61
0,0,0,
rgui::RGUI_A,              // 65
rgui::RGUI_B,              // 66
rgui::RGUI_C,              // 67
rgui::RGUI_D,              // 68
rgui::RGUI_E,              // 69
rgui::RGUI_F,              // 70
rgui::RGUI_G,              // 71
rgui::RGUI_H,              // 72
rgui::RGUI_I,              // 73
rgui::RGUI_J,              // 74
rgui::RGUI_K,              // 75
rgui::RGUI_L,              // 76
rgui::RGUI_M,              // 77
rgui::RGUI_N,              // 78
rgui::RGUI_O,              // 79
rgui::RGUI_P,              // 80
rgui::RGUI_Q,              // 81
rgui::RGUI_R,              // 82
rgui::RGUI_S,              // 83
rgui::RGUI_T,              // 84
rgui::RGUI_U,              // 85
rgui::RGUI_V,              // 86
rgui::RGUI_W,              // 87
rgui::RGUI_X,              // 88
rgui::RGUI_Y,              // 89
rgui::RGUI_Z,              // 90
rgui::RGUI_LEFTBRACKET,    // 91
rgui::RGUI_BACKSLASH,      // 92
rgui::RGUI_RIGHTBRACKET,   // 93
0,0,                       // 95
rgui::RGUI_GRAVE,          // 96
0,0,0,0,                   // 100
0,0,0,0,0,0,0,0,0,0,       // 110
0,0,0,0,0,0,0,0,0,0,       // 120
0,0,0,0,0,0,0,0,0,0,       // 130
0,0,0,0,0,0,0,0,0,0,       // 140
0,0,0,0,0,0,0,0,0,0,       // 150
0,0,0,0,0,0,0,0,0,0,       // 160
0,0,                       // 161,162
0,0,0,0,0,0,0,0,           // 170
0,0,0,0,0,0,0,0,0,0,       // 180
0,0,0,0,0,0,0,0,0,0,       // 190
0,0,0,0,0,0,0,0,0,0,       // 200
0,0,0,0,0,0,0,0,0,0,       // 210
0,0,0,0,0,0,0,0,0,0,       // 220
0,0,0,0,0,0,0,0,0,0,       // 230
0,0,0,0,0,0,0,0,0,0,       // 240
0,0,0,0,0,0,0,0,0,0,       // 250
0,0,0,0,0,                 // 255
rgui::RGUI_ESCAPE,         // 256
rgui::RGUI_RETURN,         // 257
rgui::RGUI_TAB,            // 258
rgui::RGUI_BACKSPACE,      // 259
rgui::RGUI_INSERT,         // 260
rgui::RGUI_DELETE,         // 261
rgui::RGUI_RIGHT,          // 262
rgui::RGUI_LEFT,           // 263
rgui::RGUI_DOWN,           // 264
rgui::RGUI_UP,             // 265
rgui::RGUI_PAGEUP,         // 266
rgui::RGUI_PAGEDOWN,       // 267
rgui::RGUI_HOME,           // 268
rgui::RGUI_END,            // 269
0,0,0,0,0,0,0,0,0,0,       // 279
rgui::RGUI_CAPSLOCK,       // 280
rgui::RGUI_SCROLLLOCK,     // 281
rgui::RGUI_NUMLOCKCLEAR,   // 282
rgui::RGUI_PRINTSCREEN,    // 283
rgui::RGUI_PAUSE,          // 284
0,0,0,0,0,                 // 289
rgui::RGUI_F1,             // 290
rgui::RGUI_F2,             // 291
rgui::RGUI_F3,             // 292
rgui::RGUI_F4,             // 293
rgui::RGUI_F5,             // 294
rgui::RGUI_F6,             // 295
rgui::RGUI_F7,             // 296
rgui::RGUI_F8,             // 297
rgui::RGUI_F9,             // 298
rgui::RGUI_F10,            // 299
rgui::RGUI_F11,            // 300
rgui::RGUI_F12,            // 301
rgui::RGUI_F13,            // 302
rgui::RGUI_F14,            // 303
rgui::RGUI_F15,            // 304
rgui::RGUI_F16,            // 305
rgui::RGUI_F17,            // 306
rgui::RGUI_F18,            // 307
rgui::RGUI_F19,            // 308
rgui::RGUI_F20,            // 309
rgui::RGUI_F21,            // 310
rgui::RGUI_F22,            // 311
rgui::RGUI_F23,            // 312
rgui::RGUI_F24,            // 313
0             ,            // 314
0,0,0,0,0,                 // 319
rgui::RGUI_KP_0,           // 320
rgui::RGUI_KP_1,           // 321
rgui::RGUI_KP_2,           // 322
rgui::RGUI_KP_3,           // 323
rgui::RGUI_KP_4,           // 324
rgui::RGUI_KP_5,           // 325
rgui::RGUI_KP_6,           // 326
rgui::RGUI_KP_7,           // 327
rgui::RGUI_KP_8,           // 328
rgui::RGUI_KP_9,           // 329
rgui::RGUI_KP_DECIMAL,     // 330
rgui::RGUI_KP_DIVIDE,      // 331
rgui::RGUI_KP_MULTIPLY,    // 332
rgui::RGUI_KP_MINUS,       // 333
rgui::RGUI_KP_PLUS,        // 334
rgui::RGUI_KP_ENTER,       // 335
rgui::RGUI_KP_EQUALS,      // 336
0,0,0,                     // 339
rgui::RGUI_LSHIFT,         // 340
rgui::RGUI_LCTRL,          // 341
rgui::RGUI_LALT,           // 342
rgui::RGUI_LGUI,           // 343
rgui::RGUI_RSHIFT,         // 344
rgui::RGUI_RCTRL,          // 345
rgui::RGUI_RALT,           // 346
rgui::RGUI_RGUI            // 347
};
*/

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}


int main(void)
{

    auto W  = glre::utils::Window::create(640,480, "window 1");

    W->EventsMap["Exit"] = [&] (const glre::utils::Event & E)
    {
        if( E.type == glre::utils::FILEDROP)
        {
            for(int i=0;i<E.FileDrop.count;i++)
            {
                std::cout << E.FileDrop.files[i] << std::endl;
            }
        }
        else if (E.type == glre::utils::TEXT)
        {
            std::cout << E.Text.codepoint << std::endl;
        }
        else if( E.type == glre::utils::KEY)
        {
            std::cout << " Key pressed: " << E.Key.key << "," << E.Key.scancode << std::endl;
            if(E.Key.key == GLFW_KEY_ESCAPE)
            {
                //W->SetCursorMode( glre::utils::Window::CURSOR_NORMAL );
            }

        }
        else if( E.type == glre::utils::MOUSECURSOR)
        {
            std::cout << " Mouse Moved : " << E.MouseCursor.x << "," << E.MouseCursor.y << std::endl;
        }
        else if( E.type == glre::utils::WINDOW)
        {
            if( E.Window.action == glre::utils::FOCUS_GAINED)
                std::cout << " Window gained focus: " << std::endl;
            if( E.Window.action == glre::utils::FOCUS_LOST)
                std::cout << " Window lost focus: " << std::endl;
            if( E.Window.action == glre::utils::MINIMIZED)
                std::cout << " Window minimized: " << std::endl;
            if( E.Window.action == glre::utils::RESTORED)
                std::cout << " Window restored: " << std::endl;
        }
    };


    W->MakeCurrent();
    while ( !W->WantsToClose() )
    {

        float ratio;

        int width = 640;
        int height = 480;

        //glfwGetFramebufferSize(window, &width, &height);

        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.f, 0.f);
        glVertex3f(-0.6f, -0.4f, 0.f);
        glColor3f(0.f, 1.f, 0.f);
        glVertex3f(0.6f, -0.4f, 0.f);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.f, 0.6f, 0.f);
        glEnd();

        W->SwapBuffers();
        glre::utils::Window::PollEvents();
    }
    W.reset();

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
