$fs = 0.1;
$fn = 100;
eps = $fs/2;

height = 30;

width = 10;
length = 40;
thickness = 2;
hole_diam = 1;


h = height;
a = length/3;
b = a/4;
s = b/5;

translate([0,-width/2,0])
cube([length, width, thickness]);

translate([0,thickness/2,0])
rotate([90,0,0])
    difference(){
        linear_extrude(height = thickness)
        polygon([[a,0],[2*a,0],[a-s+b,h+b],[a-s-b,h+b]]);

        translate([a,h,-eps])
        cylinder(2*thickness,hole_diam/2,hole_diam/2);
    }
