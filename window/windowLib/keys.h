#pragma once

enum class Command: byte
{
    Direction,
    Step
};

enum class Key: unsigned short
{
    Left = 1,
    Right,
    Up,
    Down,
    Space,
    None
};
