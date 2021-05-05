module mob(d,h,t,cl)
{
    tt = 0.5*t+cl;

    translate([t,0,0])
    difference()
    {
        translate([t,0,0])
        union()
        {
            for (f=[-1,1])
            {
                translate([0,f*d/2,0])
                cylinder(h,(t+cl)/2,(t+cl)/2);
            }
            translate([-tt/2,-d/2,0])
            cube([tt,d,h]);

            translate([-4*t,-t/2,0])
            cube([4*t,t,h]);
        }

        dd = d+2*t+2*$fs;
        translate([-3*t-$fs,-dd/2,-$fs])
        cube([2*t,dd,t]);
    }
}

module fix(d,h,t, cl)
{
    translate([cl,0,0])
    difference()
    {
        translate([2*t,0,0])
        for (f=[-1,1])
        {
            translate([0,f*d/2,0])
            cylinder(h,t,t);
        }

        translate([0,0,-$fs])
        mob(d,h+2*$fs,t,cl);
    }
}

$fn = 100;
$fs = 0.01;

d = 10;
h = 10;
t = 2;
cl = 0.3;

fix(d,h,t,cl);
    /* translate([15,0,0]) */
translate([cl,0,0])
    mob(d,h,t, 0);
