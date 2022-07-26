//
// Created by mikhail on 6/14/22.
//

#ifndef MCPICO2AT_SRC_OUT_TREE_MANAGER_H_
#define MCPICO2AT_SRC_OUT_TREE_MANAGER_H_

#include <TFile.h>
#include <TTree.h>

#include <DataHeader.hpp>
#include <Configuration.hpp>
#include <EventHeader.hpp>
#include <Detector.hpp>

class OutTreeManager {
public:
  OutTreeManager(const std::string& out_file_name,
                 const std::string& out_tree_name);
  virtual ~OutTreeManager();
  void Fill(){
    out_file_->cd();
    out_tree_->Fill();
  }
  [[nodiscard]] const AnalysisTree::BranchConfig &GetEventHeaderConfig() const {
    return event_header_config_;
  }
  [[nodiscard]] const AnalysisTree::BranchConfig &GetParticlesConfig() const {
    return tracks_config_;
  }
  [[nodiscard]] AnalysisTree::EventHeader *GetEventHeader() const { return event_header_; }
  [[nodiscard]] AnalysisTree::Particles *GetTracks() const { return tracks_; }
  [[nodiscard]] const AnalysisTree::DataHeader &GetDataHeader() const { return data_header_; }
  void CheckIfNewFile();
  void WriteDataHeader(const std::string& colliding_system, double energy);

private:
  std::unique_ptr<TFile> out_file_;
  std::unique_ptr<TTree> out_tree_;

  AnalysisTree::DataHeader data_header_;
  AnalysisTree::Configuration configuration_;

  AnalysisTree::BranchConfig event_header_config_;
  AnalysisTree::BranchConfig tracks_config_;

  AnalysisTree::EventHeader*event_header_{nullptr};
  AnalysisTree::Particles *tracks_{nullptr};
};

#endif // MCPICO2AT_SRC_OUT_TREE_MANAGER_H_
