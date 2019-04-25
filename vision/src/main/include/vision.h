#pragma once

#include "networktables/NetworkTable.h"
#include "networktables/NetworkTableEntry.h"
#include "networktables/NetworkTableInstance.h"

class curtinfrc_vision {
 public:
  void run();

 private:

  nt::NetworkTableEntry UsePedestrianTrackEntry;
  bool UsePedestrianTrack;

};

