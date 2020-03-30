$fs = 0.01;
$fn = 100;

length = 122;

angle = 13;

thick = 3;
height = 3.5;
space = 5;
hole_diam = 1;

module claw()
{
    translate([-thick/2,0,0])
    cube([thick, 3*thick, height], true);

    for (sign = [-1,1])
    {
        difference()
        {
            x = space+thick;
            y = sign*thick;
            color("green")
            translate([x/2,y,0])
            cube([x,thick,height], true);

            color("red")
            translate([space,y,0])
            rotate(90, [1,0,0])
            cylinder(thick+$fs, hole_diam/2,hole_diam/2, true);
        }
    }
}

color("blue")
cube([length-thick-2*space,thick,height], true);

for (rot = [0,180])
{
    rotate(rot,[0,0,1])
    translate([length/2-space, 0, 0])
    rotate(angle,[0,0,1])
    claw();
}
