#pragma once
#include "ImageProcessing.h"
#include "humoments.h"
#include <string>
#include <atlstr.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <ctime>

#define PI 3.14159

namespace ImageProcessing {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		LPCTSTR input, output;
		int width, height, printCount = 0, objectCount, *imageTagArray, *recogImage;//ImageTagArray->Objelere tag atılmış int temsili dizisi
		long size, new_size;
		double* angleArray;
		BYTE  *image, *rawintensity, *binaryImage, *taggedImage;//TaggedImage->Çerçeveli binary
		Bitmap^ surface;
		Color color;

		unsigned int *histogram = new unsigned int[256];

		MyForm(void)
		{
			InitializeComponent();

			color = Color::White;
		}

	protected:

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		System::Windows::Forms::PictureBox^  pictureBox1;
		System::Windows::Forms::Button^  button1;
		System::Windows::Forms::Button^  button2;
		System::Windows::Forms::Button^  button3;
		System::Windows::Forms::Button^  button4;
		System::Windows::Forms::Button^  button5;
		System::Windows::Forms::Button^  button6;
		System::Windows::Forms::Button^  button7;
		System::Windows::Forms::Button^  button8;
		System::Windows::Forms::Button^  button9;
		System::Windows::Forms::Button^  button10;
		System::Windows::Forms::Button^  button11;
		System::Windows::Forms::Button^  button12;
		System::Windows::Forms::Button^  button13;
		System::Windows::Forms::Button^  button14;
		System::Windows::Forms::Button^  button15;
		System::Windows::Forms::Label^  label1;
		System::Windows::Forms::Label^  label2;
		System::Windows::Forms::Label^  label3;
		System::Windows::Forms::ContextMenuStrip^  contextMenuStrip1;
		System::Windows::Forms::Label^  label4;
		System::Windows::Forms::Label^  label5;
		System::Windows::Forms::Label^  label6;
		System::Windows::Forms::Label^  label7;
		System::Windows::Forms::Label^  label8;
		System::Windows::Forms::Label^  label9;
		System::Windows::Forms::MenuStrip^  menuStrip1;
		System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
		System::Windows::Forms::OpenFileDialog^  openFileDialog1;
		System::Windows::Forms::PictureBox^  pictureBox2;
		System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;
	private: System::ComponentModel::IContainer^  components;
			 /// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->button9 = (gcnew System::Windows::Forms::Button());
			this->button10 = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->button11 = (gcnew System::Windows::Forms::Button());
			this->button12 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->button13 = (gcnew System::Windows::Forms::Button());
			this->button14 = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->contextMenuStrip1 = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->button15 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(8, 23);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(522, 500);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(1180, 613);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 36);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Reset";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(8, 613);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 36);
			this->button2->TabIndex = 2;
			this->button2->Text = L"Draw Circle";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::button2_Click);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(1099, 613);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 38);
			this->button3->TabIndex = 3;
			this->button3->Text = L"Save";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::button3_Click);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1420, 24);
			this->menuStrip1->TabIndex = 4;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->openToolStripMenuItem });
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(103, 22);
			this->openToolStripMenuItem->Text = L"Open";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::openToolStripMenuItem_Click);
			// 
			// pictureBox2
			// 
			this->pictureBox2->Location = System::Drawing::Point(818, 23);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(498, 500);
			this->pictureBox2->TabIndex = 5;
			this->pictureBox2->TabStop = false;
			// 
			// chart1
			// 
			chartArea1->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->chart1->Legends->Add(legend1);
			this->chart1->Location = System::Drawing::Point(8, 674);
			this->chart1->Name = L"chart1";
			series1->ChartArea = L"ChartArea1";
			series1->Legend = L"Legend1";
			series1->Name = L"Pixels";
			this->chart1->Series->Add(series1);
			this->chart1->Size = System::Drawing::Size(565, 292);
			this->chart1->TabIndex = 6;
			this->chart1->Text = L"chart1";
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(170, 612);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(75, 37);
			this->button4->TabIndex = 7;
			this->button4->Text = L"Show Histogram";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MyForm::button4_Click);
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(251, 613);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(75, 37);
			this->button5->TabIndex = 8;
			this->button5->Text = L"K-Means";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &MyForm::button5_Click);
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(89, 613);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(75, 36);
			this->button6->TabIndex = 9;
			this->button6->Text = L"Show Intensity";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &MyForm::button6_Click);
			// 
			// button7
			// 
			this->button7->Location = System::Drawing::Point(641, 487);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(75, 36);
			this->button7->TabIndex = 10;
			this->button7->Text = L"Cipher";
			this->button7->UseVisualStyleBackColor = true;
			this->button7->Click += gcnew System::EventHandler(this, &MyForm::button7_Click);
			// 
			// button8
			// 
			this->button8->Location = System::Drawing::Point(332, 612);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(77, 36);
			this->button8->TabIndex = 11;
			this->button8->Text = L"Assign Tags";
			this->button8->UseVisualStyleBackColor = true;
			this->button8->Click += gcnew System::EventHandler(this, &MyForm::button8_Click);
			// 
			// button9
			// 
			this->button9->Location = System::Drawing::Point(641, 138);
			this->button9->Name = L"button9";
			this->button9->Size = System::Drawing::Size(75, 37);
			this->button9->TabIndex = 12;
			this->button9->Text = L"Erosion";
			this->button9->UseVisualStyleBackColor = true;
			this->button9->Click += gcnew System::EventHandler(this, &MyForm::button9_Click);
			// 
			// button10
			// 
			this->button10->Location = System::Drawing::Point(641, 181);
			this->button10->Name = L"button10";
			this->button10->Size = System::Drawing::Size(75, 37);
			this->button10->TabIndex = 13;
			this->button10->Text = L"Dilation";
			this->button10->UseVisualStyleBackColor = true;
			this->button10->Click += gcnew System::EventHandler(this, &MyForm::button10_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			this->openFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::openFileDialog1_FileOk);
			// 
			// button11
			// 
			this->button11->Location = System::Drawing::Point(641, 39);
			this->button11->Name = L"button11";
			this->button11->Size = System::Drawing::Size(75, 37);
			this->button11->TabIndex = 14;
			this->button11->Text = L"Average Filter";
			this->button11->UseVisualStyleBackColor = true;
			this->button11->Click += gcnew System::EventHandler(this, &MyForm::button11_Click);
			// 
			// button12
			// 
			this->button12->Location = System::Drawing::Point(641, 82);
			this->button12->Name = L"button12";
			this->button12->Size = System::Drawing::Size(75, 37);
			this->button12->TabIndex = 15;
			this->button12->Text = L"Median Filter";
			this->button12->UseVisualStyleBackColor = true;
			this->button12->Click += gcnew System::EventHandler(this, &MyForm::button12_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(628, 23);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(101, 13);
			this->label1->TabIndex = 16;
			this->label1->Text = L"Intensity Processing";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(628, 122);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(97, 13);
			this->label2->TabIndex = 17;
			this->label2->Text = L" Binary Processing ";
			// 
			// button13
			// 
			this->button13->Location = System::Drawing::Point(641, 403);
			this->button13->Name = L"button13";
			this->button13->Size = System::Drawing::Size(75, 36);
			this->button13->TabIndex = 18;
			this->button13->Text = L"huMoments";
			this->button13->UseVisualStyleBackColor = true;
			this->button13->Click += gcnew System::EventHandler(this, &MyForm::button13_Click);
			// 
			// button14
			// 
			this->button14->Location = System::Drawing::Point(641, 445);
			this->button14->Name = L"button14";
			this->button14->Size = System::Drawing::Size(75, 36);
			this->button14->TabIndex = 19;
			this->button14->Text = L"Recognition";
			this->button14->UseVisualStyleBackColor = true;
			this->button14->Click += gcnew System::EventHandler(this, &MyForm::button14_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->ForeColor = System::Drawing::Color::Red;
			this->label3->Location = System::Drawing::Point(588, 674);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(85, 13);
			this->label3->TabIndex = 20;
			this->label3->Text = L"Kırmızı Mercimek";
			// 
			// contextMenuStrip1
			// 
			this->contextMenuStrip1->Name = L"contextMenuStrip1";
			this->contextMenuStrip1->Size = System::Drawing::Size(61, 4);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->ForeColor = System::Drawing::Color::Green;
			this->label4->Location = System::Drawing::Point(588, 696);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(78, 13);
			this->label4->TabIndex = 22;
			this->label4->Text = L"Yeşil Mercimek";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->ForeColor = System::Drawing::Color::Purple;
			this->label5->Location = System::Drawing::Point(588, 721);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(43, 13);
			this->label5->TabIndex = 23;
			this->label5->Text = L"Fasulye";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->ForeColor = System::Drawing::Color::Blue;
			this->label6->Location = System::Drawing::Point(588, 744);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(33, 13);
			this->label6->TabIndex = 24;
			this->label6->Text = L"Ceviz";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->label7->Location = System::Drawing::Point(588, 766);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(49, 13);
			this->label7->TabIndex = 25;
			this->label7->Text = L"Çekirdek";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->ForeColor = System::Drawing::Color::Olive;
			this->label8->Location = System::Drawing::Point(588, 790);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(33, 13);
			this->label8->TabIndex = 26;
			this->label8->Text = L"Pirinç";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(128)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->label9->Location = System::Drawing::Point(588, 815);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(33, 13);
			this->label9->TabIndex = 27;
			this->label9->Text = L"Erişte";
			// 
			// button15
			// 
			this->button15->Location = System::Drawing::Point(416, 613);
			this->button15->Name = L"button15";
			this->button15->Size = System::Drawing::Size(75, 35);
			this->button15->TabIndex = 28;
			this->button15->Text = L"Canny";
			this->button15->UseVisualStyleBackColor = true;
			this->button15->Click += gcnew System::EventHandler(this, &MyForm::button15_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1420, 1000);
			this->Controls->Add(this->button15);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->button14);
			this->Controls->Add(this->button13);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button12);
			this->Controls->Add(this->button11);
			this->Controls->Add(this->button10);
			this->Controls->Add(this->button9);
			this->Controls->Add(this->button8);
			this->Controls->Add(this->button7);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->chart1);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
			//Load BMP Dialog
			private: System::Void openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {}

			//Restart
			private: System::Void MyForm::button1_Click(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);

			//Draw Circle
			private: System::Void MyForm::button2_Click(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
			
			//Open BMP
			private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e);

			//Show Intensity
			private: System::Void button6_Click(System::Object^  sender, System::EventArgs^  e);

			//Average-Mean Filter
			private: System::Void button11_Click(System::Object^  sender, System::EventArgs^  e);

			//Median Filter
			private: System::Void button12_Click(System::Object^  sender, System::EventArgs^  e);

			//Show Histogram
			private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e);

			//K-Means
			private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e);

			//Erosion
			private: System::Void button9_Click(System::Object^  sender, System::EventArgs^  e);

			//Dilation
			private: System::Void button10_Click(System::Object^  sender, System::EventArgs^  e);

			//Assign Tags
			private: System::Void button8_Click(System::Object^  sender, System::EventArgs^  e);

			//Tagger
			private: Void Tagger(int* markMap, int position, int mark);

			//Euklidean Distance Function
			private: double euklidean_Distance(double *arr1, double *arr2);

			//huMoments
			private: System::Void button13_Click(System::Object^  sender, System::EventArgs^  e);

			//Recognition
			private: System::Void button14_Click(System::Object^  sender, System::EventArgs^  e);

			//Canny Edge Detection
			private: System::Void button15_Click(System::Object^  sender, System::EventArgs^  e);

			//Cipher
			private: System::Void button7_Click(System::Object^  sender, System::EventArgs^  e);

			//Save
			private: Void Save(BYTE* Buffer, int widthf, int heightf, long ssize);

			//Save Picture Box 2
			private: System::Void MyForm::button3_Click(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
};
}
