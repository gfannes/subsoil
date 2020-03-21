$fs = 0.01;
$fn = 100;

thick = 2;
board_thick = 5;
a = 15;
b = 5;
c = 40;

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
}
