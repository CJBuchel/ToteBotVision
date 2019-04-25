#include <iostream>
#include <networktables/NetworkTableInstance.h>
#include "vision.h"

#ifndef RUNNING_FRC_TESTS
int main(int argc, char **argv) {
  int team = 0;
  if (argc > 1) {
    team = std::stoi(argv[1]);
  }


#ifdef __DESKTOP__
  std::cout << "Running on Desktop - imshow enabled" << std::endl;
  bool isDesktop = true;
#else
  std::cout << "Running embedded - imshow disabled" << std::endl;
  bool isDesktop = false;
#endif

  auto ntinst = nt::NetworkTableInstance::GetDefault();
  if (team != 0) {
    std::cout << "Starting CurtinFRC Vision Program (Client Mode - Team " << team << ")" << std::endl;
    ntinst.StartClientTeam(team);
  } else {
    std::cout << "Starting CurtinFRC Vision Program (Server Mode - For Testing Only)" << std::endl;
    ntinst.SetServer("CurtinFRCVision");
    ntinst.StartServer();
  }

  auto inst = nt::NetworkTableInstance::GetDefault();
  auto visionTable = inst.GetTable("VisionTracking");

  
  curtinfrc_vision vision;  
  vision.run();

  std::cout << "Vision Program Exited. Now there is a chance that it is meant to do that... but chances are it broke" << std::endl;
  return -1;
}
#endif
