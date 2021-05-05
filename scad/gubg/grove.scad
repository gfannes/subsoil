use <stop.scad>
use <fixer.scad>

gubg_grove_scd30_ears  = [[0,30], [10,0],[50,0],[60,30],[30,40]];
gubg_grove_scd30_holes = [[0,10], [30,0],[60,10],[50,40],[10,40]];
gubg_grove_scd30_fixer = [20,10,2];

module gubg_grove_scd30_mock(cl)
{
    difference()
    {
        union()
        {
            //Base plate
            translate([-cl,-cl,0])
            cube([60+2*cl,40+2*cl,1.5+cl]);

            //Ears
            for (co=gubg_grove_scd30_ears)
                translate(co)
                cylinder(1.5+cl,2,2);
        }

        //Ears removal
        for (co=gubg_grove_scd30_ears)
            translate(co)
            translate([0,0,-$fs])
            cylinder(1.5+cl+2*$fs,1.25,1.25);

        //Holes
        for (co=gubg_grove_scd30_holes)
        {
            translate(concat(co,[-$fs]))
            cylinder(1.5+cl+2*$fs,2.25,2.25);
        }
    }

    color([1,0,0])
    translate([0,0,1.5])
    {
        //Connector
        translate([0,20-13/2,0])
        cube([10,13,5]);

        //Sensor
        translate([15,6.5,0])
        {

            {
                cube([41.5+cl,27+cl,10+cl]);
                //Cylinders of sensor
                translate([0+1,27/2,-1.5-0.5-cl])
                {
                    r = (7+cl)/2;
                    cylinder(10+1.5+0.5+2*cl,r,r);
                    translate([41.5-2*1+cl,0,0])
                    cylinder(10+1.5+0.5+2*cl,r,r);
                }
            }

            //Wires on the bottom
            translate([3-cl,1.5-cl,-1.5-1-cl+$fs])
            cube([10+2*cl,2+2*cl,1+cl]);
        }
    }
}

module gubg_grove_scd30_mount(h,cl)
{
    for (co=gubg_grove_scd30_ears)
        translate(co)
        {
            cylinder(h,2.25,2.25);
            translate([0,0,h])
            cylinder(1.5,(1.5-cl)/2,0);
        }

    for (co=gubg_grove_scd30_holes)
        translate(co)
        {
            cylinder(h,3.5,3.5);
            translate([0,0,h])
            cylinder(1.5,(4-cl)/2,0);
        }

    translate([30,40,0])
    rotate(90,[0,0,1])
    gubg_fixer_track(gubg_grove_scd30_fixer[0],h+gubg_grove_scd30_fixer[1],gubg_grove_scd30_fixer[2], cl);

    translate([20,0,0])
    {
        d = 12;
        rotate(90,[0,0,1])
        gubg_stop(2,d,h+1.5+cl,2);

        translate([20,0,0])
        rotate(90,[0,0,1])
        gubg_stop(2,d,h+1.5+cl,2);
    }
}

module gubg_grove_scd30_slider(cl)
{
    gubg_fixer_slider(gubg_grove_scd30_fixer[0],gubg_grove_scd30_fixer[1],gubg_grove_scd30_fixer[2], cl);
}

gubg_grove_oled_ears  = [[10,0],[40,10],[10,20]];
gubg_grove_oled_holes = [[0,10],[30,0],[30,20]];
gubg_grove_oled_fixer = [20,5,2];

module gubg_grove_oled_mock(cl)
{
    difference()
    {
        union()
        {
            //Base plate
            translate([-cl,-cl,0])
            cube([40+2*cl,20+2*cl,1.5+cl]);

            //Ears
            for (co=gubg_grove_oled_ears)
                translate(co)
                cylinder(1.5+cl,2.25,2.25);
        }

        //Ears removal
        for (co=gubg_grove_oled_ears)
            translate(co)
            translate([0,0,-$fs])
            cylinder(1.5+cl+2*$fs,1.25,1.25);

        //Holes
        for (co=gubg_grove_oled_holes)
            translate(concat(co,[-$fs]))
            cylinder(1.5+cl+2*$fs,2,2);
    }

    //Connector
    translate([0,10-13/2,1.5])
    cube([10,13,5]);

    //Screen
    translate([10.5-cl,-cl,-2-cl+$fs])
    cube([26.5+2*cl,20+2*cl,2+cl]);
}

module gubg_grove_oled_mount(h,cl)
{
    for (co=gubg_grove_oled_ears)
        translate(co)
        {
            cylinder(h,2.25,2.25);
            translate([0,0,h])
            cylinder(1.5,(1.5-cl)/2,0);
        }

    translate(gubg_grove_oled_holes[0])
    {
        cylinder(h,3.5,3.5);
        translate([0,0,h])
        cylinder(1.5,(4-cl)/2,0);
    }

    translate([20,20,0])
    rotate(-90,[0,0,1])
    gubg_stop(2,12,h+1.5+cl,2);

    translate([20,0,0])
    rotate(-90,[0,0,1])
    gubg_fixer_track(gubg_grove_oled_fixer[0],h+gubg_grove_oled_fixer[1],gubg_grove_oled_fixer[2], cl);
}

module gubg_grove_oled_slider(cl)
{
    gubg_fixer_slider(gubg_grove_oled_fixer[0],gubg_grove_oled_fixer[1],gubg_grove_oled_fixer[2], cl);
}

if (false)
    translate([50,0,0])
{
    $fn = 100;
    $fs = 0.01;

    h = 10;

    gubg_grove_scd30_mount(h,0.2);

    translate([0,-20,0])
    gubg_grove_scd30_slider(0.2);

    translate([0,0,5+h])
    gubg_grove_scd30_mock(0.0);
}

if (false)
{
    $fn = 100;
    $fs = 0.01;

    h = 10;

    gubg_grove_oled_mount(h,0.2);

    translate([0,-20,0])
    gubg_grove_oled_slider(0.2);

    color([1,0,0])
    translate([0,0,h])
    gubg_grove_oled_mock(0.0);
}
