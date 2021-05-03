gubg_arduino_uno_size = [68.6, 53.3, 1];
gubg_arduino_uno_holes = [
[14,   2.5],
[66.1, 7.6],
[66.1, 35.5],
[15.3, 50.7],
];

module gubg_arduino_uno_mock(c)
{
    color([1,0,0])
    difference()
    {
        cube([gubg_arduino_uno_size[0],gubg_arduino_uno_size[1],1]);
        for (co=gubg_arduino_uno_holes)
            translate([co[0],co[1],-$fs])
            cylinder(t+2*$fs,1.6,1.6);
    }
}

