#include "exceptions.h"

namespace mfast{

struct tag_error_code;
typedef boost::error_info<tag_error_code,std::string> fast_error_info;

fast_error::fast_error(const char* error_code)
{
  *this << fast_error_info(error_code);
}
}

namespace boost {
template class error_info<mfast::tag_referenced_by,std::string>;
template class error_info<mfast::tag_template_id,unsigned>;
template class error_info<mfast::tag_template_name,std::string>;
}