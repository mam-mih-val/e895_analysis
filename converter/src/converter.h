//
// Created by mikhail on 6/14/22.
//

#ifndef MCPICO2AT_SRC_CONVERTER_H_
#define MCPICO2AT_SRC_CONVERTER_H_

#include "input_chain.h"
#include "out_tree_manager.h"

class Converter {
public:
  Converter(const std::string& in_file_list,
            const std::string& out_file_name,
            const std::string& out_tree_name);
  ~Converter() = default;
  void SetCollidingSystem(const std::string &colliding_system, double energy) {
    colliding_system_ = colliding_system;
    energy_ = energy;
    beta_cm_ = sqrt( 1 - 4*0.938*0.938 / energy / energy);
    gama_cm_ = 1.0 / sqrt( 1.0 - beta_cm_*beta_cm_ );
    out_tree_.WriteDataHeader( colliding_system, energy );
  }
  void SetRunId(int run_id) { run_id_ = run_id; }
  void Run();
private:
  int run_id_;
  std::string colliding_system_;
  double energy_; // Beam energy in AGeV
  double beta_cm_;
  double gama_cm_;
  InputChain in_chain_;
  OutTreeManager out_tree_;
};

#endif // MCPICO2AT_SRC_CONVERTER_H_
