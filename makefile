# **************************************************************************
# L3Informatique						C/Unix
# 			TP Hash_Map

# * Group 	: GROUPE 3.1
# * Name 1 : David RENARD CALZANT
# * Name 2 :	Julien ROYON CHALENDARD
# *
# **************************************************************************#

# Taper make dans le cmd
# Link
#

bin/test: obj/file.o obj/hash.o obj/main.o
	gcc -o bin/test obj/file.o obj/hash.o obj/main.o
	
# Objets of TP Lists
#

obj/hash.o: src/hash.c
	gcc -o obj/hash.o -c src/hash.c 
	
obj/file.o: src/file.c
	gcc -o obj/file.o -c src/file.c
	

obj/main.o: src/main.c
	gcc -o obj/main.o -c src/main.c

#
# Listing creation in pdf
#

listing:
	a2ps -o listing.ps include/functions.h include/types.h src/file.c src/hash.c src/main.c
	ps2pdf listing.ps
	rm listing.ps

#
# Remove files
#

clean :
	rm -rf obj/*.o
	
mrproper : clean
	rm -rf bin/test

#
# Test the functionality 
#

test :
	./bin/test
