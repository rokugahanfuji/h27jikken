PROGS = sabun frame color

amplify: amplify.c image_rw.c params.h
	gcc -O2 -o amplify amplify.c image_rw.c

sabun : sabun.c params.h
	gcc -O2 -o sabun sabun.c image_rw.c

frame : frame.c params.h
	gcc -O2 -o frame frame.c image_rw.c

color : color.c ppmlib.h
	gcc -O2 -o color color.c image_rw.c