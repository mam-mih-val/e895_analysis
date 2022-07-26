//
// Created by mikhail on 6/16/22.
//

#include <iostream>
#include <fstream>
#include "input_chain.h"

InputChain::InputChain(const std::string &input_file_list)
    : chain_(new TChain("T")) {
  this->AddFiles(input_file_list);
  this->InitFields();
}

void InputChain::AddFiles(const std::string& file_list) {
  std::cout << "Reading file from filelist " << file_list << std::endl;
  std::ifstream infile(file_list);
  std::string input_root_file;
  if( !infile.is_open() ){
    throw std::runtime_error( "Filelist "+file_list+" cannot be open" );
  }
  int i=0;
  while( infile >> input_root_file ){
    auto success = chain_->AddFile( input_root_file.c_str() );
    if( success != 1 )
      throw std::runtime_error( "File "+ input_root_file +" cannot be open for reading" );
    std::cout << i << ". " << input_root_file << " added\n";
    i++;
  }
}

bool InputChain::NextEvent() {
  assert(chain_);
  current_event_++;
  if(current_event_ >= chain_->GetEntries())
    return false;
  chain_->GetEntry(current_event_);
  return true;
}
void InputChain::InitFields() {
  assert(chain_);
  chain_->SetBranchAddress("ievnt", &ievnt_);
  chain_->SetBranchAddress("ntrks_g", &ntrks_g_);
  chain_->SetBranchAddress("mfilt_g", &mfilt_g_);
  chain_->SetBranchAddress("nv0_g", &nv0_g_);
  chain_->SetBranchAddress("qphi_g", &qphi_g_);
  chain_->SetBranchAddress("dqphi_g", &dqphi_g_);
  chain_->SetBranchAddress("phi12_g", &phi12_g_);
  chain_->SetBranchAddress("qxcorr_g", &qxcorr_g_);
  chain_->SetBranchAddress("qycorr_g", &qycorr_g_);
  chain_->SetBranchAddress("n", &n_);
  chain_->SetBranchAddress("itrk", itrk_);
  chain_->SetBranchAddress("a1", a1_);
  chain_->SetBranchAddress("a2", a2_);
  chain_->SetBranchAddress("a3", a3_);
  chain_->SetBranchAddress("z1", z1_);
  chain_->SetBranchAddress("z2", z2_);
  chain_->SetBranchAddress("z3", z3_);
  chain_->SetBranchAddress("p1", p1_);
  chain_->SetBranchAddress("p2", p2_);
  chain_->SetBranchAddress("p3", p3_);
  chain_->SetBranchAddress("dedx", dedx_);
  chain_->SetBranchAddress("diffphi", diffphi_);
  chain_->SetBranchAddress("pzcm", pzcm_);
  chain_->SetBranchAddress("qphi", qphi_);
  chain_->SetBranchAddress("theta", theta_);
  chain_->SetBranchAddress("weight", weight_);
  chain_->SetBranchAddress("ylab", ylab_);
  chain_->SetBranchAddress("ycm", ycm_);
  chain_->SetBranchAddress("mass", mass_);
  chain_->SetBranchAddress("nnet", nnet_);
}
