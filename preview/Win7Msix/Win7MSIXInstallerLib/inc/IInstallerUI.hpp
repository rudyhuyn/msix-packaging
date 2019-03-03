#pragma once
namespace Win7MsixInstallerLib {

	class IInstallerUI
	{
	public:
		virtual void UpdateProgressBar() = 0;
		virtual HRESULT ShowUI() = 0;
	};
}