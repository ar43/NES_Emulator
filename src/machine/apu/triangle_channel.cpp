#include "triangle_channel.h"

void TriangleChannel::Clock()
{
    bool ultrasonic = false;
    if (freq_timer < 2 && freq_counter == 0)
        ultrasonic = true;

    bool clock_triunit = true;
    if (length_counter == 0 || linear_counter == 0 || ultrasonic)
        clock_triunit = false;
    
    if( clock_triunit )
    {
        if (freq_counter > 0)
            freq_counter--;
        else
        {
            freq_counter = freq_timer;
            tri_step = (tri_step + 1) & 0x1F;
        }
    }
    if (ultrasonic)                
        output = 0; //not sure if this is right
    else if (tri_step & 0x10)      
        output = tri_step ^ 0x1F;
    else                            
        output = tri_step;
}

void TriangleChannel::ClockQuarter()
{
    if (linear_reload)
        linear_counter = linear_load;
    else if (linear_counter > 0)
        linear_counter--;

    if (!linear_control)
        linear_reload = false;
}

void TriangleChannel::ClockHalf()
{
    if (length_enabled && length_counter)
        length_counter--;
}