gubg_grove_scd30_holes = [[0,-10], [30,-20],[60,-10],[50,20],[10,20]];

module gubg_grove_scd30(clearance)
{
    module cuby(b,d,h)
    {
        translate([0,-d/2,0])
        cube([b,d,h]);
    }

    translate([0,0,-1.5-clearance])
    difference()
    {
        union()
        {
            cuby(60+clearance,40+clearance,1.5+clearance);
            for (co=gubg_grove_scd30_holes)
            {
                translate(co)
                cylinder(1.5+clearance,2,2);
            }
        }
        for (co=[[10,-20], [50,-20],[60,10],[30,20],[0,10]])
        {
            translate(concat(co,[-$fs]))
            cylinder(1.5+clearance+2*$fs,2.25,2.25);
        }
    }
    translate([3-clearance/2,0,0])
    union()
    {
        cuby(41.5+clearance,27+clearance,10+clearance);
        cylinder(10+clearance,3,3);
        translate([41.5+clearance,0,0])
        cylinder(10+clearance,3,3);
    }
}

gubg_grove_oled_holes = [[0,0], [30,-10],[30,10]];

module gubg_grove_oled(clearance)
{
    module cuby(b,d,h)
    {
        translate([0,-d/2,0])
        cube([b,d,h]);
    }

    difference()
    {
        union()
        {
            cuby(40+clearance,20+clearance,1.5+clearance);
            for (co=gubg_grove_oled_holes)
            {
                translate(co)
                cylinder(1.5+clearance,2.25,2.25);
            }
        }
        for (co=[[10,-10],[40,0],[10,10]])
        {
            translate(concat(co,[-$fs]))
            cylinder(1.5+clearance+2*$fs,2,2);
        }
    }
    translate([3-clearance/2,0,-2-clearance])
    cuby(26.5+clearance,20+clearance,2+clearance);
}

if (false)
{
    $fn = 100;

    translate([50,0,0])
    gubg_grove_scd30(0);
    gubg_grove_oled(0);
}
