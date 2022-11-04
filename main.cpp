#include "mbed.h"

PwmOut left_clk(A0);
DigitalOut left_dir(A1);
DigitalOut enable(A2);
PwmOut right_clk(A3);
DigitalOut right_dir(A4);
InterruptIn button(USER_BUTTON);

DigitalIn left(PC_9);
DigitalIn mid(PB_8);
DigitalIn right(PB_9);

Ticker timer;

const int CW = 0;
const int CCW = 1;

void pressed() {
    left_dir = !left_dir;
    right_dir = !right_dir;
}

void released()
{
    left_dir = !left_dir;
    right_dir = !right_dir;
}

void straight() {
    left_dir = CCW;
    right_dir = CW;
    
    right_clk.period_ms(3);
    left_clk.period_ms(3);
}
void back() {
    left_dir = CW;
    right_dir = CCW;
    right_clk.period_ms(3);
    left_clk.period_ms(3);
}

void go_hard_left() {
    left_dir = CCW;
    right_dir = CW;
    left_clk.period_ms(5);
    right_clk.period_ms(1);
}
void go_hard_right() {
    left_dir = CCW;
    right_dir = CW;
    left_clk.period_ms(1);
    right_clk.period_ms(5);
}
void go_left() {
    left_dir = CCW;
    right_dir = CW;
    left_clk.period_ms(4);
    right_clk.period_ms(2);
}
void go_right() {
    left_dir = CCW;
    right_dir = CW;
    left_clk.period_ms(2);
    right_clk.period_ms(4);
}

void callback() {
    int left_val = left.read(), mid_val = mid.read(), right_val = right.read();
    if(left_val == 0 && mid_val == 1 && right_val == 1)
        go_hard_left() ;
    else if (left_val == 0 && mid_val == 0 && right_val == 1)    
        go_left() ;
    else if (left_val == 1 && mid_val == 0 && right_val == 1)
        straight();
    else if (left_val == 1 && mid_val == 0 && right_val == 0)
        go_right() ;
    else if (left_val == 1 && mid_val == 1 && right_val == 0)
        go_hard_right() ;
    else
        back();
    
}
int main()
{    
    left_clk.write(0.5); // write 0.5 고정
    left_clk.period_ms(1); // period_ms로 속도 조절
    left_dir = CW;
    right_clk.write(0.5);
    right_clk.period_ms(1);
    right_dir = CW;
    enable = true;
    
    button.fall(&pressed);
    button.rise(&released);
    
    timer.attach(&callback, 0.1);
    while(1) { }
}
