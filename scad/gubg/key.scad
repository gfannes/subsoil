module gubg_key(h,ri,ri2,l)
{
    cylinder(h-ri,ri,ri);
    translate([0,0,h-ri-$fs])
    cylinder(ri+$fs,ri,ri2);
    translate([0,-ri/2,0])
    cube([l,ri,h]);
}

module gubg_lock(h,ro,ri)
{
    difference()
    {
        cylinder(h,ro,ro);

        translate([0,0,-$fs])
        gubg_key(h+2*$fs,ri,ri,ro+$fs);
    }
}

$fn = 100;
$fs = 0.01;

color([0,1,0])
    gubg_lock(10, 6, 3+0.4);

color([1,0,0])
    gubg_key(10, 3,2, 10);
