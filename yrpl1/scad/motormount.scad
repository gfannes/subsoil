$fs = 0.01;
$fn = 100;

thick = 3;
diam = 45;
board_thick = 5.1;
screw_shaft_diam = 3.2;
screw_head_diam = 5.7;

height = diam/2.35;

module corner()
{
    for (sign = [-1,1])
    {
        translate([0,sign*(board_thick+thick)/2,0])
        translate([board_thick/2,0,0])
        translate([0,thick/2,0])
        rotate(90,[1,0,0])
        linear_extrude(thick)
        {
            polygon([[0,0], [height,0], [0,height]]);
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
        /* for (angle = [0]) */
        {
            rotate(angle, [0,0,1])
            corner();
        }
    }

    for (sign = [-1,1])
    {
        rotate(45, [0,0,1])
        {
            translate([0,sign*19/2,0])
            {
                translate([0,0,-thick/2])
                cylinder(thick+$fs, screw_shaft_diam/2,screw_shaft_diam/2, true);

                cylinder(height, screw_head_diam/2,screw_head_diam/2);
            }

            translate([sign*16/2,0,0])
            {
                translate([0,0,-thick/2])
                cylinder(thick+$fs, screw_shaft_diam/2,screw_shaft_diam/2, true);

                cylinder(height, screw_head_diam/2,screw_head_diam/2);
            }
        }
    }
}
