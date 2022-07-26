// simple way to ignore the CAP command sent by clients as it is not part of the RFC

const std::string cap(const int &, const std::vector<std::string> &, const std::string &,Server *)
{ return (NULL); }