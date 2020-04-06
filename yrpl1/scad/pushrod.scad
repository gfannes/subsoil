$fs = 0.01;
$fn = 100;


thick = 3;
height = 3.5;
space = 5;
hole_diam = 1.5;

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

module pushrod(length, angle1, angle2)
{
    color("blue")
    cube([length-thick-2*space,thick,height], true);

    for (rot = [0,180])
    {
        rotate(rot,[0,0,1])
        translate([length/2-space, 0, 0])
        rotate(angle1,[0,0,1])
        rotate(angle2/2, [1,0,0])
        claw();
    }
}

//Left wing
if (false)
{
    translate([0,00,0])
    pushrod(122, 13, 0);
}

//Right wing
if (false)
{
    translate([0,20,0])
    pushrod(130, 13, 90);
}

//Rudder
/* if (false) */
{
    translate([0,40,0])
    pushrod(140.5, 0, 90);
}

//Elevator
/* if (false) */
{
    translate([0,60,0])
    pushrod(103, 0, 0);
}
