module mob(d,h,t,cl)
{
    tt = 0.5*t+cl;

    translate([t,0,0])
    difference()
    {
        union()
        {
            translate([t,0,0])
            for (f=[-1,1])
            {
                translate([0,f*d/2,0])
                {
                    cylinder(h,(t+cl)/2,(t+cl)/2);
                    translate([-4*t,-tt/2,0])
                    cube([4*t,tt,h]);
                }
            }
            translate([-3*t,-d/2,0])
            cube([t,d,h]);
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
fix(10,10,2, 0.3);
translate([15,0,0])
mob(10,10,2, 0.0);
