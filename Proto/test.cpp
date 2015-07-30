#include <fcntl.h>
#include <fstream>
#include <string>
#include <stdint.h>
#include <iostream>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/message.h>
#include "DeepLearning.pb.h"

using google::protobuf::io::FileInputStream;
using google::protobuf::io::FileOutputStream;
using google::protobuf::io::ZeroCopyInputStream;
using google::protobuf::io::CodedInputStream;
using google::protobuf::io::ZeroCopyOutputStream;
using google::protobuf::io::CodedOutputStream;
using google::protobuf::Message;

bool ReadProtoFromTextFile(const char* filename, Message* proto) {
  int fd = open(filename, O_RDONLY);
//  CHECK_NE(fd, -1) << "File not found: " << filename;
  FileInputStream* input = new FileInputStream(fd);
  bool success = google::protobuf::TextFormat::Parse(input, proto);
  delete input;
//  close(fd);
  return success;
}

int main(int argc, char *argv[]){

	NeuralNetParameter message; 
	if (argc == 2){

		ReadProtoFromTextFile(argv[1], &message);
	std::cout << message.layerstruct_size();	
	}

	return 0;
}