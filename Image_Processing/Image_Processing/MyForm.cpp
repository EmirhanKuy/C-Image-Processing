#include "MyForm.h"

using namespace System;
using namespace ImageProcessing;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

//Restart
System::Void MyForm::button1_Click(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	System::Windows::Forms::Application::Restart();
}

//Draw Circle
System::Void MyForm::button2_Click(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	int CoordinateX = pictureBox1->Width / 2, CoordinateY = pictureBox1->Height / 2;
	int centerx = 0, centery = 0, r = 60;
	double firstDotx = centerx + r, firstDoty = centery, Dotx, Doty;
	double sine1 = 0.01745240643, cosine1 = 0.99984769515;
	double rotMatrix1[4] = { cosine1, -sine1, sine1, cosine1 };

	surface = gcnew Bitmap(pictureBox2->Width, pictureBox2->Height);

	surface->SetPixel(centerx + CoordinateX, centery + CoordinateY, Color::Red);
	pictureBox2->Image = surface;

	surface->SetPixel(firstDotx + CoordinateX, firstDoty + CoordinateY, Color::Black);
	Dotx = firstDotx;
	Doty = firstDoty;

	for (int i = 0; i < 360; i++)
	{
		double tempx = Dotx, tempy = Doty;
		Dotx = (tempx * rotMatrix1[0]) + (tempy * rotMatrix1[2]);
		Doty = (tempx * rotMatrix1[1]) + (tempy * rotMatrix1[3]);
		surface->SetPixel(Dotx + CoordinateX, Doty + CoordinateY, Color::Black);
		pictureBox2->Image = surface;
	}
}

//Open BMP
System::Void MyForm::openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
{
	CString str;
	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
	{
		str = openFileDialog1->FileName;
		input = (LPCTSTR)str;
		//Read BMP
		image = LoadBMP(width, height, size, input);
		pictureBox1->Width = width;
		pictureBox1->Height = height;

		pictureBox1->ImageLocation = openFileDialog1->FileName;
		rawintensity = ConvertBMPToIntensity(image, width, height);
	}
}

//Show Intensity
System::Void MyForm::button6_Click(System::Object^  sender, System::EventArgs^  e)
{
	pictureBox2->Width = pictureBox1->Width;
	pictureBox2->Height = pictureBox1->Height;
	
	//Displaying the Image without using the Bitmap class...
	//Save(rawintensity, width, height, new_size);
	//pictureBox2->ImageLocation = ("C://Users//Nahrime//source//repos//Image_Processing//Image_Processing//Images//Outs//myImage" + (printCount - 1) + ".bmp");

	surface = gcnew Bitmap(width, height);
	Color pixelColor;

	for (int row = 0; row < height; row++)
		for (int col = 0; col < width; col++)
		{
			int pos = row * width + col;
			pixelColor = Color::FromArgb(rawintensity[pos], rawintensity[pos], rawintensity[pos]);
			surface->SetPixel(col, row, pixelColor);
		}

	pictureBox2->Image = surface;
}

//Average-Mean Filter
System::Void MyForm::button11_Click(System::Object^  sender, System::EventArgs^  e)
{
	BYTE* filteredIntensity = new BYTE[height * width];

	for (int row = 0; row < height; row++)
		for (int col = 0; col < width; col++)
		{
			int pos = row * width + col;

			//Keep the very edge same.
			if (!row || !col || row == height - 1 || col == width - 1)
				filteredIntensity[pos] = rawintensity[pos];
			else
				filteredIntensity[pos] = 0;
		}

	int maskPos[9] = { -width - 1, -width, -width + 1,
					-1, 0, 1,
				width - 1, width, width + 1 };

	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int avg = 0;
			int pos = row * width + col;
			avg = rawintensity[pos + maskPos[0]] + rawintensity[pos + maskPos[1]] + rawintensity[pos + maskPos[2]] + rawintensity[pos + maskPos[3]] + rawintensity[pos] + rawintensity[pos + maskPos[5]] + rawintensity[pos + maskPos[6]] + rawintensity[pos + maskPos[7]] + rawintensity[pos + maskPos[8]];
			avg /= 9;

			filteredIntensity[pos] = avg;
		}

	Bitmap^ surface = gcnew Bitmap(width, height);
	Color pixelColor;

	for (int row = 0; row < height; row++)
		for (int col = 0; col < width; col++)
		{
			int pos = row * width + col;
			pixelColor = Color::FromArgb(filteredIntensity[pos], filteredIntensity[pos], filteredIntensity[pos]);
			surface->SetPixel(col, row, pixelColor);
		}
	rawintensity = filteredIntensity;
	pictureBox2->Image = surface;
}

//Median Filter
System::Void MyForm::button12_Click(System::Object^  sender, System::EventArgs^  e)
{
	BYTE* filteredIntensity = new BYTE[height * width];
	for (int row = 0; row < height; row++)
		for (int col = 0; col < width; col++)
		{
			int pos = row * width + col;

			if (!row || !col || row == height - 1 || col == width - 1)
				filteredIntensity[pos] = rawintensity[pos];
			else
				filteredIntensity[pos] = 0;
		}

	int maskPos[9] = { -width - 1, -width, -width + 1,
				-1, 0, 1,
			width - 1, width, width + 1 };

	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;

			int values[9] =
			{
				rawintensity[pos + maskPos[0]], rawintensity[pos + maskPos[1]], rawintensity[pos + maskPos[2]],
				rawintensity[pos + maskPos[3]], rawintensity[pos + maskPos[4]], rawintensity[pos + maskPos[5]],
				rawintensity[pos + maskPos[6]], rawintensity[pos + maskPos[7]], rawintensity[pos + maskPos[8]]
			};

			//Sorting the array
			int min, temp;
			for (int i = 0; i < 8; i++)
			{
				min = i;
				for (int j = i + 1; j < 9; j++)
					if (values[j] < values[min])
						min = j;
				temp = values[i];
				values[i] = values[min];
				values[min] = temp;
			}

			filteredIntensity[pos] = values[4];
		}

	Bitmap^ surface = gcnew Bitmap(width, height);

	Color pixelColor;

	for (int row = 0; row < height; row++)
		for (int col = 0; col < width; col++)
		{
			int pos = row * width + col;
			pixelColor = Color::FromArgb(filteredIntensity[pos], filteredIntensity[pos], filteredIntensity[pos]);
			surface->SetPixel(col, row, pixelColor);
		}

	pictureBox2->Image = surface;
	rawintensity = filteredIntensity;
}

//Show Histogram
System::Void MyForm::button4_Click(System::Object^  sender, System::EventArgs^  e)
{
	for (int i = 0; i < 256; i++)
		histogram[i] = 0;

	int pixelCount = width * height;

	for (int i = 0; i < pixelCount; i++)
		histogram[(int)rawintensity[i]]++;

	for (int i = 0; i < 256; i++)
		chart1->Series["Pixels"]->Points->AddXY(i, histogram[i]);
}

//K-Means Binary
System::Void MyForm::button5_Click(System::Object^  sender, System::EventArgs^  e)
{
	double cluster1 = 0, cluster2 = 255, centroid1 = -1, centroid2 = -1;
	double sum1 = 0, sum2 = 0, division1 = 0, division2 = 0;

	surface = gcnew Bitmap(width, height);
	binaryImage = new BYTE[width * height];

	while (true)
	{
		for (int i = 0; i < 256; i++)
		{
			if (Math::Abs(i - cluster1) < Math::Abs(i - cluster2))
			{
				sum1 += (i * histogram[i]);
				division1 += histogram[i];
			}
			else
			{
				sum2 += (i * histogram[i]);
				division2 += histogram[i];
			}
		}

		if (!division1)
			division1++;
		if (!division2)
			division2++;

		centroid1 = Math::Round((sum1 / division1));
		centroid2 = Math::Round((sum2 / division2));

		if ((cluster1 == centroid1) && (cluster2 == centroid2))
			break;

		cluster1 = centroid1;
		cluster2 = centroid2;
	}

	for (int row = 0; row < height; row++)
	{
		for (int column = 0; column < width; column++)
		{
			if (Math::Abs(rawintensity[row * width + column] - centroid1) < Math::Abs(rawintensity[row * width + column] - centroid2))
			{
				color = Color::White;
				surface->SetPixel(column, row, color);
				binaryImage[row * width + column] = (BYTE)255;
			}
			else
			{
				color = Color::Black;
				surface->SetPixel(column, row, color);
				binaryImage[row * width + column] = (BYTE)0;
			}
		}
	}
	pictureBox2->Image = surface;
}

//Erosion
System::Void MyForm::button9_Click(System::Object^  sender, System::EventArgs^  e)
{
	BYTE* erodedImage = new BYTE[height * width];
	for (int i = 0; i < height * width; i++)
		erodedImage[i] = 0;

	bool key = 0;

	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;
			key = (binaryImage[pos - width] && binaryImage[pos - 1] && binaryImage[pos] && binaryImage[pos + 1] && binaryImage[pos + width]);

			if (key == true)
				erodedImage[pos] = 255;
			else
				erodedImage[pos] = 0;
		}

	Bitmap^ surface = gcnew Bitmap(width, height);

	for (int row = 0; row < height; row++)
		for (int col = 0; col < width; col++)
		{
			int pos = row * width + col;

			if (erodedImage[pos])
				surface->SetPixel(col, row, Color::White);
			else
				surface->SetPixel(col, row, Color::Black);
		}
	binaryImage = erodedImage;
	pictureBox2->Image = surface;
}

//Dilation
System::Void MyForm::button10_Click(System::Object^  sender, System::EventArgs^  e)
{
	BYTE* dilatedImage = new BYTE[height * width];
	for (int i = 0; i < height * width; i++)
		dilatedImage[i] = 0;

	bool key = 0;

	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;
			key = (binaryImage[pos - width] || binaryImage[pos - 1] || binaryImage[pos] || binaryImage[pos + 1] || binaryImage[pos + width]);

			if (key == true)
				dilatedImage[pos] = 255;
			else
				dilatedImage[pos] = 0;
		}

	Bitmap^ surface = gcnew Bitmap(width, height);

	for (int row = 0; row < height; row++)
		for (int col = 0; col < width; col++)
		{
			int pos = row * width + col;

			if (dilatedImage[pos])
				surface->SetPixel(col, row, Color::White);
			else
				surface->SetPixel(col, row, Color::Black);
		}
	binaryImage = dilatedImage;
	pictureBox2->Image = surface;
}

//Assign Tags
System::Void  MyForm::button8_Click(System::Object^  sender, System::EventArgs^  e)
{
	taggedImage = new BYTE[height * width];

	for (int row = 0; row < height; row++)
		for (int col = 0; col < width; col++)
		{
			int pos = row * width + col;
			taggedImage[pos] = binaryImage[pos];
		}

	////int neighbourMatrix[8];
	////for (int i = 0; i < 9; i++)
	////{
	////	if( i == 0 || 2 || 6 || 8)
	////		neighbourMatrix[i] = 0;
	////	else
	////		neighbourMatrix[i] = 1;
	////}

	//int newTag;
	//for (int row = 0; row < height - 2; row++)
	//	for (int col = 0; col < width - 2; col++)
	//	{
	//		int pos = row * width + col;
	//		int* aim = new int[9];
	//		aim[0] = pos;
	//		aim[1] = pos + 1;
	//		aim[2] = pos + 2;
	//		aim[3] = pos + width;
	//		aim[4] = pos + width + 1;
	//		aim[5] = pos + width + 2;
	//		aim[6] = pos + (2 * width);
	//		aim[7] = pos + (2 * width) + 1;
	//		aim[8] = pos + (2 * width) + 2;

	//		if (imageTagArray[aim[4]] != 0)
	//		{
	//			newTag = imageTagArray[aim[4]];
	//			for (int i = 0; i < 9; i++)
	//			{
	//				if (imageTagArray[aim[i]] != 0 && imageTagArray[aim[i]] < newTag)
	//					newTag = imageTagArray[aim[i]];
	//			}
	//			for (int i = 0; i < 9; i++)
	//			{
	//				if (imageTagArray[aim[i]] != 0)
	//					imageTagArray[aim[i]] = newTag;
	//			}
	//		}
	//	}

	int tag = 0;
	imageTagArray = new int[height * width];
	for (int i = 0; i < height * width; i++)
		imageTagArray[i] = 0;

	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;

			if (binaryImage[pos] == 255 && !imageTagArray[pos])
			{
				tag++;
				Tagger(imageTagArray, pos, tag);
			}
		}

	objectCount = tag;
	int window[4];

	std::ofstream out;
	out.open("ObjectRectangles.txt");

	for (int i = 1; i <= objectCount; i++)
	{
		window[0] = width - 1;
		window[1] = height - 1;
		window[2] = 0;
		window[3] = 0;
		for (int row = 1; row < height - 1; row++)
			for (int col = 1; col < width - 1; col++)
			{
				int pos = row * width + col;

				if (imageTagArray[pos] == i)
				{
					if (col < window[0])    //x1
						window[0] = col;
					if (row < window[1])    //y1
						window[1] = row;
					if (col > window[2])    //x2
						window[2] = col;
					if (row > window[3])    //y2
						window[3] = row;
				}
			}

		out << window[0] << " " << window[1] << " " << window[2] << " " << window[3] << " ";


		for (int i = window[1] * width + window[0]; i <= window[1] * width + window[2]; i++)
			taggedImage[i] = 155;
		for (int i = window[3] * width + window[0]; i <= window[3] * width + window[2]; i++)
			taggedImage[i] = 155;
		for (int i = window[1] * width + window[0]; i <= window[3] * width + window[0]; i += width)
			taggedImage[i] = 155;
		for (int i = window[1] * width + window[2]; i <= window[3] * width + window[2]; i += width)
			taggedImage[i] = 155;
	}
	out.close();

	Bitmap^ surface = gcnew Bitmap(width, height);

	for (int row = 0; row < height; row++)
		for (int col = 0; col < width; col++)
		{
			int pos = row * width + col;
			if (taggedImage[pos] == 0)
				surface->SetPixel(col, row, Color::Black);
			else if (taggedImage[pos] == 155)
				surface->SetPixel(col, row, Color::Red);
			else if (taggedImage[pos] == 255)
				surface->SetPixel(col, row, Color::White);
		}
	pictureBox2->Image = surface;
}

//Tag Function
Void MyForm::Tagger(int* markMap, int position, int mark)
{
	markMap[position] = mark;

	if (binaryImage[position + 1] == 255 && !markMap[position + 1])
		Tagger(markMap, position + 1, mark);
	if (binaryImage[position - 1] == 255 && !markMap[position - 1])
		Tagger(markMap, position - 1, mark);
	if (binaryImage[position + width] == 255 && !markMap[position + width])
		Tagger(markMap, position + width, mark);
	if (binaryImage[position - width] == 255 && !markMap[position - width])
		Tagger(markMap, position - width, mark);
}

//Euklidean Distance Function
double MyForm::euklidean_Distance(double *arr1, double *arr2)
{
	int momentsSize = 7;
	double temp = 0;
	for (int i = 0; i < momentsSize; i++)
	{
		temp += pow((arr1[i] - arr2[i]), 2);
	}
	return sqrt(temp);
}

//huMoments
System::Void  MyForm::button13_Click(System::Object^  sender, System::EventArgs^  e)
{
	unsigned char** objectArray = new unsigned char*[objectCount];

	double** objectMoments = new double*[objectCount];
	for (int i = 0; i < objectCount; i++)
		objectMoments[i] = new double[7];

	int window[4];
	int windowWidth, windowHeight, windowPosition;
	std::ifstream in("ObjectRectangles.txt");

	for (int i = 0; i < objectCount; i++)
	{
		in >> window[0];
		in >> window[1];
		in >> window[2];
		in >> window[3];

		windowWidth = window[2] - window[0] + 1;
		windowHeight = window[3] - window[1] + 1;

		objectArray[i] = new unsigned char[windowWidth * windowHeight];

		windowPosition = 0;
		for (int row = window[1]; row <= window[3]; row++)
			for (int col = window[0]; col <= window[2]; col++)
			{
				int pos = row * width + col;

				objectArray[i][windowPosition] = (int)binaryImage[pos];
				windowPosition++;
			}

		myHuMoments* huMomentsObj = new myHuMoments(objectArray[i], windowWidth, windowHeight);
		objectMoments[i] = huMomentsObj->getHuMoments();		
		huMomentsObj->~myHuMoments();
	}
	in.close();

	std::ofstream out("ObjectMoments.txt");

	//for (int i = 0; i < 7; i++)
	//{
	//	for (int j = 0; j < objectCount; j++)
	//		out << objectMoments[j][i] << " ";
	//	out << std::endl;
	//}

	double temp;
	for (int i = 0; i < 7; i++)
	{
		temp = 0;
		for (int j = 0; j < objectCount; j++)
			temp += objectMoments[j][i] / objectCount;
		out << temp << " ";
	}
	out.close();
}

//Recognition
System::Void  MyForm::button14_Click(System::Object^  sender, System::EventArgs^  e)
{
	recogImage = new int[height * width];
	for (int i = 0; i < width * height; i++)
		recogImage[i] = imageTagArray[i];

	double** savedMoments = new double*[7];
	for (int i = 0; i < 4; i++)
		savedMoments[i] = new double[7];

	double YMercimek[7] = {0.000641218, 1.78701e-08, 7.38951e-13, 5.34067e-14, 4.31811e-26, 9.88018e-18, 4.86388e-27};//Yeşil
	double KMercimek[7] = {0.000671783, 6.31436e-08, 1.2755e-12, 1.50889e-13, 5.57229e-25, 6.94845e-17, 4.30592e-25};//Kırmızı
	double Pirinc[7] = {0.000850257, 3.23333e-07, 1.43089e-11, 1.17783e-12, -1.32337e-24, -3.16679e-16, -2.24316e-24};//Sarı
	double Fasulye[7] = {0.000734864, 6.05305e-08, 8.87971e-12, 3.91783e-12, -4.03264e-24, 6.31098e-16, -8.22573e-23};//Mor
	double Cekirdek[7] = {0.00101535, 6.51629e-07, 4.345e-11, 1.44893e-11, 5.79745e-22, 1.15439e-14, 1.03524e-22};//Pembe
	double Ceviz[7] = {0.000805207, 2.16517e-07, 2.64535e-11, 1.74554e-12, 1.07621e-23, 5.31172e-16, -1.56615e-23};//Mavi
	double Eriste[7] = {0.00360198, 1.24532e-05, 1.80371e-09, 6.74603e-10, 1.47702e-18, 1.40608e-12, -1.00958e-18};//Turuncu

	savedMoments[0] = YMercimek;
	savedMoments[1] = KMercimek;
	savedMoments[2] = Pirinc;
	savedMoments[3] = Fasulye;
	savedMoments[4] = Cekirdek;
	savedMoments[5] = Ceviz;
	savedMoments[6] = Eriste;

	int* objectTagArray = new int[objectCount];
	unsigned char** objectArray = new unsigned char*[objectCount];

	double** objectMoments = new double*[objectCount];
	for (int i = 0; i < objectCount; i++)
		objectMoments[i] = new double[7];

	int window[4];
	int windowWidth, windowHeight, windowPosition;
	std::ifstream in("ObjectRectangles.txt");

	for (int i = 0; i < objectCount; i++)
	{
		in >> window[0];
		in >> window[1];
		in >> window[2];
		in >> window[3];

		windowWidth = window[2] - window[0] + 1;
		windowHeight = window[3] - window[1] + 1;
		objectArray[i] = new unsigned char[windowWidth * windowHeight];

		windowPosition = 0;
		for (int row = window[1]; row <= window[3]; row++)
			for (int col = window[0]; col <= window[2]; col++)
			{
				int pos = row * width + col;
				objectArray[i][windowPosition] = (int)binaryImage[pos];
				windowPosition++;
			}

		myHuMoments* huMomentsObj = new myHuMoments(objectArray[i], windowWidth, windowHeight);
		objectMoments[i] = huMomentsObj->getHuMoments();
		huMomentsObj->~myHuMoments();
	}
	in.close();

	//std::ofstream out("ObjectMoments2222.txt");

	//double temp;
	//for (int i = 0; i < 7; i++)
	//{
	//	temp = 0;
	//	for (int j = 0; j < objectCount; j++)
	//		temp += objectMoments[j][i] / objectCount;
	//	out << temp << " ";
	//}
	//out.close();

	double distance;
	double distance2;
	for(int i = 0; i < objectCount; i++)
	{
		distance = 9999999999999999999;
		for (int j = 0; j < 7; j++)
		{
			distance2 = euklidean_Distance(objectMoments[i], savedMoments[j]);
			if (distance > distance2)
			{
				distance = distance2;
				objectTagArray[i] = j + 1;
			}
		}
	}

	for (int i = 0; i < width * height; i++)
		if (imageTagArray[i] != 0)
			recogImage[i] = objectTagArray[imageTagArray[i] - 1];

	Bitmap^ surface = gcnew Bitmap(width, height);

	for (int row = 0; row < height; row++)
		for (int col = 0; col < width; col++)
		{
			int pos = row * width + col;
			if (recogImage[pos] == 0)
				surface->SetPixel(col, row, Color::Black);
			else if (recogImage[pos] == 1)
				surface->SetPixel(col, row, Color::Green);
			else if (recogImage[pos] == 2)
				surface->SetPixel(col, row, Color::Red);
			else if (recogImage[pos] == 3)
				surface->SetPixel(col, row, Color::Yellow);
			else if (recogImage[pos] == 4)
				surface->SetPixel(col, row, Color::Purple);
			else if (recogImage[pos] == 5)
				surface->SetPixel(col, row, Color::Pink);
			else if (recogImage[pos] == 6)
				surface->SetPixel(col, row, Color::Blue);
			else if (recogImage[pos] == 7)
				surface->SetPixel(col, row, Color::Orange);
			else if (recogImage[pos] == 255)
				surface->SetPixel(col, row, Color::White);
		}
	pictureBox2->Image = surface;
}

//Canny
System::Void MyForm::button15_Click(System::Object^  sender, System::EventArgs^  e)
{
	BYTE* saveImage = new BYTE[width * height];
	int* tempSobelX = new int[width * height];
	int* sobelImage = new int[width * height];
	angleArray = new double[width * height];

	//Sobel matrix mask for X axis.
	int Gx[9] = { 1, 0, -1,
					2, 0, -2,
					1, 0, -1 };

	//Sobel matrix mask for Y axis.
	int Gy[9] = {	1, 2, 1,
					0, 0, 0,
					-1, -2, -1};

	//Applying the X axis sobel mask to the image
	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;

			tempSobelX[pos] = Gx[0] * rawintensity[pos - width - 1] + Gx[1] * rawintensity[pos - width] + Gx[2] * rawintensity[pos - width + 1] +
							Gx[3] * rawintensity[pos - 1] + Gx[4] * rawintensity[pos] + Gx[5] * rawintensity[pos + 1] +
							Gx[6] * rawintensity[pos + width - 1] + Gx[7] * rawintensity[pos + width] + Gx[8] * rawintensity[pos + width + 1];
		}

	//Save
	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;
			saveImage[pos] = tempSobelX[pos] / 4;
		}
	Save(saveImage, width, height, new_size);

	//Applying the Y axis sobel mask to the image
	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;

			sobelImage[pos] = Gy[0] * rawintensity[pos - width - 1] + Gy[1] * rawintensity[pos - width] + Gy[2] * rawintensity[pos - width + 1] +
				Gy[3] * rawintensity[pos - 1] + Gy[4] * rawintensity[pos] + Gy[5] * rawintensity[pos + 1] +
				Gy[6] * rawintensity[pos + width - 1] + Gy[7] * rawintensity[pos + width] + Gy[8] * rawintensity[pos + width + 1];
		}

	//Save
	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;
			saveImage[pos] = sobelImage[pos] / 4;
		}
	Save(saveImage, width, height, new_size);

	//Finding the angle of the gradient direction vector
	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;
			angleArray[pos] = atan2(sobelImage[pos] * 1.0 , tempSobelX[pos] * 1.0) * 180 / PI;
		}

	//Saving the gradiant image. Instead of root of the square of both values, we sum absolute values for process time
	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;

			sobelImage[pos] = Math::Abs(sobelImage[pos]) + Math::Abs(tempSobelX[pos]);
		}//Have the Sobel Image at this point

	//Continue onto canny from sobel image
	int* cannyImage = new int[width * height];

	//Save
	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;
			saveImage[pos] = sobelImage[pos] / 4;
			cannyImage[pos] = sobelImage[pos] / 4;
		}
	Save(saveImage, width, height, new_size);

	delete[] tempSobelX;
	delete[] sobelImage;

	//Non-maxima supression
	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;
			int i, j;
			if ((angleArray[pos] >= -22.5 && angleArray[pos] < 22.5) || (angleArray[pos] >= -180 && angleArray[pos] < -157.5) || ( angleArray[pos] >= 157.5 && angleArray[pos] <= 180))
			{
				i = 1; j = -1;
			}
			else if ((angleArray[pos] >= 22.5 && angleArray[pos] < 67.5) || (angleArray[pos] >= -157.5 && angleArray[pos] < -112.5))
			{
				i = -width + 1; j = width - 1;
			}
			else if ((angleArray[pos] >= 67.5 && angleArray[pos] < 112.5) || (angleArray[pos] >= -112.5 && angleArray[pos] < -67.5))
			{
				i = -width; j = width;
			}
			else if ((angleArray[pos] >= 112.5 && angleArray[pos] < 157.5) || (angleArray[pos] >= -67.5 && angleArray[pos] < -22.5))
			{
				i = -width - 1; j = width + 1;
			}

			//if (!((cannyImage[pos] > cannyImage[pos + i]) && (cannyImage[pos] > cannyImage[pos + j])))
			//	cannyImage[pos] = 0;

			if ((cannyImage[pos] <= cannyImage[pos + i]) || (cannyImage[pos] <= cannyImage[pos + j]))
				cannyImage[pos] = 0;
		}

	//Save
	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;
			saveImage[pos] = cannyImage[pos];
		}
	Save(saveImage, width, height, new_size);

	//Hysteresis Thresholding
	double T1 = 0, T2, T3;
	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;
			if (cannyImage[pos] > T1)
				T1 = cannyImage[pos];
		}

	T2 = T1 * 20 / 100;
	T3 = T1 * 60 / 100;

	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;

			if (cannyImage[pos] <= T2)
				cannyImage[pos] = 0;
			if (cannyImage[pos] >= T3)
				cannyImage[pos] = 255;
		}

	//Save
	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{			
			int pos = row * width + col;
			saveImage[pos] = cannyImage[pos];
		}
	Save(saveImage, width, height, new_size);

	//Clarify edges between thresholds(Continue Hyteresis Thresholding)
	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;
			int mask[9] = { -width - 1, -width, -width + 1,
							- 1, 0, 1,
							width - 1, width, width + 1 };

			int i, j;
			if ((angleArray[pos] >= -22.5 && angleArray[pos] < 22.5) || (angleArray[pos] >= -180 && angleArray[pos] < -157.5) || (angleArray[pos] >= 157.5 && angleArray[pos] <= 180))
			{
				i = mask[1]; j = mask[7];
			}
			else if ((angleArray[pos] >= 22.5 && angleArray[pos] < 67.5) || (angleArray[pos] >= -157.5 && angleArray[pos] < -112.5))
			{
				i = mask[0]; j = mask[8];
			}
			else if ((angleArray[pos] >= 67.5 && angleArray[pos] < 112.5) || (angleArray[pos] >= -112.5 && angleArray[pos] < -67.5))
			{
				i = mask[3]; j = mask[5];
			}
			else if ((angleArray[pos] >= 112.5 && angleArray[pos] < 157.5) || (angleArray[pos] >= -67.5 && angleArray[pos] < -22.5))
			{
				i = mask[2]; j = mask[6];
			}

			if (cannyImage[pos] != 0 && cannyImage[pos] != 255)
			{
				if (cannyImage[pos + i] == 255 && cannyImage[pos + j] == 255)
					cannyImage[pos] = 255;
				else if (cannyImage[pos + i] == 0 && cannyImage[pos + j] == 0)
					cannyImage[pos] = 0;
			}
			else if (cannyImage[pos] == 255)
			{
				if (cannyImage[pos + i] != 0)
					cannyImage[pos + i] = 255;
				if (cannyImage[pos + j] != 0)
					cannyImage[pos + j] = 255;
			}
		}

	//Save
	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;
			saveImage[pos] = cannyImage[pos];
		}
	Save(saveImage, width, height, new_size);

	//Convert angleArray from gradient angle to edge angle
	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;

			angleArray[pos] += 90;

			if (angleArray[pos] > 360)
				angleArray[pos] -= 360;
		}

	//Hough Transform
	int dMax = width * height / 100; //dMax = round(sqrt(pow(width, 2) + pow(height, 2)))
	int* houghSpace = new int[dMax * 360];

	for (int row = 0; row < dMax; row++)
		for (int col = 0; col < 360; col++)
		{
			int pos = row * 360 + col;
			houghSpace[pos] = 0;
		}

	int errorT = 4;
	int d, houghIndex;

	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;
			
			if (cannyImage[pos] == 255)
			{
				if (angleArray[pos] < 0)
					angleArray[pos] += 360;

				if ((angleArray[pos] > (270 - errorT) && angleArray[pos] < (270 + errorT)) || (angleArray[pos] > (90 - errorT) && angleArray[pos] < (90 + errorT)))
					angleArray[pos] = 90;
				if ((angleArray[pos] > (180 - errorT) && angleArray[pos] < (180 + errorT)) || (angleArray[pos] < (0 + errorT)) || (angleArray[pos] > (360 - errorT)))
					angleArray[pos] = 0;



				//if (round(angleArray[pos]) == 270 || round(angleArray[pos]) == 90)
				//{
				//	angleArray[pos] = 90;
				//	//System::Windows::Forms::MessageBox::Show("KENAN KOMUTANIM");
				//}
				//	
				//if (round(angleArray[pos]) == 0 || round(angleArray[pos]) == 180 || round(angleArray[pos]) == 360)
				//{
				//	angleArray[pos] = 0;
				//	//System::Windows::Forms::MessageBox::Show("Efendim asker");
				//}



				if (angleArray[pos] == 0 || angleArray[pos] == 90)
				{
					d = Math::Abs(row * round(sin(angleArray[pos])) + col * round(cos(angleArray[pos])));
					houghIndex = d * 360 + angleArray[pos];
					//System::Windows::Forms::MessageBox::Show(houghIndex + " ");
					houghSpace[houghIndex] +=  1;
				}
			}
		}
	
	//Save hough Space representation
	BYTE* houghSave = new BYTE[dMax * 360];

	for (int row = 0; row < dMax; row++)
		for (int col = 0; col < 360; col++)
		{
			int pos = row * 360 + col;

			if (houghSpace[pos] > 5)
				houghSave[pos] = 255;
			else
				houghSave[pos] = 0;
		}
	Save(houghSave, dMax, 360, new_size);

	//Finding the Max 16 values in hough space
	int maxHough[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 };
	int dHough[16];
	int angHough[16];
	int max = 0;
	int dHoughMax;
	int angHoughMax;

	for (int row = 0; row < dMax; row++)
		for (int col = 0; col < 360; col++)
		{
			int pos = row * 360 + col;

			if (houghSpace[pos] > max)
			{
				max = houghSpace[pos];
				dHoughMax = row;
				angHoughMax = col;
			}
		}

	maxHough[0] = max;
	dHough[0] = dHoughMax;
	angHough[0] = angHoughMax;

	for (int k = 1; k < 16; k++)
	{
		max = 0;
		for (int row = 0; row < dMax; row++)
			for (int col = 0; col < 360; col++)
			{
				int pos = row * 360 + col;

				if (houghSpace[pos] > max && houghSpace[pos] < maxHough[k-1])
				{
					max = houghSpace[pos];
					dHoughMax = row;
					angHoughMax = col;
				}
			}
		maxHough[k] = max;
		dHough[k] = dHoughMax;
		angHough[k] = angHoughMax;
	}//Found the values

	//System::Windows::Forms::MessageBox::Show(dHough[0] + " " + dHough[1] + " " + dHough[2] + " " + dHough[3] + " " + dHough[4] + " " + dHough[5] + " " + dHough[6] + " " + dHough[7] + " " + dHough[8] + " " + dHough[9] + " " + dHough[10]  +" " + dHough[11] +" " + dHough[12] + " " + dHough[13] + " " + dHough[14] +" " + dHough[15]  +" ");
	int r[16], c[16];

	for (int i = 0; i < 16; i++)
	{
		r[i] = 9999999;
		c[i] = 9999999;
	}

	for (int k = 0; k < 16; k++)
	{
		if (angHough[k] == 0)
			r[k] = dHough[k];
			
		else if (angHough[k] == 90)
			c[k] = dHough[k];
	}

	for (int row = 1; row < height - 1; row++)
		for (int col = 1; col < width - 1; col++)
		{
			int pos = row * width + col;
			if (r[0] == col || r[1] == col || r[2] == col || r[3] == col || r[4] == col || r[5] == col || r[6] == col || r[7] == col || c[0] == row || c[1] == row || c[2] == row || c[3] == row || c[4] == row || c[5] == row || c[6] == row || c[7] == row
				|| r[8] == col || r[9] == col || r[10] == col || r[11] == col || r[12] == col || r[13] == col || r[14] == col || r[15] == col || c[8] == row || c[9] == row || c[10] == row || c[11] == row || c[12] == row || c[13] == row || c[14] == row || c[15] == row)
			{
				saveImage[pos] = 255;
			}
			else
				saveImage[pos] = cannyImage[pos];
		}

	Save(saveImage, width, height, new_size);
	pictureBox2->ImageLocation = ("C://Users//Nahrime//source//repos//Image_Processing//Image_Processing//Images//Outs//myImage" + (printCount - 1) + ".bmp");
}

//Cipher
System::Void MyForm::button7_Click(System::Object^  sender, System::EventArgs^  e)
{
	int* dizi = new int[size];
	srand(time(0));
	///RANDOM MATRÝS DOLDURMA
	for (int i = 0; i < size; i++) {
		dizi[i] = (int)image[i];
	}
	//cout << endl << endl;
	int Matrix[16][16];

	bool q = false;
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			Matrix[i][j] = -1;
		}
	}
	for (int a = 0; a < 256; a++) {
		while (true)
		{

			int x = rand() % 16;
			int y = rand() % 16;

			if (Matrix[x][y] == -1)
			{
				Matrix[x][y] = a;
				break;

			}

		}
	}

	//for (int i = 0; i < 16; i++) {
	//	for (int j = 0; j < 16; j++) {
	//		//MessageBox::Show(" " + Matrix[i][j] + " ");
	//	}
	//}
	////////////////////////////////////////////////////PADDÝNG VE SIRALAMA HENÜZ BÝTMEDÝ////
	int k = 0;
	int indis1 = -1;
	int indis2 = -1;
	int indis3 = -1;
	int indis4 = -1;
	while (k < size)
	{

		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				if (Matrix[i][j] == dizi[k]) {
					indis1 = i;
					indis2 = j;
				}
				if (Matrix[i][j] == dizi[k + 1]) {
					indis3 = i;
					indis4 = j;
				}
				else if (indis1 != -1 && indis3 != -1) {
					if ((indis1 == indis3 && indis2 == indis4) || (indis1 == indis3 && indis2 != indis4))
					{
						dizi[k] = Matrix[indis1][(indis2 + 1) % 16];
						dizi[k + 1] = Matrix[indis3][(indis4 + 1) % 16];
						k += 2;
						indis1 = -1;
						indis2 = -1;
						indis3 = -1;
						indis4 = -1;
					}
					else if (indis2 == indis4 && indis1 != indis3) {///
						dizi[k] = Matrix[(indis1 + 1) % 16][indis2];
						dizi[k + 1] = Matrix[(indis3 + 1) % 16][indis4];
						k += 2;
						indis1 = -1;
						indis2 = -1;
						indis3 = -1;
						indis4 = -1;
					}
					else {///
						dizi[k] = Matrix[indis3][indis2];
						dizi[k + 1] = Matrix[indis1][indis4];
						k += 2;
						indis1 = -1;
						indis2 = -1;
						indis3 = -1;
						indis4 = -1;
					}
				}
			}
		}
	}


	//cout << endl << endl;
	//for (int i = 0; i < 10; i++) {
	//	cout << dizi[i] << " ";
	//}
	//Decipher
	while (k < size)
	{

		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				if (Matrix[i][j] == dizi[k]) {
					indis1 = i;
					indis2 = j;
				}
				if (Matrix[i][j] == dizi[k + 1]) {
					indis3 = i;
					indis4 = j;
				}
				else if (indis1 != -1 && indis3 != -1) {
					if ((indis1 == indis3 && indis2 == indis4) || (indis1 == indis3 && indis2 != indis4))
					{
						dizi[k] = Matrix[indis1][(indis2 - 1) % 16];
						dizi[k + 1] = Matrix[indis3][(indis4 - 1) % 16];
						k += 2;
						indis1 = -1;
						indis2 = -1;
						indis3 = -1;
						indis4 = -1;
					}
					else if (indis2 == indis4 && indis1 != indis3) {///
						dizi[k] = Matrix[(indis1 - 1) % 16][indis2];
						dizi[k + 1] = Matrix[(indis3 - 1) % 16][indis4];
						k += 2;
						indis1 = -1;
						indis2 = -1;
						indis3 = -1;
						indis4 = -1;
					}
					else {///
						dizi[k] = Matrix[indis3][indis2];
						dizi[k + 1] = Matrix[indis1][indis4];
						k += 2;
						indis1 = -1;
						indis2 = -1;
						indis3 = -1;
						indis4 = -1;
					}
				}
			}
		}
	}

	int padding = 0;
	int scanlinebytes = width * 3;
	while ((scanlinebytes + padding) % 4 != 0)     // DWORD = 4 bytes
		padding++;
	// get the padded scanline width
	int psw = scanlinebytes + padding;


	surface = gcnew Bitmap(psw, height);
	Color pixelColor;

	for (int row = 0; row < height; row++)
		for (int col = 0; col < width; col++)
		{
			int pos = (row * width + col) * 3;
			pixelColor = Color::FromArgb(dizi[pos], dizi[pos + 1], dizi[pos + 2]);
			surface->SetPixel(col, row, pixelColor);
		}

	pictureBox2->Image = surface;
}

//Save
Void MyForm::Save(BYTE* Buffer, int widthf, int heightf, long ssize)
{
	BYTE* printBuffer = ConvertIntensityToBMP(Buffer, widthf, heightf, ssize);
	CString Location = "Images//Outs//myImage" + printCount + ".bmp";
	output = Location;
	if (!SaveBMP(printBuffer, widthf, heightf, ssize, output))
		System::Windows::Forms::MessageBox::Show("Error saving image.");
	printCount++;
	delete[] printBuffer;
}

//Save Picture Box 2
System::Void MyForm::button3_Click(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
{
	//BYTE* item = ConvertIntensityToBMP(processedImage, imageWidth, imageHeight, newSize);
	//CString Location = "Images//Outs//Out" + processCount + ".bmp";
	//output = Location;
	//if (SaveBMP(item, imageWidth, imageHeight, newSize, output))
	//	System::Windows::Forms::MessageBox::Show("Output Image was successfully saved");
	//else
	//	System::Windows::Forms::MessageBox::Show("Error on saving image");

	//delete[] item;
}

[STAThreadAttribute]

int main()
{
	Windows::Forms::Application::EnableVisualStyles();
	Windows::Forms::Application::Run(gcnew ImageProcessing::MyForm());
	return 0;
}
