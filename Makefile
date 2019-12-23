CXX = g++
CXXFLAGS = -std=c++14 -Wall -Werror=vla -MMD -g
EXEC = watan
OBJECTS = main.o controller.o board.o subject.o tile.o goal.o observer.o criteria.o noCriteria.o assignment.o midterm.o exam.o player.o fairdice.o loadeddice.o textdisplay.o resourceArray.o 
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
