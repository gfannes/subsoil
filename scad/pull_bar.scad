$fn = 200;
$fs = 0.01;
eps2 = $fs;
eps = $fs/2;

r1 = 27/2;
r2 = 49/2;

w = 55;

l1 = 10;
l2 = r2+6;

h1 = 15;
h2 = h1+15;

s = 3/2;

difference(){
    translate([-l1,-w/2,0])
    cube([l1+l2,w,h2]);
    translate([-l1-eps,-(w+eps)/2,h1])
    cube([l1+eps2,w+eps2,h2]);

    translate([l2,0,h2/2-eps])
    cylinder(h2/2+eps2, r1,r1);

    translate([l2,0,-eps])
    cylinder(h2/2+eps2, r2,r2);

    translate([0,0,h2])
    rotate(90, [1,0,0])
    translate([0,0,-w/2-eps])
    cylinder(w+eps2, h2-h1,h2-h1);

    for (sign=[1,0,-1])
        union(){
            translate([-s,sign*20,-eps])
            cylinder(h1+eps2, s,s);
            translate([-s,sign*20,h1-1.5*s+eps])
            cylinder(2*s, s,3*s);
        }
}
