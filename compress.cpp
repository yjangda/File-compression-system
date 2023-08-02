#include "HCTree.hpp"
#include "Helper.hpp"
#include <iostream>
#include <vector>
/*void writeHeader(const std::vector<int>& freqs, FancyOutputStream& out) {
    for (int i = 0; i < (int)freqs.size(); ++i) {
        if (freqs[i]>0){
            out.write_byte(freqs[1]);
        }
        else{
            out.write_byte(freqs[0]);
        }
        for (int i = 0; i < (int) freqs.size(); i++) {
            if (freqs[i] > 0) {
                out.write_int(freqs[i]);
            }
        }
    }
}
void compress(const char* infile, const char* outfile) {
    // create input and output streams
    FancyInputStream fin(infile);
    FancyOutputStream fout(outfile);


    // count the frequency of each byte in the input file
    vector<int> freqs(256, 0);
    int filesize = fin.filesize();
    //empty file
    if (filesize == 0){
        return;
    }
    if (!fin.good()) {
        error("Err: Input has an error");
        return;
    }


    for(int i = 0; i < filesize; i++) {
        freqs[fin.read_byte()]++;
    }

    // build the Huffman code tree using the frequency table
    HCTree hctree;
    hctree.build(freqs);
    writeHeader(freqs,fout);


    // reset the input stream to read from the beginning of the file
    fin.reset();

    // encode the input file using the Huffman code tree
    for(int i = 0; i < filesize; i++) {
        unsigned char symbol = fin.read_byte();
        hctree.encode(symbol, fout);
    }

    // flush any remaining bits in the output stream
    fout.flush();
} */

int main(int argc, char* argv[]) {
    // check that there are enough arguments
    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <original_file> <compressed_file>" << std::endl;
        return 1;
    }

    // get input and output file names from command line arguments
    FancyInputStream infile(argv[1]);
    FancyOutputStream outfile(argv[2]);
    vector<int> freqs(256, 0);
    int read;
    int filesize = infile.filesize();
    //empty file
    if (filesize == 0){
        return 0;
    }
    if (!infile.good()) {
        error("Err: Input has an error");
        return 1;
    }
    int uniqCount = 0;


    for(int i = 0; i < filesize; i++) {
        read = infile.read_byte();
        if (!freqs[read]) {
            uniqCount++;
        }
        freqs[read]++;
    }

    // build the Huffman code tree using the frequency table
    HCTree hctree;
    hctree.build(freqs);

    outfile.write_byte(uniqCount);
    for (int i = 0; i < 256; ++i) {
        if (freqs[i]>0){
            outfile.write_byte(i);
            outfile.write_int(freqs[i]);
        }
    }


    // reset the input stream to read from the beginning of the file
    infile.reset();

    // encode the input file using the Huffman code tree
    for(int i = 0; i < filesize; i++) {
        unsigned char symbol = infile.read_byte();
        hctree.encode(symbol, outfile);
    }

    
}
