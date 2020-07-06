$fn = 200;
$fs = 0.01;

difference()
{
    cylinder(20, 25, 25);

    translate([0,0,-$fs/2])
    cylinder(20+$fs, 2, 2);

    translate([0,0,20-5])
    cylinder(5+$fs, 2, 6);
}
