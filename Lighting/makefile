CFILES := $(wildcard *.c)
OUTFILE := ex

UNAME := $(shell uname -s)

ifeq ($(UNAME), Darwin)
  FLAGS += -framework GLUT -framework OpenGL -finline-functions -funroll-loops -O3

else ifeq ($(UNAME), Linux)
  FLAGS += -lglut -lGLU -lGL -lm -Ofast
else
ERROR = "\n***** Undetected OS -- Linking FAILED *****"
endif

all:
	@echo $(ERROR)
	gcc -Wall $(CFILES) -o $(OUTFILE) $(FLAGS)
	@echo ""

clean:
	rm $(OUTFILE)