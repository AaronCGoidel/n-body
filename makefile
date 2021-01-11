# Graphics Framework
GFFLAGS = -framework GLUT -framework OpenGL

# Supress GLUT warnings
SFLAGS = -Wno-deprecated

all: simulation

simulation: simulation.c bh_tree.c particle.c util.c graphics.c
	gcc -o sim simulation.c bh_tree.c particle.c util.c graphics.c ${SFLAGS} ${GFFLAGS}

clean:
	rm -f sim