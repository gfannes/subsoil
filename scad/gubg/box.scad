use <key.scad>

module gubg_box(x,y,z,t,bt,cl)
{
    ro = 1.5*t;
    ri = t;

    function coord2(x,y) = [bt*x,y];
    function coord3(x,y,z) = [bt*x,y,z];

    intersection()
    {
        union()
        {
            r = 0.3*t;
            d = 1.75*t;
            small_lock_h = 3*t;

            //Base plate
            linear_extrude(t)
            polygon([coord2(0,0), coord2(x,0), coord2(x,y), coord2(0,y)]);

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
                    gubg_key(z-2*t,ri-cl,sqrt(2)*2*t);

                    //Small lock
                    translate(coord3(ro,ro,z-small_lock_h-cl-t))
                    cylinder(small_lock_h+cl,ro,ro);

                    //Small key
                    translate(coord3(x-ro,y-ro,0))
                    {
                        small_key_angle = 90+bt*90;
                        rotate(small_key_angle,[0,0,1])
                        gubg_key(small_lock_h,ri-cl,small_lock_h);

                        if (false)
                            cylinder(5,ro,ro);
                    }
                }

                translate(coord3(3*t,y-t/2-cl,t+z/2))
                cube([t,t+cl,z],true);

                lock_angle = -45*bt+90;
                translate(coord3(ro,y-ro,t))
                rotate(lock_angle, [0,0,1])
                gubg_key(z-2*t+$fs,ri-0.0,sqrt(2)*2*t);

                //Small key removal
                translate(coord3(ro,ro,z-small_lock_h-cl-t-$fs))
                rotate(90-bt*90,[0,0,1])
                gubg_key(small_lock_h+cl+2*$fs,ri-cl,small_lock_h);
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
