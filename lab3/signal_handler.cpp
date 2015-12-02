#include "signal_handler.h"

std::atomic<bool> QuitFlags::interrupt_flag(false);
std::atomic<bool> QuitFlags::quit_flag (false);



//bool SignalHandler::interrupt_flag = false;
//bool SignalHandler::quit_flag = false;