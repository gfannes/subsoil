module gubg_fixer_slider(d,h,t,hh,cnt, cl)
{
    tt = 0.5*t-cl;

    //Upside-down for better printing
    translate([0,0,h])
    rotate(180, [1,0,0])

    translate([t,0,0])
    difference()
    {
        translate([t,0,0])
        union()
        {
            for (f=[-1,1])
            {
                translate([0,f*d/2,0])
                cylinder(h,(t-cl)/2,(t-cl)/2);
            }
            translate([-tt/2,-d/2,0])
            cube([tt,d,h]);

            dd = d/cnt;
            for (i=[0:cnt-1])
            {
                translate([-4*t,(-d+dd)/2+i*dd-t/2,0])
                cube([4*t,t,h]);
            }
        }

        hhh = hh-cl;
        if (hhh > 0)
        {
            dd = d+2*t+2*$fs;
            translate([-3*t-$fs,-dd/2,-$fs])
            cube([2*t,dd,hhh]);
        }
    }
}

module gubg_fixer_track(d,h,screw_h,t, cl)
{
    translate([cl,0,0])
    difference()
    {
        translate([2*t,0,0])
        {
            for (f=[-1,1])
            {
                translate([0,f*d/2,0])
                cylinder(h+screw_h,t,t);
            }
            translate([t/2,-d/2,0])
            cube([t/2,d,h]);
        }

        translate([0,0,-$fs])
        gubg_fixer_slider(d,h+screw_h+2*$fs,t,0,1,-cl);
    }
}

$fn = 100;
$fs = 0.01;

d = 10;
h = 10;
t = 2;

cls = [0.3, 0.4];

for (clix=[0:1])
{
    cl = cls[clix];

    translate([0,clix*-20,0])
    {
        gubg_fixer_track(d,h,4,t,cl);
        translate([15,0,0])
        translate([cl,0,0])
        gubg_fixer_slider(d,h,t,0,2, cl);
    }
}
