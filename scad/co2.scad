$fn = 100;
$fs = 0.01;

use <gubg/box.scad>

t = 2;

gubg_box(50,30,10,t, 1, [1,0,0]);

gubg_box(50,30,10,t, -1, [0,1,0]);
