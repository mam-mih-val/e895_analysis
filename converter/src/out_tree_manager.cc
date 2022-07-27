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
  event_header_config_.AddField<int>( "event_id", "The event id" );
  event_header_config_.AddField<int>( "M", "multiplicity for centrality determination" );
  event_header_config_.AddField<int>( "selected_M", "Selected multiplicity for centrality" );
  event_header_config_.AddField<int>( "N_V0", "number of V0 candidated" );
  event_header_config_.AddField<float>( "psi_ep", "Event plane angle" );
  event_header_config_.AddField<float>( "err_psi_ep", "dispersion of EP-angle" );
  event_header_config_.AddField<float>( "psi12", "RS difference" );
  event_header_config_.AddField<float>( "Qx", "x-component for the Q-vector of the event" );
  event_header_config_.AddField<float>( "Qy", "y-component for the Q-vector of the event" );
  event_header_config_.AddField<int>( "n", "number of tracks in the event" );

  configuration_.AddBranchConfig(event_header_config_);
  event_header_ = new AnalysisTree::EventHeader( event_header_config_.GetId() );
  event_header_->Init( event_header_config_ );

  tracks_config_ = AnalysisTree::BranchConfig( "tracks", AnalysisTree::DetType::kParticle );
  tracks_config_.AddField<int>("track_id", "track number");
  tracks_config_.AddField<int>("a1", "pid variable");
  tracks_config_.AddField<int>("a2", "pid variable");
  tracks_config_.AddField<int>("a3", "pid variable");
  tracks_config_.AddField<int>("z1", "charge");
  tracks_config_.AddField<int>("z2", "charge");
  tracks_config_.AddField<int>("z3", "charge");
  tracks_config_.AddField<float>("dEdx", "");
  tracks_config_.AddField<float>("log_dEdx", "");
  tracks_config_.AddField<float>("diffphi", "phi-Psi");
  tracks_config_.AddField<float>("pz_cm", "pz in CM system");
  tracks_config_.AddField<float>("qphi", "corrected phi-Psi");
  tracks_config_.AddField<float>("theta", "polar angle");
  tracks_config_.AddField<float>("weight", "weight of particle in rplane reconstr");
  tracks_config_.AddField<float>("e895_y_lab", "Rapidity written in original data format");
  tracks_config_.AddField<float>("e895_y_cm", "Rapidity written in original data format");
  tracks_config_.AddField<float>("e895_y_norm", "y_cm / y_beam");
  tracks_config_.AddField<float>("inv_mass", "invariant mass");
  tracks_config_.AddField<float>("nnet", "neural network parameter for V0");
  tracks_config_.AddField<float>("y_cm", "Rapidity based on PID");
  tracks_config_.AddField<float>("y_norm", "y_cm / y_beam");
  tracks_config_.AddField<bool>("primary", "If the particle is primary");
  configuration_.AddBranchConfig(tracks_config_);
  tracks_ = new AnalysisTree::Particles (tracks_config_.GetId() );;

  out_tree_->Branch("event_header", "AnalysisTree::EventHeader",&event_header_);
  out_tree_->Branch("tracks", "AnalysisTree::Particles", &tracks_);

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
