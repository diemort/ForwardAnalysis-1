
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"

const char* ExclusiveDijetsEvent::name = "ExclusiveDijetsEvent";

ExclusiveDijetsEvent::ExclusiveDijetsEvent() {}

ExclusiveDijetsEvent::~ExclusiveDijetsEvent() {}

void ExclusiveDijetsEvent::reset(){
  size_t len_hltTrigResults = sizeof(hltTrigResults_)/sizeof(int);
  for(size_t k = 0; k < len_hltTrigResults; ++k) hltTrigResults_[k] = 0;

  //...
}