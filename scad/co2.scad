$fn = 100;
$fs = 0.01;

use <gubg/box.scad>
use <gubg/stop.scad>
include <gubg/grove.scad>
include <gubg/arduino.scad>

t = 2;

w = gubg_arduino_uno_size[0]+7*t;
d = gubg_arduino_uno_size[1]+6*t+19;
h = 30;
cl = 0.4;

arduino_h = 2;

module vent_holes()
{
    for (xi=[0:8])
    {
        x = 6+xi*7.8;
        for (zi=[0:2])
        {
            z = 5+zi*8;

            //We use 45-degree cubes to allow printing without support
            r = 3;
            translate([x,-3*t-$fs,z])
            rotate(45,[0,1,0])
            rotate(-90,[1,0,0])
            translate([-r/2,-r/2,0])
            cube([r,r,t+2*$fs]);
        }
    }
}

/* if (false) */
    translate([3*t,0,0])
union()
{
    translate([0,0,t])
    {
        difference()
        {
            translate([-t-cl,-3*t,-t])
            gubg_box(w,d,h,t, 1, cl);

            translate([0,0,arduino_h])
            gubg_arduino_uno_mock(cl);

            vent_holes();
        }
        gubg_arduino_uno_mount(arduino_h, cl);
    }

    translate([70,-20,0])
    gubg_arduino_uno_slider(cl);

    if (false)
        translate([0,0,arduino_h])
        gubg_arduino_uno_mock(cl);
}

/* if (false) */
    translate([-w-t,0,0])
union()
{
    scd30_pos = [w-60-3*t-3*t,0,0];
    oled_pos  = [w-40-3*t-3*t,d-20-3*t-3*t,0];

    translate([0,0,t])
    {
        difference()
        {
            translate([w-3*t,-3*t,-t])
            gubg_box(w,d,h,t, -1, cl);

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
                gubg_grove_oled_mock(0.5*cl);
            }

            vent_holes();
        }

        translate(oled_pos)
        gubg_grove_oled_mount(0, cl);

        translate(scd30_pos)
        gubg_grove_scd30_mount(0, cl);
    }

    translate([0,-20,0])
    gubg_grove_oled_slider(cl);

    translate([20,-20,0])
    gubg_grove_scd30_slider(cl);
}

