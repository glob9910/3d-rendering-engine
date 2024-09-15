mkdir -p bin && \
g++ src/glad.c src/main.cpp -o bin/main -I./include -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl && \
./bin/main
