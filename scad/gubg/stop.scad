module gubg_slider(b,d,t)
{
    translate([0,-d/2,0])
    union()
    {
        cube([b,d,t]);
        translate([b-0.5*$fs,0,0])
        cube([t,d,2*t]);
    }
}

module gubg_block(b,d,h,t,clearance)
{
    dd = d+2*t;
    hh = h+2*t;
    difference()
    {
        translate([0,-dd/2,0])
        cube([b,dd,hh]);

        translate([-$fs-t-clearance,0,h])
        gubg_slider(b+2*$fs,d+clearance,t+clearance);
    }
}

module gubg_stop(b,d,h,t)
{
    intersection()
    {
        union()
        {
            translate([-b,-d/2,0])
            cube([b,d,h+t/sqrt(2)]);

            translate([0,0,h+t/sqrt(2)])
            rotate(45,[0,1,0])
            cube([t,d,t],true);
        }

        translate([-b,-d/2,0])
        cube([b+t/sqrt(2),d,h+t/sqrt(2)]);
    }
}

$fs = 0.01;

translate([-20,0,0])
{
    gubg_block(5,5,4,2,0.2);

    color([1,0,0])
    translate([-2,0,4])
    gubg_slider(5,5,2);
}

gubg_stop(5,5,4,2);
