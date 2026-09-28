echo "Linking"
g++ compressor.cpp -O3 -o compressor.out -fopenmp
./compressor.out <bothead.cpp >bot.cpp
g++ bot.cpp -o BOT.out -O3 2> error.txt