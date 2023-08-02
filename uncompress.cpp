
#include "HCTree.hpp"

using namespace std;

int main(int argc, char* argv[]) { 
    if (argc != 3) { // We expect 3 arguments: the program name, original file and the destination path
        std::cerr << "Usage: " << argv[0] << "<compressed_file> <uncompressed_file>" << std::endl;
        return 1;
    }

    FancyInputStream infile(argv[1]);
    FancyOutputStream outfile(argv[2]);

    if (infile.filesize()==0){
        return 0;
    }

    vector<int> freqs(256, 0); //Empty array of size 256
    int count = 0; //Count of characters
    int charcount = infile.read_byte();
    for(int i = 0; i < charcount; i++) {
        int symbol=infile.read_byte();
        freqs[symbol] = infile.read_int();
        count+=freqs[symbol];
    }
    //Build Huffman code tree

    HCTree tree; //Huffman coding tree
    tree.build(freqs); 

    //Decode count number of times, send to output file
    while(count>0) {
        outfile.write_byte(tree.decode(infile));
        count--;
    }
    return 0; 
}
