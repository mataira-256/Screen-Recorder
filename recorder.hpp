#pragma once
#include <Windows.h>
#include "window.hpp"
#include <thread>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <codecvt>

void Recording();
void RecStop();
std::string getCurrentDateTimeString();