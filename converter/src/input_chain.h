//
// Created by mikhail on 6/16/22.
//

#ifndef MCPICO2AT_SRC_MC_PICO_CHAIN_CC_INPUT_CHAIN_H_
#define MCPICO2AT_SRC_MC_PICO_CHAIN_CC_INPUT_CHAIN_H_

#include <TChain.h>
#include <TVector3.h>
class InputChain {
public:
  InputChain() = delete;
  explicit InputChain(const std::string &input_file_list);
  virtual ~InputChain() = default;

  void AddFiles(const std::string &file_list);
  bool NextEvent();

  virtual std::tuple<float, float, float> GetMomentum(int idx) {
    return {p1_[idx], p2_[idx], p3_[idx]};
  }
  [[nodiscard]] long long int GetCurrentEvent() const { return current_event_; }
  [[nodiscard]] int GetIevnt() const { return ievnt_; }
  [[nodiscard]] int GetNtrksG() const { return ntrks_g_; }
  [[nodiscard]] int GetMfiltG() const { return mfilt_g_; }
  [[nodiscard]] int GetNv0G() const { return nv0_g_; }
  [[nodiscard]] float GetQphiG() const { return qphi_g_; }
  [[nodiscard]] float GetDqphiG() const { return dqphi_g_; }
  [[nodiscard]] float GetPhi12G() const { return phi12_g_; }
  [[nodiscard]] float GetQxcorrG() const { return qxcorr_g_; }
  [[nodiscard]] float GetQycorrG() const { return qycorr_g_; }
  [[nodiscard]] int GetN() const { return n_; }
  [[nodiscard]] int GetItrk(int idx) const { return itrk_[idx]; }
  [[nodiscard]] int GetA1(int idx) const { return a1_[idx]; }
  [[nodiscard]] int GetA2(int idx) const { return a2_[idx]; }
  [[nodiscard]] int GetA3(int idx) const { return a3_[idx]; }
  [[nodiscard]] int GetZ1(int idx) const { return z1_[idx]; }
  [[nodiscard]] int GetZ2(int idx) const { return z2_[idx]; }
  [[nodiscard]] int GetZ3(int idx) const { return z3_[idx]; }
  [[nodiscard]] float GetP1(int idx) const { return p1_[idx]; }
  [[nodiscard]] float GetP2(int idx) const { return p2_[idx]; }
  [[nodiscard]] float GetP3(int idx) const { return p3_[idx]; }
  [[nodiscard]] float GetDedx(int idx) const { return dedx_[idx]; }
  [[nodiscard]] float GetDiffphi(int idx) const { return diffphi_[idx]; }
  [[nodiscard]] float GetPzcm(int idx) const { return pzcm_[idx]; }
  [[nodiscard]] float GetQphi(int idx) const { return qphi_[idx]; }
  [[nodiscard]] float GetTheta(int idx) const { return theta_[idx]; }
  [[nodiscard]] float GetWeight(int idx) const { return weight_[idx]; }
  [[nodiscard]] float GetYlab(int idx) const { return ylab_[idx]; }
  [[nodiscard]] float GetYcm(int idx) const { return ycm_[idx]; }
  [[nodiscard]] float GetMass(int idx) const { return mass_[idx]; }
  [[nodiscard]] float GetNnet(int idx) const { return nnet_[idx]; }

protected:
  void InitFields();
  std::unique_ptr<TChain> chain_;
  long long current_event_ = -1;
  int ievnt_;
  int ntrks_g_;
  int mfilt_g_;
  int nv0_g_;
  float qphi_g_;
  float dqphi_g_;
  float phi12_g_;
  float qxcorr_g_;
  float qycorr_g_;
  int n_;
  int itrk_[500];      //[n]
  int a1_[500];        //[n]
  int a2_[500];        //[n]
  int a3_[500];        //[n]
  int z1_[500];        //[n]
  int z2_[500];        //[n]
  int z3_[500];        //[n]
  float p1_[500];      //[n]
  float p2_[500];      //[n]
  float p3_[500];      //[n]
  float dedx_[500];    //[n]
  float diffphi_[500]; //[n]
  float pzcm_[500];    //[n]
  float qphi_[500];    //[n]
  float theta_[500];   //[n]
  float weight_[500];  //[n]
  float ylab_[500];    //[n]
  float ycm_[500];     //[n]
  float mass_[500];    //[n]
  float nnet_[500];    //[n]
};

#endif // MCPICO2AT_SRC_MC_PICO_CHAIN_CC_INPUT_CHAIN_H_
