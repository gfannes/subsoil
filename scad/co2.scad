$fn = 100;
$fs = 0.01;

use <gubg/box.scad>
use <gubg/stop.scad>
use <gubg/buzzer.scad>
include <gubg/grove.scad>
include <gubg/arduino.scad>

t = 2;

w = gubg_arduino_uno_size[0]+7*t;
d = gubg_arduino_uno_size[1]+6*t+17;
h = 40;
cl = 0.4;
slider_cl = 0.2;


module vent_holes_front(bt)
{
    for (xi=[0:8])
    {
        x = 2*t+6.5+xi*7.6;
        for (zi=[0:3])
        {
            z = 2*t+5+zi*8;

            //We use 45-degree cubes to allow printing without support
            r = 4.5;
            a = w/2; b = -w/2;
            translate([x+a*bt+b,-$fs,z])
            rotate(45,[0,1,0])
            rotate(-90,[1,0,0])
            translate([-r/2,-r/2,0])
            cube([r,r,t+2*$fs]);
        }
    }
}
module vent_holes_side(bt)
{
    for (yi=[0:8])
    {
        y = 2*t+6+yi*7.8;
        for (zi=[0:3])
        {
            z = 2*t+5+zi*8;

            //We use 45-degree cubes to allow printing without support
            r = 5;
            a = t/2; b = -t/2;
            translate([-$fs+a*bt+b,y,z])
            rotate(90,[0,1,0])
            rotate(45,[0,0,1])
            translate([-r/2,-r/2,0])
            cube([r,r,t+2*$fs]);
        }
    }
}

/* if (false) */
    translate([3*t,0,0])
union()
{
    arduino_h = 2.5;
    arduino_pos = [0,-1,0];

    translate([0,0,t])
    {
        difference()
        {
            translate([-t-cl,-2*t,-t])
            difference()
            {
                gubg_box(w,d,h,t, 1, cl);
                vent_holes_front(1);
                vent_holes_side(1);
            }

            translate(arduino_pos)
            translate([0,0,arduino_h])
            gubg_arduino_uno_mock(cl);

        }
        translate(arduino_pos)
        gubg_arduino_uno_mount(arduino_h, cl);
    }

    translate([70,-20,0])
    gubg_arduino_uno_slider(slider_cl);

    if (false)
        translate([0,0,arduino_h])
        gubg_arduino_uno_mock(cl);
}

/* if (false) */
    translate([-w-t,0,0])
union()
{
    scd30_pos  = [w-60-3*t-3*t,0,0];
    oled_pos   = [w-40-3*t-3*t,d-20-3*t-3*t,0];
    buzzer_pos = [5,oled_pos[1]+10,0];

    translate([0,0,t])
    {
        difference()
        {
            translate([w-3*t,-2*t,-t])
            difference()
            {
                gubg_box(w,d,h,t, -1, cl);
                vent_holes_front(-1);
                vent_holes_side(-1);
            }

            //SCD30
            {
                color([1,0,0])
                translate(scd30_pos)
                gubg_grove_scd30_mock(cl);
            }

            //OLED
            {
                color([0,1,0])
                translate(oled_pos)
                gubg_grove_oled_mock(cl);
            }

            //Buzzer
            {
                color([0,0,1])
                translate(buzzer_pos)
                gubg_buzzer_mock(cl);
            }
        }

        translate(oled_pos)
        gubg_grove_oled_mount(0, cl);

        translate(scd30_pos)
        gubg_grove_scd30_mount(0, cl);

        translate(buzzer_pos)
        translate([0,0,-t])
        gubg_buzzer_mount(t,cl);
    }

    translate([0,-20,0])
    gubg_grove_oled_slider(slider_cl);

    translate([20,-20,0])
    gubg_grove_scd30_slider(slider_cl);

    translate([40,-20,0])
    gubg_buzzer_slider(t,slider_cl);
}

