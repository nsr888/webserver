#include "ProcessMethod.hpp"

ProcessMethod::ProcessMethod(Request &request, Response &respone, std::string method)
{
	_response = respone;
	_request = request;
	_method = method;
}

ProcessMethod::~ProcessMethod()
{
}