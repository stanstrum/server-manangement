#include "GmcDefaultServerFetch.hpp"

const char* GmcDefaultServerFetch::path() { return "/dashboard/game/servers/"; };
const char* GmcDefaultServerFetch::referrer() { return "/dashboard/profile/"; };
GmcApiRequest::Method GmcDefaultServerFetch::method() { return HEAD; };

void GmcDefaultServerFetch::finalize(CURL* curl) {};
void GmcDefaultServerFetch::consume_response(std::string response) {};
