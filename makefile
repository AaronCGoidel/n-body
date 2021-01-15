# Graphics Framework
GFFLAGS = -framework GLUT -framework OpenGL

# Supress GLUT warnings
SFLAGS = -Wno-deprecated

all: simulation

simulation: simulation.c bh_tree.c particle.c util.c graphics.c
	clang -fopenmp -L/usr/local/opt/llvm/lib -o sim simulation.c bh_tree.c particle.c util.c graphics.c ${SFLAGS} ${GFFLAGS} 

prof: simulation.c bh_tree.c particle.c util.c graphics.c
	gcc -pg -o simulation simulation.c bh_tree.c particle.c util.c graphics.c -lGLU -lGL -lglut -lm

clean:
	rm -f sim