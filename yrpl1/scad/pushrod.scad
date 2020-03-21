$fs = 0.01;
$fn = 100;

length = 50;
thick = 2.1;
height = 2.5;
space = 5;
hole_diam = 1;

difference()
{
    union()
    {
        cube([length-2*space,thick,height], true);
        for (sign_x = [-1,1])
        {
            for (y = [-thick, thick])
            {
                c = space+height/2+height;
                x = sign_x*(length/2-(c-height)/2);
                translate([x,y,0])
                cube([c,thick,height], true);
            }
        }
    }

    for (x = [-length/2, length/2])
    {
        color("green")
        rotate(90, [1,0,0])
        translate([x,0,0])
        cylinder(3*thick+$fs, hole_diam/2,hole_diam/2 ,true);
    }
}
