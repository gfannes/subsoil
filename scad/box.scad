module part(x,y,z,t,cover,c)
{
    color(c)
    translate([0,0,(z-t)*cover])
    cube([x,y,t]);

    color(c/2)
    translate([0,0,t])
    linear_extrude(z-2*t)
    polygon([[cover*x,cover*y], [x,0], [x-t,t], [t+(x-2*t)*cover, t+(y-2*t)*cover], [t,y-t], [0,y]]);
}

t = 2;

part(50,30,10,t, 0, [1,0,0]);

part(50,30,10,t, 1, [0,1,0]);
