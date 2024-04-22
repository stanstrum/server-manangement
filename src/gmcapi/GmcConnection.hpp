#include <string>

class GmcConnection {
private:
  std::string host;
  std::string panel_url;

public:
  GmcConnection(std::string host);

  void debug();
};
