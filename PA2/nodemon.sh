nodemon --exec "g++ -std=c++11 -pedantic -c formula.cpp && g++ -std=c++11 -pedantic -c mmpa.cpp && g++ -std=c++11 -pedantic -c main.cpp && g++ formula.o mmpa.o main.o -o main && ./main" -e "h cpp"