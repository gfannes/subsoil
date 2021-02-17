$fn = 200;
$fs = 0.1;

dax = 50;
day = 17;
daz = 5;

dbx = 20;
dby = day;
dbz = 20;

ddd = 4;
ddr = ddd/2;

module screw_c(){
    a = 2;
    b = 4.5;
    cylinder(daz+$fs, a, a);
    z = b-a;
    translate([0,0,daz-z])
    cylinder(z+$fs, a, b);
}

difference(){
    union(){
        cube([dax, day, daz]);
        translate([(dax-dbx)/2,0,daz])
        cube([dbx, dby, dbz]);
    }

    translate([(dax-dbx)/4,day/2,-$fs/2])     screw_c();
    translate([dax-(dax-dbx)/4,day/2,-$fs/2]) screw_c();

    translate([dax/2,day/2,-$fs/2])
    cylinder(daz+dbz+$fs, ddr, ddr);
}
