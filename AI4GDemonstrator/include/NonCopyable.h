#pragma once

class NonCopyable
{
public:
					NonCopyable()					{}
					NonCopyable(const NonCopyable&) = delete;
	virtual			~NonCopyable()					{}
	NonCopyable&	operator=(const NonCopyable&)	= delete;
};