//
// Created by mikhail on 6/14/22.
//

#include "out_tree_manager.h"

#include <memory>

OutTreeManager::OutTreeManager(const std::string &out_file_name,
                     const std::string &out_tree_name) {
//  using namespace AnalysisTree;
  out_file_.reset(TFile::Open( out_file_name.c_str(), "recreate" ));
  out_tree_ = std::make_unique<TTree>( out_tree_name.c_str(), "Analysis Tree with MC data" );

  event_header_config_ = AnalysisTree::BranchConfig( "event_header", AnalysisTree::DetType::kEventHeader );
  event_header_config_.AddField<int>( "run_id", "" );
  event_header_config_.AddField<int>( "ievnt", "" );
  event_header_config_.AddField<int>( "ntrks_g", "" );
  event_header_config_.AddField<int>( "mfilt_g", "" );
  event_header_config_.AddField<int>( "nv0_g", "" );
  event_header_config_.AddField<float>( "qphi_g", "" );
  event_header_config_.AddField<float>( "dqphi_g", "" );
  event_header_config_.AddField<float>( "phi12_g", "" );
  event_header_config_.AddField<float>( "qxcorr_g", "" );
  event_header_config_.AddField<float>( "qycorr_g", "" );
  event_header_config_.AddField<int>( "n", "" );

  configuration_.AddBranchConfig(event_header_config_);
  event_header_ = new AnalysisTree::EventHeader( event_header_config_.GetId() );
  event_header_->Init( event_header_config_ );

  tracks_config_ = AnalysisTree::BranchConfig( "tracks", AnalysisTree::DetType::kTrack );
  tracks_config_.AddField<int>("itrk", "");
  tracks_config_.AddField<int>("a1", "");
  tracks_config_.AddField<int>("a2", "");
  tracks_config_.AddField<int>("a3", "");
  tracks_config_.AddField<int>("z1", "");
  tracks_config_.AddField<int>("z2", "");
  tracks_config_.AddField<int>("z3", "");
  tracks_config_.AddField<float>("p1", "");
  tracks_config_.AddField<float>("p2", "");
  tracks_config_.AddField<float>("p3", "");
  tracks_config_.AddField<float>("dedx", "");
  tracks_config_.AddField<float>("diffphi", "");
  tracks_config_.AddField<float>("pzcm", "");
  tracks_config_.AddField<float>("qphi", "");
  tracks_config_.AddField<float>("theta", "");
  tracks_config_.AddField<float>("weight", "");
  tracks_config_.AddField<float>("ylab", "");
  tracks_config_.AddField<float>("ycm", "");
  tracks_config_.AddField<float>("mass", "");
  tracks_config_.AddField<float>("nnet", "");
  configuration_.AddBranchConfig(tracks_config_);
  tracks_ = new AnalysisTree::TrackDetector (tracks_config_.GetId() );;

  out_tree_->Branch("event_header", "AnalysisTree::EventHeader",&event_header_);
  out_tree_->Branch("tracks", "AnalysisTree::TrackDetector", &tracks_);

  out_file_->cd();
  configuration_.Write("Configuration");
}

OutTreeManager::~OutTreeManager() {
  out_file_->cd();
  out_tree_->Write();
  out_tree_.reset();
  out_file_->Close();

  delete event_header_;
  delete tracks_;
}
void OutTreeManager::WriteDataHeader(const std::string &colliding_system,
                                     double T) {
  data_header_.SetSystem(colliding_system);
  const double M=0.940;
  double sqrt_snn = sqrt( 4*M*M + 2*M*T );
  auto p_mom = sqrt_snn * sqrt( sqrt_snn*sqrt_snn / ( 4*M*M ) - 1 );
  data_header_.SetBeamMomentum(p_mom);
  data_header_.Write("DataHeader");
}
void OutTreeManager::CheckIfNewFile() {
  auto* file = out_tree_->GetCurrentFile();
  if( file != out_file_.get() ) {
    out_file_.reset(file);
    out_file_->cd();
    configuration_.Write("Configuration");
    data_header_.Write("DataHeader");
  }
}
