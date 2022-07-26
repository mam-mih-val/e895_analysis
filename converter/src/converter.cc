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
  auto out_event_header = out_tree_.GetEventHeader();
  auto out_event_header_config = out_tree_.GetEventHeaderConfig();
  auto out_tracks = out_tree_.GetTracks();
  auto out_tracks_config = out_tree_.GetParticlesConfig();

  auto id_event_header_int_run_id = out_event_header_config.GetFieldId("run_id");
  auto id_event_header_int_ievnt = out_event_header_config.GetFieldId("ievnt");
  auto id_event_header_int_ntrks_g = out_event_header_config.GetFieldId("ntrks_g");
  auto id_event_header_int_mfilt_g = out_event_header_config.GetFieldId("mfilt_g");
  auto id_event_header_int_nv0_g = out_event_header_config.GetFieldId("nv0_g");
  auto id_event_header_float_qphi_g = out_event_header_config.GetFieldId("qphi_g");
  auto id_event_header_float_dqphi_g = out_event_header_config.GetFieldId("dqphi_g");
  auto id_event_header_float_phi12_g = out_event_header_config.GetFieldId("phi12_g");
  auto id_event_header_float_qxcorr_g = out_event_header_config.GetFieldId("qxcorr_g");
  auto id_event_header_float_qycorr_g = out_event_header_config.GetFieldId("qycorr_g");
  auto id_event_header_int_n = out_event_header_config.GetFieldId("n");

  auto id_tracks_int_itrk = out_tracks_config.GetFieldId("itrk");
  auto id_tracks_int_a1 = out_tracks_config.GetFieldId("a1");
  auto id_tracks_int_a2 = out_tracks_config.GetFieldId("a2");
  auto id_tracks_int_a3 = out_tracks_config.GetFieldId("a3");
  auto id_tracks_int_z1 = out_tracks_config.GetFieldId("z1");
  auto id_tracks_int_z2 = out_tracks_config.GetFieldId("z2");
  auto id_tracks_int_z3 = out_tracks_config.GetFieldId("z3");
  auto id_tracks_float_p1 = out_tracks_config.GetFieldId("p1");
  auto id_tracks_float_p2 = out_tracks_config.GetFieldId("p2");
  auto id_tracks_float_p3 = out_tracks_config.GetFieldId("p3");
  auto id_tracks_float_dedx = out_tracks_config.GetFieldId("dedx");
  auto id_tracks_float_diffphi = out_tracks_config.GetFieldId("diffphi");
  auto id_tracks_float_pzcm = out_tracks_config.GetFieldId("pzcm");
  auto id_tracks_float_qphi = out_tracks_config.GetFieldId("qphi");
  auto id_tracks_float_theta = out_tracks_config.GetFieldId("theta");
  auto id_tracks_float_weight = out_tracks_config.GetFieldId("weight");
  auto id_tracks_float_ylab = out_tracks_config.GetFieldId("ylab");
  auto id_tracks_float_ycm = out_tracks_config.GetFieldId("ycm");
  auto id_tracks_float_mass = out_tracks_config.GetFieldId("mass");
  auto id_tracks_float_nnet = out_tracks_config.GetFieldId("nnet");

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

    int multiplicity = 0;
    for( int i=0; i<n; ++i ){
      auto [px, py, pz] = in_chain_.GetMomentum(i);
      auto itrk = in_chain_.GetItrk(i);
      auto a1 = in_chain_.GetA1(i);
      auto a2 = in_chain_.GetA2(i);
      auto a3 = in_chain_.GetA3(i);
      auto z1 = in_chain_.GetZ1(i);
      auto z2 = in_chain_.GetZ2(i);
      auto z3 = in_chain_.GetZ3(i);
      auto p1 = in_chain_.GetP1(i);
      auto p2 = in_chain_.GetP2(i);
      auto p3 = in_chain_.GetP3(i);
      auto dedx = in_chain_.GetDedx(i);
      auto diffphi = in_chain_.GetDiffphi(i);
      auto pzcm = in_chain_.GetPzcm(i);
      auto qphi = in_chain_.GetQphi(i);
      auto theta = in_chain_.GetTheta(i);
      auto weight = in_chain_.GetWeight(i);
      auto ylab = in_chain_.GetYlab(i);
      auto ycm = in_chain_.GetYcm(i);
      auto mass = in_chain_.GetMass(i);
      auto nnet = in_chain_.GetNnet(i);

      auto& track = out_tracks->AddChannel(out_tracks_config);

      track.SetMomentum( float(px), float(py), float(pz) );

      track.SetField(int(itrk), id_tracks_int_itrk);
      track.SetField(int(a1), id_tracks_int_a1);
      track.SetField(int(a2), id_tracks_int_a2);
      track.SetField(int(a3), id_tracks_int_a3);
      track.SetField(int(z1), id_tracks_int_z1);
      track.SetField(int(z2), id_tracks_int_z2);
      track.SetField(int(z3), id_tracks_int_z3);
      track.SetField(float(p1), id_tracks_float_p1);
      track.SetField(float(p2), id_tracks_float_p2);
      track.SetField(float(p3), id_tracks_float_p3);
      track.SetField(float(dedx), id_tracks_float_dedx);
      track.SetField(float(diffphi), id_tracks_float_diffphi);
      track.SetField(float(pzcm), id_tracks_float_pzcm);
      track.SetField(float(qphi), id_tracks_float_qphi);
      track.SetField(float(theta), id_tracks_float_theta);
      track.SetField(float(weight), id_tracks_float_weight);
      track.SetField(float(ylab), id_tracks_float_ylab);
      track.SetField(float(ycm), id_tracks_float_ycm);
      track.SetField(float(mass), id_tracks_float_mass);
      track.SetField(float(nnet), id_tracks_float_nnet);
    }

    out_tree_.Fill();
    out_tree_.CheckIfNewFile();
    n_events++;
  }
  std::cout << n_events << " events were converted to AnalysisTree" << std::endl;
}
