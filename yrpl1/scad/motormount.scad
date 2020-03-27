$fs = 0.01;
$fn = 100;

thick = 2;
diam = 35;
board_thick = 5;
hole_diam = 3.2;
knob_diam = 6;

height = diam/2.5;

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
        {
            rotate(angle, [0,0,1])
            pole();
        }
    }

    for (sign = [-1,1])
    {
        rotate(45, [0,0,1])
        {
            translate([0,sign*19/2,0])
            {
                translate([0,0,-thick/2])
                cylinder(thick+$fs, hole_diam/2,hole_diam/2, true);

                cylinder(height, knob_diam/2,knob_diam/2);
            }

            translate([sign*16/2,0,0])
            {
                translate([0,0,-thick/2])
                cylinder(thick+$fs, hole_diam/2,hole_diam/2, true);

                cylinder(height, knob_diam/2,knob_diam/2);
            }
        }
    }
}
