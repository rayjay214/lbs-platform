#include "redisclient.h"
#include "gns_api_ice.h"
#include "string"
#include "map"

int getEidByUid(uint64_t devID, uint64_t *EID, std::string caller, std::string callee);

int getUserIdByImei(std::string strImei, uint64_t *devID , std::string caller, std::string callee);

