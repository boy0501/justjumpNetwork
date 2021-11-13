#pragma once
#include "Client.h"

class LoginClient : public Client
{
public:
	LoginClient();
	virtual ~LoginClient();

	virtual void update(float delta_time) override;

	float elapsedtime;

};

