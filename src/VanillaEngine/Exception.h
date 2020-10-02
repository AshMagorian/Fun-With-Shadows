#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <exception>
#include <string>

/**
*Custom exception class which takes a string as a parmeter so that more useful error messages
*can be displayed. This is because strings can be added to other strings
*/
struct Exception : public std::exception
{
	/**
	*\brief initialises the message variable
	*/
	Exception(const std::string& _message);
	/**
	*\brief destructor
	*/
	virtual ~Exception() throw();
	/**
	*\brief returns the message
	*/
	virtual const char* what() const throw();

private:
	std::string m_Message; ///< The message to be printed

};
#endif