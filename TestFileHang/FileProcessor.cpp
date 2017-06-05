#include "pch.h"
#include "FileProcessor.h"

using namespace concurrency;
using namespace Windows::Storage;
using namespace Windows::UI::Core;

FileProcessor::FileProcessor(Platform::String^ fileName)
	: m_fileName(fileName)
	, m_dispatcher(Windows::UI::Core::CoreWindow::GetForCurrentThread()->Dispatcher)
{
}

FileProcessor::~FileProcessor()
{
	Stop();
}

void FileProcessor::Start()
{
	// If the render loop is already running then do not start another thread.
	if (m_worker != nullptr && m_worker->Status == Windows::Foundation::AsyncStatus::Started)
	{
		return;
	}

	// Create a task for rendering that will be run on a background thread.
	auto workItemHandler = ref new Windows::System::Threading::WorkItemHandler([this](Windows::Foundation::IAsyncAction ^ action)
	{
		critical_section::scoped_lock lock(m_threadFence);

		while (action->Status == Windows::Foundation::AsyncStatus::Started)
		{
			try {
				create_task(KnownFolders::DocumentsLibrary->CreateFileAsync(m_fileName, CreationCollisionOption::ReplaceExisting)).then([this](StorageFile^ file) {
					return create_task(ApplicationData::Current->TemporaryFolder->CreateFileAsync(m_fileName, CreationCollisionOption::ReplaceExisting)).then([file, this](StorageFile^ tempFile) {
						return create_task(FileIO::WriteTextAsync(tempFile, m_fileName)).then([file, this, tempFile] {
							return tempFile->MoveAndReplaceAsync(file);
						});
					});
				}).get();

				m_dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this] {
					FileCreated(this, nullptr);
				}));
			}
			catch (Platform::Exception^ e) {
				m_dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this, e] {
					FileCreatedError(this, e);
				}));
			}

			// Sleep(10);
		}
	});

	m_worker = Windows::System::Threading::ThreadPool::RunAsync(workItemHandler, Windows::System::Threading::WorkItemPriority::Normal, Windows::System::Threading::WorkItemOptions::TimeSliced);
}

void FileProcessor::Stop()
{
	if (m_worker)
	{
		m_worker->Cancel();
		m_worker = nullptr;

		critical_section::scoped_lock lock(m_threadFence);
	}
}