$fn = 100;
$fs = 0.01;
d1 = 115;
d2 = 25;
w1 = 4;
w2 = 45;
w3 = 6;

h1 = 20;
h2 = 55;
h3 = 125;

a = 63;

module one(i){
    w = w1+w2;
    ww = w1;
    translate([0,i*w,0]){
        difference()
        {
            union(){
                translate([0,-(w+ww)/2,0])
                cube([w3,w+ww,h2]);

                for (sign = [-1,1]){
                    translate([0,sign*(w2/2+ww/2)-ww/2,0])
                    cube([d1,ww,h1]);
                }

                rr = h1/sin(a);
                translate([(d1-d2),w/2,0])
                rotate(90,[1,0,0])
                linear_extrude(w)
                polygon([[0,0], [d2,0], [d2,h1], [rr*cos(a),h1]]);
            }

            r = w2/2;
            translate([r*cos(a)+(d1-d2),0,r*sin(a)])
            rotate(-a,[0,1,0])
            translate([0,0,$fs])
            cylinder(h3, r,r);
        }
    }
}

n = 4;
for (i = [0:n-1]){
    one(i);
}
