$fs = 0.01;
$fn = 100;

thick = 2;
board_thick = 5;
a = 15;
b = 4;
c = 30;

slot = [2.3, 17, 15];
lower = 12;
extra = 4;
block = slot+[extra,extra,extra/2];

screw_diam = 3;

difference()
{
    union()
    {
        translate([0,0,-b])
        rotate(90, [1,0,0])
        translate([0,0,-thick-board_thick/2])
        linear_extrude(2*thick+board_thick)
        {
            polygon([[0,a+b], [c,a+b], [0.8*c,0], [0.6*c,0]]);
        }

        translate([0.7*c,0,-block.z])
        translate([0,0,block.z/2])
        cube(block, true);
    }

    translate([0,-board_thick/2,0])
    cube([c,board_thick,a+$fs]);

    translate([0.7*c,0,-block.z])
    {
        translate([0,0,slot.z/2-$fs])
        cube(slot, true);

        translate([0,0,slot.x/2-$fs])
        cube([slot.x,block.y+$fs,slot.x], true);
        for (sign = [-1,1])
        {
            angle = 32;
            y = 15;
            z = 10;
            d = 10;
            h = 11.5;
            translate([0,sign*y,z])
            rotate(sign*angle,[1,0,0])
            translate([0,0,h/2])
            cube([10,d,h], true);
        }
    }

    translate([0.7*c-(extra+slot.x)/2-$fs/2,0,-extra/2-slot.x-screw_diam/2])
    rotate(90, [0,1,0])
    cylinder(extra+slot.x+$fs,screw_diam/2,screw_diam/2);

}
