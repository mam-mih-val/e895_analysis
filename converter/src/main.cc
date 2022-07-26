//
// Created by mikhail on 6/14/22.
//

#include "converter.h"
#include <boost/program_options.hpp>

int main(int argv, char** argc){
  namespace po = boost::program_options;

  if( argv < 2 ){
    throw std::runtime_error( "Incorrect number of argument. Type ./convert --help to see options" );
  }

  std::string input_list;
  std::string output_file;
  std::string output_tree;
  std::string system;
  double energy; // Sqrt snn
  int run_id; // Sqrt snn

  po::options_description options("Options");
  options.add_options()
      ("help,h", "Help screen")
      ("input,i", po::value<std::string>(&input_list),"Input file list")
      ("output,o", po::value<std::string>(&output_file)->default_value("output.root"),"Name of output file")
      ("output-tree", po::value<std::string>(&output_tree)->default_value("atree"),"Name of output tree")
      ("system,s", po::value<std::string>(&system)->default_value("Au+Au"),"Collision system")
      ("energy,e", po::value<double>(&energy)->default_value(3.3),"Kinetic beam energy in lab system")
      ("run-id,r", po::value<int>(&run_id)->default_value(0),"Collision energy per nucleon pair");
  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(argv, argc).options(options).run();
  po::store(parsed, vm);
  po::notify(vm);
  if (vm.count("help")){
    std::cout << options << std::endl;
    return 0;
  }

  auto converter = Converter( input_list, output_file, output_tree );
  converter.SetCollidingSystem( system, energy );
  converter.SetRunId( run_id );
  converter.Run();
  return 0;
}