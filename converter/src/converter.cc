//
// Created by mikhail on 6/14/22.
//

#include <TDatabasePDG.h>
#include <random>
#include "converter.h"
#include "input_chain.h"
Converter::Converter(const std::string& in_file_list,
                     const std::string& out_file_name,
                     const std::string& out_tree_name)
    : out_tree_(out_file_name, out_tree_name), in_chain_(in_file_list){}
void Converter::Run() {
  auto data_header = out_tree_.GetDataHeader();
  auto out_event_header = out_tree_.GetEventHeader();
  auto out_event_header_config = out_tree_.GetEventHeaderConfig();
  auto out_tracks = out_tree_.GetTracks();
  auto out_tracks_config = out_tree_.GetParticlesConfig();
  const auto y_beam = data_header.GetBeamRapidity();

  const std::vector<float> impact_parameters{ 2, 3, 4, 5, 6, 7, 8, 9 };
  const std::vector<float> cross_section{ 1.8, 4.2, 7.4, 12, 17, 23, 30, 38 };
  std::vector<int> multiplicity_range;
  if( fabs( energy_ - 2.0 ) < std::numeric_limits<float>::min()  )
    multiplicity_range = {190, 175, 160, 140, 125, 105, 85, 60};
  if( fabs( energy_ - 4.0 ) < std::numeric_limits<float>::min()  )
    multiplicity_range = {260, 235, 210, 180, 155, 120, 90, 60};
  if( fabs( energy_ - 6.0 ) < std::numeric_limits<float>::min()  )
    multiplicity_range = {285, 260, 230, 200, 170, 135, 95, 60};
  if( fabs( energy_ - 8.0 ) < std::numeric_limits<float>::min()  )
    multiplicity_range = {320, 290, 260, 225, 190, 145, 105, 60};

  const auto id_event_header_int_run_id = out_event_header_config.GetFieldId("run_id");
  const auto id_event_header_int_ievnt = out_event_header_config.GetFieldId("event_id");
  const auto id_event_header_int_ntrks_g = out_event_header_config.GetFieldId("M");
  const auto id_event_header_int_mfilt_g = out_event_header_config.GetFieldId("selected_M");
  const auto id_event_header_int_nv0_g = out_event_header_config.GetFieldId("N_V0");
  const auto id_event_header_float_centrality = out_event_header_config.GetFieldId("centrality");
  const auto id_event_header_float_b = out_event_header_config.GetFieldId("b");
  const auto id_event_header_float_qphi_g = out_event_header_config.GetFieldId("psi_ep");
  const auto id_event_header_float_dqphi_g = out_event_header_config.GetFieldId("err_psi_ep");
  const auto id_event_header_float_phi12_g = out_event_header_config.GetFieldId("psi12");
  const auto id_event_header_float_qxcorr_g = out_event_header_config.GetFieldId("Qx");
  const auto id_event_header_float_qycorr_g = out_event_header_config.GetFieldId("Qy");
  const auto id_event_header_int_n = out_event_header_config.GetFieldId("n");

  const auto id_tracks_int_itrk = out_tracks_config.GetFieldId("track_id");
  const auto id_tracks_int_a1 = out_tracks_config.GetFieldId("a1");
  const auto id_tracks_int_a2 = out_tracks_config.GetFieldId("a2");
  const auto id_tracks_int_a3 = out_tracks_config.GetFieldId("a3");
  const auto id_tracks_int_z1 = out_tracks_config.GetFieldId("z1");
  const auto id_tracks_int_z2 = out_tracks_config.GetFieldId("z2");
  const auto id_tracks_int_z3 = out_tracks_config.GetFieldId("z3");
  const auto id_tracks_float_dedx = out_tracks_config.GetFieldId("dEdx");
  const auto id_tracks_float_log_dedx = out_tracks_config.GetFieldId("log_dEdx");
  const auto id_tracks_float_diffphi = out_tracks_config.GetFieldId("diffphi");
  const auto id_tracks_float_pzcm = out_tracks_config.GetFieldId("pz_cm");
  const auto id_tracks_float_qphi = out_tracks_config.GetFieldId("qphi");
  const auto id_tracks_float_theta = out_tracks_config.GetFieldId("theta");
  const auto id_tracks_float_weight = out_tracks_config.GetFieldId("weight");
  const auto id_tracks_float_e895_y_lab = out_tracks_config.GetFieldId("e895_y_lab");
  const auto id_tracks_float_e895_y_cm = out_tracks_config.GetFieldId("e895_y_cm");
  const auto id_tracks_float_norm_e895_y_cm = out_tracks_config.GetFieldId("e895_y_norm");
  const auto id_tracks_float_inv_mass = out_tracks_config.GetFieldId("inv_mass");
  const auto id_tracks_float_nnet = out_tracks_config.GetFieldId("nnet");
  const auto id_tracks_float_y_cm = out_tracks_config.GetFieldId("y_cm");
  const auto id_tracks_float_y_normalized = out_tracks_config.GetFieldId("y_norm");
  const auto id_tracks_bool_primary = out_tracks_config.GetFieldId("primary");
  const auto id_tracks_bool_baryon = out_tracks_config.GetFieldId("baryon");


  int n_events=0;
  while( in_chain_.NextEvent() ){
    out_tracks->ClearChannels();

    auto ievnt = in_chain_.GetIevnt();
    auto n_trks_g = in_chain_.GetNtrksG();
    auto mfilt_g = in_chain_.GetMfiltG();
    auto nv0_g = in_chain_.GetNv0G();
    auto qphi_g = in_chain_.GetQphiG();
    auto dqphi_g = in_chain_.GetDqphiG();
    auto phi12_g = in_chain_.GetPhi12G();
    auto qxcorr_g = in_chain_.GetQxcorrG();
    auto qycorr_g = in_chain_.GetQycorrG();
    auto n = in_chain_.GetN();

    out_event_header->SetField( int(run_id_), id_event_header_int_run_id );
    out_event_header->SetField( int(ievnt), id_event_header_int_ievnt );
    out_event_header->SetField( int(n_trks_g), id_event_header_int_ntrks_g );
    out_event_header->SetField( int(mfilt_g), id_event_header_int_mfilt_g );
    out_event_header->SetField( int(nv0_g), id_event_header_int_nv0_g );
    out_event_header->SetField( float(qphi_g), id_event_header_float_qphi_g );
    out_event_header->SetField( float(dqphi_g), id_event_header_float_dqphi_g );
    out_event_header->SetField( float(phi12_g), id_event_header_float_phi12_g );
    out_event_header->SetField( float(qxcorr_g), id_event_header_float_qxcorr_g );
    out_event_header->SetField( float(qycorr_g), id_event_header_float_qycorr_g );
    out_event_header->SetField( int(n), id_event_header_int_n );

    auto multiplicity = mfilt_g;
    auto centrality = -1.0f;
    auto impact_parameter = -1.0f;
    int idx = 0;
    float bin_edge = multiplicity_range[idx];
    while( multiplicity < bin_edge &&
           idx < multiplicity_range.size()-1 ){
      idx++;
      bin_edge = multiplicity_range[idx];
    }
    centrality = (cross_section[idx-1] + cross_section[idx])/2.0f;
    impact_parameter = (impact_parameters[idx-1] + impact_parameters[idx])/2.0f;
    out_event_header->SetField( float(centrality), id_event_header_float_centrality );
    out_event_header->SetField( float(impact_parameter), id_event_header_float_b );
    for( int i=0; i<n; ++i ){
      auto [px, py, pz] = in_chain_.GetMomentum(i);
      auto itrk = in_chain_.GetItrk(i);
      auto a1 = in_chain_.GetA1(i);
      auto a2 = in_chain_.GetA2(i);
      auto a3 = in_chain_.GetA3(i);
      auto z1 = in_chain_.GetZ1(i);
      auto z2 = in_chain_.GetZ2(i);
      auto z3 = in_chain_.GetZ3(i);
      auto dedx = in_chain_.GetDedx(i);
      auto diffphi = in_chain_.GetDiffphi(i);
      auto pzcm = in_chain_.GetPzcm(i);
      auto qphi = in_chain_.GetQphi(i);
      auto theta = in_chain_.GetTheta(i);
      auto weight = in_chain_.GetWeight(i);
      auto e895_ylab = in_chain_.GetYlab(i);
      auto e895_ycm = in_chain_.GetYcm(i);
      auto e895_y_norm = e895_ycm /y_beam;
      auto inv_mass = in_chain_.GetMass(i);
      auto nnet = in_chain_.GetNnet(i);

      auto& track = out_tracks->AddChannel(out_tracks_config);

      AnalysisTree::PdgCode_t pid = 0;
      auto tdb_pdg = TDatabasePDG::Instance();
      if(a1 == 0)
        pid = 211;
      if(a1 == 1)
        pid = 2212;
      if(a1 == 2)
        pid = 1'000'010'020;
      if(a1 == 5)
        pid = 321;

      auto mass = 0.f;
      if(tdb_pdg->GetParticle(pid))
        mass = tdb_pdg->GetParticle(pid)->Mass();
      if( pid == 1'000'010'020 )
        mass = 2*931.0;
      pid = pid*z1;
      track.SetMass(mass);
      track.SetPid(pid);
      bool primary = a2 == 0 && a3 == 0;
      auto E = sqrt( mass*mass + px*px + py*py + pz*pz );
      auto y = 0.5 * log( ( E + pz ) / ( E - pz ) );
      auto y_cm = y - y_beam;
      auto y_norm = y_cm / y_beam;
      auto is_baryon = true;
      if( fabs(pid) == 211 )
        is_baryon = false;
      if( fabs(pid) == 321 )
        is_baryon = false;

      track.SetMomentum( float(px), float(py), float(pz) );

      track.SetField(int(itrk), id_tracks_int_itrk);
      track.SetField(int(a1), id_tracks_int_a1);
      track.SetField(int(a2), id_tracks_int_a2);
      track.SetField(int(a3), id_tracks_int_a3);
      track.SetField(int(z1), id_tracks_int_z1);
      track.SetField(int(z2), id_tracks_int_z2);
      track.SetField(int(z3), id_tracks_int_z3);
      track.SetField(float(dedx), id_tracks_float_dedx);
      track.SetField(logf(dedx), id_tracks_float_log_dedx);
      track.SetField(float(diffphi), id_tracks_float_diffphi);
      track.SetField(float(pzcm), id_tracks_float_pzcm);
      track.SetField(float(qphi), id_tracks_float_qphi);
      track.SetField(float(theta), id_tracks_float_theta);
      track.SetField(float(weight), id_tracks_float_weight);
      track.SetField(float(e895_ylab), id_tracks_float_e895_y_lab);
      track.SetField(float(e895_ycm), id_tracks_float_e895_y_cm);
      track.SetField(float(e895_y_norm), id_tracks_float_norm_e895_y_cm);
      track.SetField(float(inv_mass), id_tracks_float_inv_mass);
      track.SetField(float(nnet), id_tracks_float_nnet);
      track.SetField(primary, id_tracks_bool_primary);
      track.SetField(is_baryon, id_tracks_bool_baryon);
      track.SetField(float(y_cm), id_tracks_float_y_cm);
      track.SetField(float(y_norm), id_tracks_float_y_normalized);
    }

    out_tree_.Fill();
    out_tree_.CheckIfNewFile();
    n_events++;
  }
  std::cout << n_events << " events were converted to AnalysisTree" << std::endl;
}
