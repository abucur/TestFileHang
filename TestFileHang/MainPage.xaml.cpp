//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

#include <memory>

using namespace TestFileHang;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();

	m_processor1 = ref new FileProcessor("Test1.hang");
	BindProcessor(m_processor1, CountThread1);

	m_processor2 = ref new FileProcessor("Test2.hang");
	BindProcessor(m_processor2, CountThread2);

	m_processor3 = ref new FileProcessor("Test3.hang");
	BindProcessor(m_processor3, CountThread3);

	m_processor4 = ref new FileProcessor("Test4.hang");
	BindProcessor(m_processor4, CountThread4);

	m_processor5 = ref new FileProcessor("Test5.hang");
	BindProcessor(m_processor5, CountThread5);

	m_processor6 = ref new FileProcessor("Test6.hang");
	BindProcessor(m_processor6, CountThread6);
}


void TestFileHang::MainPage::StartButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}

void TestFileHang::MainPage::BindProcessor(FileProcessor^ processor, Windows::UI::Xaml::Controls::TextBlock^ block)
{
	WeakReference weakSelf(this);

	std::shared_ptr<int> counter = std::make_shared<int>(0);
	processor->FileCreated += ref new Windows::Foundation::EventHandler<Platform::Object^>([weakSelf, counter, block](Platform::Object^ sender, Platform::Object^) {
		auto self = weakSelf.Resolve<TestFileHang::MainPage>();

		int currentCount = (*counter)++;
		Platform::String^ newText = L"Count " + currentCount;
		block->Text = newText;
	});

	processor->FileCreatedError += ref new Windows::Foundation::EventHandler<Platform::Exception^>([](Platform::Object^ sender, Platform::Exception^ e) {

	});
}

void TestFileHang::MainPage::Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	m_processor1->Start();
	m_processor2->Start();
	m_processor3->Start();
	m_processor4->Start();
	m_processor5->Start();
	m_processor6->Start();
}
