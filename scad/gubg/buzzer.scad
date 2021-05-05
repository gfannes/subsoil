use <fixer.scad>

module gubg_buzzer_mock(cl)
{
    r = (11+cl)/2;
    cylinder(8.5,r,r);
    translate([0,0,-5])
    cylinder(5+$fs,1.5,1.5);
}

module gubg_buzzer_mount(t,cl)
{
    r = (11+t)/2;
    difference()
    {
        cylinder(8.5+t,r,r);

        translate([0,0,t+$fs])
        gubg_buzzer_mock(cl);

        translate([-t/2,-r-$fs,t])
        cube([t,2*r+2*$fs,8.5+$fs]);
    }

    translate([r,0,0])
    gubg_fixer_track(20,8.5+5,t,cl);
}

module gubg_buzzer_slider(t,cl)
{
    gubg_fixer_slider(20,5,t,cl);
}

if (false)
{
    $fn = 100;
    $fs = 0.01;

    t = 2;

    gubg_buzzer_mount(t, 0.3);

    color([1,0,0])
    translate([0,0,t])
    gubg_buzzer_mock(0);

    translate([(11+t)/2+0.3,0,0])
    gubg_buzzer_slider(t,0.0);
}
