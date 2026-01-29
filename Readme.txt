To compile this code you can run:
make
or 
gcc RojasFilters.c PixelProcessor.c BmpProcessor.c -o RojasFilters.out

To run the compiled program:

./RojasFilters.out -i test2-1.bmp -o out.bmp -f b 
./RojasFilters.out -i test2-1.bmp -o out.bmp -f c