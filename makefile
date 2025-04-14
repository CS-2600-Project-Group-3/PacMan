# Mac users: in terminal use "brew install csfml". After that "make pacmanMac" should work
pacmanMac:
	gcc pacman.c -I/usr/local/Cellar/csfml/2.6.1_1/include -L/usr/local/Cellar/csfml/2.6.1_1/lib -lcsfml-graphics -lcsfml-window -lcsfml-system -o pacman

testMac:
	gcc test.c -I/usr/local/Cellar/csfml/2.6.1_1/include -L/usr/local/Cellar/csfml/2.6.1_1/lib -lcsfml-graphics -lcsfml-window -lcsfml-system -o test

# Windows users