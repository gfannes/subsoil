module slider(w,d,l, h1)
{
    translate([-l,0,0])
    difference()
    {
        union()
        {
            rotate(90,[0,1,0])
            rotate(90,[0,0,1])
            translate([-d/2,0,0])
            linear_extrude(w+l)
            polygon([[0,0],[d,0],[d/2,d/2]]);

            r = 0.35*d/2;
            translate([l+w/2,0,d/2-r])
            sphere(r);
        }

        translate([-$fs,-d/2-$fs,-$fs])
        cube([l,d+2*$fs,h1]);
    }
}

module block(w,d,h0, h1,s, t,cl)
{
    translate([0,0,h0])
    difference()
    {
        d_o = d+2*t;
        translate([0,-d_o/2,-h0])
        cube([w,d_o,h0+0.9*d/2]);

        translate([-$fs,0,0])
        slider(w+2*$fs,d+cl,0, 0);

        translate([-$fs,-d_o/2-$fs,0])
        cube([s,d_o+2*$fs,h1]);

        r = cl;
        for (f=[-1,1])
        {
            translate([-$fs,f*(d/2-0.75*r),r/2])
            rotate(90, [0,1,0])
            cylinder(w+2*$fs,r,r);
        }
    }
}

$fn = 100;
$fs = 0.01;

block(8,8,3, 2,2, 2,0.3);

/* translate([15,0,0]) */
translate([0,0,3])
slider(8,8,3, 1.5);
