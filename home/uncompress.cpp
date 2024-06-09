#include "HCTree.hpp"
#include "Helper.hpp"
using namespace std;

int main(int argc, char* argv[])
{
  HCTree* huffman_coding_tree_ptr = new HCTree();
  vector<int> frequency_of_char_arr;
  unsigned int uncompress_total_size;

  FancyInputStream input_stream(argv[1]);
  FancyOutputStream output_stream(argv[2]);

  if (input_stream.filesize() <= 0){
    delete huffman_coding_tree_ptr;
    return 0;
  }

  //Read frequencies to construct Huffman tree
  for (unsigned int i = 0; i < 256; i++){
    frequency_of_char_arr.push_back(input_stream.read_int());
  }

  uncompress_total_size = 0;
  for (unsigned int i = 0; i < 256; i++){
    uncompress_total_size += frequency_of_char_arr[i];
  } 

  huffman_coding_tree_ptr->build(frequency_of_char_arr);
  for(unsigned int i = 0; i < uncompress_total_size; i++){
      output_stream.write_byte(huffman_coding_tree_ptr->decode(input_stream));
  }

  delete huffman_coding_tree_ptr;
  return 0;  
}
