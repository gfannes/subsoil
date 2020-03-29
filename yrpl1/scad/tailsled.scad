$fs = 0.01;
$fn = 100;

thick = 3;
board_thick = 5.1;
a = 15;
b = 8;
c = 40;

wire_diam = 2.4;

difference()
{
    union()
    {
        translate([0,0,-b])
        rotate(90, [1,0,0])
        translate([0,0,-thick-board_thick/2])
        linear_extrude(2*thick+board_thick)
        {
            polygon([[0,a+b], [c,a+b], [0.75*c,0], [c/2,0]]);
        }
    }

    translate([0,-board_thick/2,0])
    cube([c,board_thick,a+$fs]);

    color("green")
    translate([0.68*c,0,-b-1])
    rotate(-30,[0,1,0])
    cylinder(b+3, wire_diam/2,wire_diam/2);
}
