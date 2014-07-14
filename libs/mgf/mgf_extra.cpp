
#include "mgf_extra.h"

namespace mgf{

//###############################################################  stoT
template<> void stoT<float>(std::string str, std::size_t *pos, float *in){
	*in = std::stof(str, pos);
	return;
}

template<> void stoT<double>(std::string str, std::size_t *pos, double *in){
	*in = std::stod(str, pos);
	return;
}

} // mgf
