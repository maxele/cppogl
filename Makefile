EXENAME := base

FREETYPE := -lfreetype
LIBS := -lSDL2 -lGL -lGLEW $(FREETYPE)
TAGGED_LIBS := /usr/include/SDL2 /usr/include/GL /usr/include/freetype2
SRCFILES := *.cpp

$(EXENAME): $(SRCFILES)
	g++ -g -o $(EXENAME) main.cpp $(LIBS)
$(EXENAME)-debug: $(SRCFILES)
	g++ -g -o $(EXENAME)-debug main.cpp $(LIBS) -g

.PHONY: run tags clean
run: $(EXENAME)
	./$(EXENAME)
libTags: $(TAGGED_LIBS)
	ctags -a -R -f libTags $(TAGGED_LIBS)
tags: libTags $(SRCFILES)
	cp libTags tags
	ctags -a -R .
clean:
	rm -f $(EXENAME) tags libTags
