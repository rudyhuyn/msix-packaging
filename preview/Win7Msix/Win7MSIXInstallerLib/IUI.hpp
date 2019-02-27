#pragma once

class IUI
{
public:
	virtual void UpdateProgressBar() = 0;
	virtual HRESULT ShowUI() = 0;
};