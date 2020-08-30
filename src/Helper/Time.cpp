#include "Time.h"

#include <ctime>
#include "String.h"

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
	std::string day = MyString::ToString(ltm->tm_mday, 2);
	std::string hour = std::to_string(ltm->tm_hour);
	std::string minute = MyString::ToString(ltm->tm_min, 2);
	std::string second = MyString::ToString(ltm->tm_sec, 2);

	return year + "_" + month + "_" + day + "__" + hour + "h" + minute + "m" + second + "s";
}