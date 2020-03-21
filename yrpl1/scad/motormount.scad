$fs = 0.01;
$fn = 100;

thick = 2;
diam = 35;
board_thick = 5;
hole_diam = 2;
//TODO: check if this is correct: distance between mounting screws
offset = 12;

module pole()
{
    for (sign = [-1,1])
    {
        translate([0,sign*board_thick/2,0])
        translate([board_thick/2,0,0])
        translate([0,thick/2,0])
        rotate(90,[1,0,0])
        linear_extrude(thick)
        {
            a = diam/2.5;
            polygon([[0,0], [a,0], [0,a]]);
        }
    }
}

difference()
{
    union()
    {
        translate([0,0,-thick/2])
        cylinder(thick, diam/2,diam/2,true);
        for (angle = [0,90,180,270])
        {
            rotate(angle, [0,0,1])
            pole();
        }
    }

    for (angle = [0,90,180,270])
    {
        rotate(angle, [0,0,1])
        translate([offset/2,offset/2,-thick/2])
        cylinder(thick+$fs, hole_diam/2,hole_diam/2, true);
    }
}
