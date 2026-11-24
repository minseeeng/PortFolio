#pragma once
enum MOUSE_TYPE
{
    MOUSE_MOVE,
    MOUSE_DOWN,
    MOUSE_UP
};

struct MouseEvent
{
    MOUSE_TYPE type;
    int x;
    int y;
};