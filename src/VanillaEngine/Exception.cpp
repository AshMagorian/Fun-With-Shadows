#include "Exception.h"

#include <iostream>

/**
*\brief initialises the message variable
*/
Exception::Exception(const std::string& _message)
{
	this->m_Message = _message;
}
/**
*\brief destructor
*/
Exception::~Exception() throw() { }
/**
*\brief returns the message
*/
const char* Exception::what() const throw()
{
	return m_Message.c_str();
}