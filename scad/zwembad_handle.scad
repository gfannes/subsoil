//uitsteek pin-tot-pin: 176mm

$fs = 0.01;
$fn = 100;

diam = 2.8;

difference()
{
    cube([157,15,15], true);

    color("green")
    rotate(90, [0,1,0])
    cylinder(200,diam/2,diam/2, true);
}
