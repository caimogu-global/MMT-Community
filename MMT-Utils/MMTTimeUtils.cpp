#include "MMTTimeUtils.h"

#include <boost/date_time/posix_time/posix_time.hpp>

namespace MMTTime {

	std::string GetCurrentTimeString() {
		boost::posix_time::ptime currentTime = boost::posix_time::second_clock::local_time();
		std::string CurrentTimeStr = boost::posix_time::to_iso_string(currentTime);
		return CurrentTimeStr;
	}
}