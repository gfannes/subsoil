$fn = 200;
$fs = 0.01;

difference(){
    union(){
        cylinder(15, 4.5, 4.5);
        translate([0,0,15])
        cylinder(15, 6, 7.5);
    }
    translate([0,12,15-1-2.5])
    rotate(90,[1,0,0])
    cylinder(42, 1, 1);
}
