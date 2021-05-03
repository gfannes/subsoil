use <key.scad>

module gubg_box(x,y,z,t,bt,clearance)
{
    ro = 1.5*t;
    ri = t;

    function coord2(x,y) = [bt*x,y];
    function coord3(x,y,z) = [bt*x,y,z];

    module hook(d,clearance)
    {
        translate([0,-2*t,0])
        rotate(90, [0,0,1])
        translate(coord3(0,(t+clearance)/2,0))
        rotate(90, [1,0,0])
        linear_extrude(t+clearance)
        polygon([[0,-$fs], [3*t+clearance,-$fs], [2*t+clearance,t+2*$fs], [0,t+$fs]]);
    }

    intersection()
    {
        union()
        {
            r = 0.3*t;
            d = 1.75*t;
            difference()
            {
                //Base plate
                linear_extrude(t)
                polygon([coord2(0,0), coord2(x,0), coord2(x,y), coord2(0,y)]);

                //Lever
                yy = y-d-r;
                xx = 0.4*x;
                xxx = 0.7*x;
                hull()
                {
                    translate(coord3(xxx+d/2+r,yy,-$fs))
                    cylinder(t+2*$fs,r,r);
                    translate(coord3(x,yy,-$fs))
                    cylinder(t+2*$fs,r,r);
                }

                translate(coord3(x-(t+clearance)/2+$fs,y-d-clearance,0))
                hook(d,1*clearance);
            }

            difference()
            {
                union()
                {
                    //Wall
                    translate(coord3(0,0,t))
                    linear_extrude(z-2*t)
                    union()
                    {
                        polygon([coord2(0,0), coord2(x-2.5*t,0), coord2(x-2.5*t,t), coord2(t,t), coord2(t,y-ro), coord2(0,y-ro)]);

                        if (false)
                            //Opposite wall
                            polygon([coord2(x,y), coord2(t,y), coord2(t,y-t), coord2(x-t,y-t), coord2(x-t,t), coord2(x,t)]);

                    }

                    //Lock
                    translate(coord3(ro,y-ro,t))
                    cylinder(z-2*t,ro,ro);

                    //Key
                    key_angle =-45*bt-90;
                    translate(coord3(x-ro,ro,t))
                    rotate(key_angle, [0,0,1])
                    gubg_key(z-2*t,ri-clearance,sqrt(2)*2*t);

                    translate(coord3(t/2,d+clearance/2,z+$fs))
                    rotate(180, [0,0,1])
                    rotate(180, [0,1,0])
                    hook(d,0);
                }

                translate(coord3(3*t,y-t/2-clearance,t+z/2))
                cube([t,t+clearance,z],true);

                lock_angle = -45*bt+90;
                translate(coord3(ro,y-ro,t))
                rotate(lock_angle, [0,0,1])
                gubg_key(z-2*t+$fs,ri-0.0,sqrt(2)*2*t);
            }
        }
        /* if (false) */
            hull()
            {
                translate(coord3(ro,ro,0))      cylinder(z+$fs,ro,ro);
                translate(coord3(x-ro,ro,0))   cylinder(z+$fs,ro,ro);
                translate(coord3(ro,y-ro,0))   cylinder(z+$fs,ro,ro);
                translate(coord3(x-ro,y-ro,0)) cylinder(z+$fs,ro,ro);
            }
    }
}

$fn = 100;
$fs = 0.01;
t = 1.5;
b = 50;
d = 30;
h = 40;
//0.2 is very tight on CR10S-Pro
translate([2,0,0])
    gubg_box(b,d,h,t,1,0.2);
//0.3 is very tight on CR10S-Pro
translate([-2,0,0])
    gubg_box(b,d,h,t,-1,0.3);
