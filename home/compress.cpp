#include "HCTree.hpp"
#include "Helper.hpp"
using namespace std;

int main(int argc, char* argv[])
{
  HCTree* huffman_coding_tree_ptr = new HCTree();

  vector<int> frequency_of_char_arr(256, 0);
  int ascii_val_of_char;

  FancyInputStream input_stream(argv[1]);
  //cout << "Filesize of input: " << input_stream.filesize() << endl;
  FancyOutputStream output_stream(argv[2]);

  if (input_stream.filesize() <= 0){
    delete huffman_coding_tree_ptr;
    return 0;
  }

  //cout << "compress: checked for empty file" << endl;

  //Traverse through the entire file to get frequency of all characters
  while ((ascii_val_of_char = input_stream.read_byte()) != -1){
    frequency_of_char_arr[ascii_val_of_char]++;
  }

  //cout << "compress: finished reading frequencies of chars..." << endl;
  
  //Write header of compressed file so it can be used to uncompress
  for (unsigned int i = 0; i < frequency_of_char_arr.size(); i++){
    output_stream.write_int(frequency_of_char_arr[i]);
  }
  
  huffman_coding_tree_ptr->build(frequency_of_char_arr);

  input_stream.reset();
  //Encoding each byte and add to output
  while((ascii_val_of_char = input_stream.read_byte()) != -1){
      huffman_coding_tree_ptr -> encode(ascii_val_of_char, output_stream);
  }
  //cout << "compress: Successful encode" << endl;
  
  delete huffman_coding_tree_ptr;

  return 0;
}
