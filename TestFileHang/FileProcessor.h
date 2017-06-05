#include <concrt.h>

#pragma once
ref class FileProcessor sealed
{
public:
	FileProcessor(Platform::String^ fileName);
	virtual ~FileProcessor();

	void Start();
	void Stop();

	event Windows::Foundation::EventHandler<Platform::Object^>^ FileCreated;
	event Windows::Foundation::EventHandler<Platform::Exception^>^ FileCreatedError;

private:
	Platform::String^ m_fileName;
	Windows::Foundation::IAsyncAction^ m_worker;
	concurrency::critical_section m_threadFence;
	Windows::UI::Core::CoreDispatcher^ m_dispatcher;
};

