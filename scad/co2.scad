$fn = 100;
$fs = 0.01;

use <gubg/box.scad>
use <gubg/stop.scad>
include <gubg/grove.scad>
include <gubg/arduino.scad>

t = 2;

b = gubg_arduino_uno_size[0]+6*t;
d = gubg_arduino_uno_size[1]+6*t+10;
h = 30;
clearance = 0.4;

/* if (false) */
translate([t,0,0])
union()
{
    difference()
    {
        gubg_box(b,d,h,t, 1, clearance);

        //USB connector
        translate([-$fs,t+37,t+t+gubg_arduino_uno_size[2]])
        cube([t+2*$fs,12,7]);
    }
    translate([t,2*t,t])
    union()
    {
        for (co=gubg_arduino_uno_holes)
        {
            translate(co)
            union()
            {
                cylinder(t,2.5,2);
                cylinder(t+1,1.2,1.2);
            }
        }

        //Stopper on left wall
        translate([0,17/2,0])
        gubg_stop(t,17,t+gubg_arduino_uno_size[2],t);
        translate([0,5/2+gubg_arduino_uno_size[1]-5,0])
        gubg_stop(t,5,t+gubg_arduino_uno_size[2],t);

        translate([gubg_arduino_uno_size[0],15,0])
        union()
        {
            gubg_block(4*t,12,t+gubg_arduino_uno_size[2], 2, 0.2);
            color([0,1,1])
            if (false)
            translate([-2,0,t+gubg_arduino_uno_size[2]])
            gubg_slider(4*t,12,2);
        }

    if (false)
        translate([0,0,t])
        gubg_arduino_uno_mock([1,0,0]);
    }

    translate([0,-10,0])
    gubg_slider(4*t,12,2);
}

/* if (false) */
translate([-b-t,0,0])
union()
{
    difference()
    {
        translate([b,0,0])
        gubg_box(b,d,h,t, -1, clearance);

        color([1,0,0])
        translate([b-3*t,20+t+2,10])
        rotate(180,[0,0,1])
        rotate(180,[1,0,0])
        gubg_grove_scd30(0.5*clearance);

        color([0,1,0])
        translate([b-3*t,d-18,2+$fs])
        rotate(180,[0,0,1])
        gubg_grove_oled(0.5*clearance);
    }

    //For SCD30
    translate([b-3*t,20+t+2,0])
    union()
    {
        for (co=gubg_grove_scd30_holes)
        {
            translate([-co[0],co[1],0])
            {
                cylinder(10,2.5,2);
                cylinder(11,0.5,0.5);
            }
        }

        {
            rotate(180,[0,0,1])
            gubg_stop(t+2,15,10+1.5,t);

            translate([-60,12,0])
            rotate(180, [0,0,1])
            gubg_block(b-60-4*t,6,10+1.5,t,clearance);
        }
    }

    //For OLED
    translate([b-3*t,d-18+10,0])
    union()
    {
        translate([0,-20,0])
        {
            for (co=gubg_grove_oled_holes)
            {
                translate([-co[0],co[1]+10,0])
                {
                    cylinder(t+1,0.5,0.5);
                }
            }
        }

        rotate(90,[0,0,1])
        translate([-20,19,0])
        gubg_stop(t,10,t+1.5,t);

        rotate(90,[0,0,1])
        translate([0,19,0])
        gubg_block(18-10-t,8,t+1.5,t,clearance);
    }

    translate([0,-10,0])
    gubg_slider(b-60-4*t,6,t);

    translate([20,-10,0])
    gubg_slider(18-10-t,8,t);
}

