EXENAME := base

SRCFILES := *.cpp

$(EXENAME): $(SRCFILES)
	g++ -g -o $(EXENAME) main.cpp -lSDL2 -lGL -lGLEW
run: $(EXENAME)
	./$(EXENAME)

libTags:
	ctags -a -R -f libTags /usr/include/SDL2
	ctags -a -R -f libTags /usr/include/GL
tags: libTags $(SRCFILES)
	cp libTags tags
	ctags -a -R .
clean:
	rm -f $(EXENAME) tags libTags
