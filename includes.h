#pragma once

#include "stdio.h"
#include "stdlib.h"
#include <xdevapi.h>
#include <string>
#include <string.h>
#include <utility>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <fstream>

#define _TURN_OFF_PLATFORM_STRING
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>
#include <cpprest/json.h> 
#include <cpprest/base_uri.h>

utility::string_t inline get__string_t(std::string input)
{
	return utility::conversions::to_string_t(input);
}