use <fixer.scad>
use <stop.scad>

gubg_arduino_uno_size = [68.6, 53.3, 1];
gubg_arduino_uno_holes = [
[14,   2.5],
[66.1, 7.6],
[66.1, 35.5],
[15.3, 50.7],
];
gubg_arduino_uno_fixer = [20,10,2];

module gubg_arduino_uno_mount(h, cl)
{
    r0 = 2.5;
    r1 = 1;
    for (co=gubg_arduino_uno_holes)
    {
        translate([co[0],co[1],-$fs])
        {
            cylinder(h,r0,r0);

            translate([0,0,h])
            cylinder(gubg_arduino_uno_size[2],r1,0);
        }
    }

    translate([gubg_arduino_uno_size[0],13,0])
    gubg_fixer_track(gubg_arduino_uno_fixer[0],h+gubg_arduino_uno_fixer[1],gubg_arduino_uno_fixer[2], cl);

    t = gubg_arduino_uno_fixer[2];
    translate([-cl,17/2,0])
    gubg_stop(t,17,h+cl+gubg_arduino_uno_size[2],t);
    translate([-cl,4/2+gubg_arduino_uno_size[1]-4,0])
    gubg_stop(t,4,h+cl+gubg_arduino_uno_size[2],t);
}

module gubg_arduino_uno_slider(cl)
{
    gubg_fixer_slider(gubg_arduino_uno_fixer[0],gubg_arduino_uno_fixer[1],gubg_arduino_uno_fixer[2], cl);
}

module gubg_arduino_uno_mock(cl)
{
    color([1,0,0])
    difference()
    {
        w = gubg_arduino_uno_size[0]+2*cl;
        d = gubg_arduino_uno_size[1]+2*cl;
        h = gubg_arduino_uno_size[2]+cl;
        union()
        {
            //Base plate
            translate([-cl,-cl,0])
            cube([w,d,h]);

            translate([0,0,h])
            {
                //USB connector
                translate([-18,37,-(7-2.5)/2])
                cube([18,12,7]);

                //Grove and Power connector
                translate([0,17,0])
                cube([5,20,8.5]);

                //Headers
                translate([26,0.5,0])
                cube([39,2.5,8.5]);
                translate([17,49,0])
                cube([48,2.5,8.5]);
            }
        }

        r = (3-cl)/2;
        for (co=gubg_arduino_uno_holes)
            translate([co[0],co[1],-$fs])
            cylinder(h+2*$fs,r,r);
    }
}

if (false)
{
    $fn = 100;
    $fs = 0.01;

    h = 10;

    gubg_arduino_uno_mount(h, 0.3);

    translate([gubg_arduino_uno_size[0],-13,h])
    gubg_arduino_uno_slider(0.3);

    translate([0,0,h+0])
    gubg_arduino_uno_mock(0.3);
}
