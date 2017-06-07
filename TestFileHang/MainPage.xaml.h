//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "FileProcessor.h"

namespace TestFileHang
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

	private:
		void Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		void BindProcessor(FileProcessor^ processor, Windows::UI::Xaml::Controls::TextBlock^ block, Windows::UI::Xaml::Controls::TextBlock^ exceptionBlock);

		FileProcessor^ m_processor1;
		FileProcessor^ m_processor2;
		FileProcessor^ m_processor3;
		FileProcessor^ m_processor4;
		FileProcessor^ m_processor5;
		FileProcessor^ m_processor6;
	};
}
