#ifndef _UCLIENT_H
#define _UCLIENT_H
#include "Usock.h"

class CUclient : public CUsock{
public:	
	CUclient(){}
	~CUclient(){}
	bool Connect();
protected:

private:

};
#endif

