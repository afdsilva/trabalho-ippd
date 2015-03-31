LIB= -I./include/
FLAG = -std=c++11
OBJ = App.o Graph.o Quadtree.o DelaunayTBB.o DelaunayThread.o Entidade.o Eventos.o FPS.o Resources.o StateManager.o State.o StateGUI.o StateTBB.o StateThread.o
LD = -L./lib/ -lsfml-graphics -lpthread -lsfml-window -lsfml-system -lsfml-audio -ltbb
EXECUTABLE = trabalho_ippd

gui: $(OBJ)
	g++ -Wl,-rpath=$(CURDIR)/lib -o $(EXECUTABLE) $(OBJ) $(LD)

App.o: src/App.cpp src/App.h
	g++ -c $(LIB) $(FLAG) src/App.cpp
Graph.o: src/Graph.cpp src/Graph.h
	g++ -c $(LIB) $(FLAG) src/Graph.cpp
Quadtree.o: src/Quadtree.cpp src/Quadtree.h
	g++ -c $(LIB) $(FLAG) src/Quadtree.cpp
DelaunayTBB.o: src/delaunay_tbb/DelaunayTBB.cpp src/delaunay_tbb/DelaunayTBB.h
	g++ -c $(LIB) $(FLAG) src/delaunay_tbb/DelaunayTBB.cpp -o DelaunayTBB.o
DelaunayThread.o: src/delaunay_threads/DelaunayThread.cpp src/delaunay_threads/DelaunayThread.h
	g++ -c $(LIB) $(FLAG) src/delaunay_threads/DelaunayThread.cpp -o DelaunayThread.o

Entidade.o: src/gui/Entidade.cpp src/gui/Entidade.h
	g++ -c $(LIB) $(FLAG) src/gui/Entidade.cpp
Eventos.o: src/gui/Eventos.cpp src/gui/Eventos.h
	g++ -c $(LIB) $(FLAG) src/gui/Eventos.cpp
FPS.o: src/gui/FPS.cpp src/gui/FPS.h
	g++ -c $(LIB) $(FLAG) src/gui/FPS.cpp
Resources.o: src/gui/Resources.cpp src/gui/Resources.h
	g++ -c $(LIB) $(FLAG) src/gui/Resources.cpp
StateManager.o: src/gui/StateManager.cpp src/gui/StateManager.h
	g++ -c $(LIB) $(FLAG) src/gui/StateManager.cpp

State.o: src/states/State.cpp src/states/State.h
	g++ -c $(LIB) $(FLAG) src/states/State.cpp
StateGUI.o: src/states/StateGUI.cpp src/states/StateGUI.h
	g++ -c $(LIB) $(FLAG) src/states/StateGUI.cpp
StateTBB.o: src/states/StateTBB.cpp src/states/StateTBB.h
	g++ -c $(LIB) $(FLAG) src/states/StateTBB.cpp
StateThread.o: src/states/StateThread.cpp src/states/StateThread.h
	g++ -c $(LIB) $(FLAG) src/states/StateThread.cpp



Dependecias:
	sudo apt-get install libglew1.5 libjpeg62 libtbb-dev
	sudo ln -s /usr/lib/x86_64-linux-gnu/libGLEW.so.1.8 /usr/lib/x86_64-linux-gnu/libGLEW.so.1.5
clean:
	rm *.o
	rm $(EXECUTABLE)
