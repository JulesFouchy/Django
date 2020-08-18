#include "Time.h"

#include <ctime>

std::string MyTime::AsString() {
	time_t now = time(0);
	tm* ltm = localtime(&now);
	std::string year = std::to_string(1900 + ltm->tm_year);
	std::string month;
	switch (ltm->tm_mon) {
	case 0:
		month = "January";
		break;
	case 1:
		month = "February";
		break;
	case 2:
		month = "March";
		break;
	case 3:
		month = "April";
		break;
	case 4:
		month = "May";
		break;
	case 5:
		month = "June";
		break;
	case 6:
		month = "July";
		break;
	case 7:
		month = "August";
		break;
	case 8:
		month = "September";
		break;
	case 9:
		month = "October";
		break;
	case 10:
		month = "November";
		break;
	case 11:
		month = "December";
		break;
	}
	std::string day = std::to_string(ltm->tm_mday);
	std::string hour = std::to_string(ltm->tm_hour);
	std::string minute = std::to_string(ltm->tm_min);
	std::string second = std::to_string(ltm->tm_sec);

	return year + "_" + month + "_" + day + "__" + hour + "h" + minute + "m" + second + "s";
}