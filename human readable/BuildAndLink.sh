echo "BUILDING"
g++ cleanedOut.cpp -O3 -o genOut.out -fopenmp
./genOut.out > table.txt
./Link.sh