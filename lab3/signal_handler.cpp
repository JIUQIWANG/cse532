#include "signal_handler.h"

std::atomic<bool> SignalHandler::interrupt_flag;
std::atomic<bool> SignalHandler::quit_flag;