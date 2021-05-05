module gubg_slider(w,d,t)
{
    translate([0,-d/2,0])
    union()
    {
        cube([w,d,t]);
        translate([w-0.5*$fs,0,0])
        cube([t,d,2*t]);
    }
}

module gubg_block(w,d,h,t,clearance)
{
    dd = d+2*t;
    hh = h+2*t;
    difference()
    {
        translate([0,-dd/2,0])
        cube([w,dd,hh]);

        translate([-$fs-t-clearance,0,h])
        gubg_slider(w+2*$fs,d+clearance,t+clearance);
    }
}

module gubg_stop(w,d,h,t)
{
    intersection()
    {
        union()
        {
            translate([-w,-d/2,0])
            cube([w,d,h+t/sqrt(2)]);

            translate([0,0,h+t/sqrt(2)])
            rotate(45,[0,1,0])
            cube([t,d,t],true);
        }

        translate([-w,-d/2,0])
        cube([w+t/sqrt(2),d,h+t/sqrt(2)]);
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
