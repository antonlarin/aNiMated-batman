#pragma once
#include "D_TestTask.h"
#include "Solvers.h"
namespace DTestTask {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainWindow
	/// </summary>
	public ref class MainWindow : public System::Windows::Forms::Form
	{
	public:
		MainWindow(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::RadioButton^  uRelaxRadioButton;
	protected:

	private: System::Windows::Forms::RadioButton^  zeidelRadioButton;

	private: System::Windows::Forms::RadioButton^  jacobyRadioButton;

	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::GroupBox^  groupBox3;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::NumericUpDown^  dimYNumericUpDown;
	private: System::Windows::Forms::NumericUpDown^  dimXNumericUpDown;
	private: System::Windows::Forms::NumericUpDown^  nmaxNumericUpDown;
	private: System::Windows::Forms::TextBox^  epsTextBox;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  omegaTextBox;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::DataGridView^  solutionGridView;


	private: System::Windows::Forms::Button^  solveButton;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  reachedEpsLabel;
	private: System::Windows::Forms::Label^  itNumLabel;
	private: System::Windows::Forms::Label^  solutionEpsLlabel;
	private: System::Windows::Forms::Label^  label9;






	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->uRelaxRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->zeidelRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->jacobyRadioButton = (gcnew System::Windows::Forms::RadioButton());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->reachedEpsLabel = (gcnew System::Windows::Forms::Label());
			this->itNumLabel = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->solveButton = (gcnew System::Windows::Forms::Button());
			this->groupBox3 = (gcnew System::Windows::Forms::GroupBox());
			this->dimYNumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->dimXNumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->nmaxNumericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->epsTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->omegaTextBox = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->solutionGridView = (gcnew System::Windows::Forms::DataGridView());
			this->solutionEpsLlabel = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->groupBox1->SuspendLayout();
			this->tableLayoutPanel1->SuspendLayout();
			this->panel1->SuspendLayout();
			this->groupBox3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dimYNumericUpDown))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dimXNumericUpDown))->BeginInit();
			this->groupBox2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nmaxNumericUpDown))->BeginInit();
			this->panel2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->solutionGridView))->BeginInit();
			this->SuspendLayout();
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->uRelaxRadioButton);
			this->groupBox1->Controls->Add(this->zeidelRadioButton);
			this->groupBox1->Controls->Add(this->jacobyRadioButton);
			this->groupBox1->Location = System::Drawing::Point(9, 9);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(220, 87);
			this->groupBox1->TabIndex = 0;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Метод решения разностной схемы";
			// 
			// uRelaxRadioButton
			// 
			this->uRelaxRadioButton->AutoSize = true;
			this->uRelaxRadioButton->Location = System::Drawing::Point(3, 63);
			this->uRelaxRadioButton->Name = L"uRelaxRadioButton";
			this->uRelaxRadioButton->Size = System::Drawing::Size(164, 17);
			this->uRelaxRadioButton->TabIndex = 2;
			this->uRelaxRadioButton->Text = L"Метод верхней релаксации";
			this->uRelaxRadioButton->UseVisualStyleBackColor = true;
			// 
			// zeidelRadioButton
			// 
			this->zeidelRadioButton->AutoSize = true;
			this->zeidelRadioButton->Location = System::Drawing::Point(3, 40);
			this->zeidelRadioButton->Name = L"zeidelRadioButton";
			this->zeidelRadioButton->Size = System::Drawing::Size(103, 17);
			this->zeidelRadioButton->TabIndex = 1;
			this->zeidelRadioButton->Text = L"Метод Зейделя";
			this->zeidelRadioButton->UseVisualStyleBackColor = true;
			// 
			// jacobyRadioButton
			// 
			this->jacobyRadioButton->AutoSize = true;
			this->jacobyRadioButton->Checked = true;
			this->jacobyRadioButton->Location = System::Drawing::Point(3, 16);
			this->jacobyRadioButton->Name = L"jacobyRadioButton";
			this->jacobyRadioButton->Size = System::Drawing::Size(92, 17);
			this->jacobyRadioButton->TabIndex = 0;
			this->jacobyRadioButton->TabStop = true;
			this->jacobyRadioButton->Text = L"Метод Якоби";
			this->jacobyRadioButton->UseVisualStyleBackColor = true;
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 2;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Absolute,
				250)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
				100)));
			this->tableLayoutPanel1->Controls->Add(this->panel1, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->panel2, 1, 0);
			this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 1;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 100)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(838, 443);
			this->tableLayoutPanel1->TabIndex = 1;
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->solutionEpsLlabel);
			this->panel1->Controls->Add(this->label9);
			this->panel1->Controls->Add(this->reachedEpsLabel);
			this->panel1->Controls->Add(this->itNumLabel);
			this->panel1->Controls->Add(this->label7);
			this->panel1->Controls->Add(this->label6);
			this->panel1->Controls->Add(this->solveButton);
			this->panel1->Controls->Add(this->groupBox3);
			this->panel1->Controls->Add(this->groupBox2);
			this->panel1->Controls->Add(this->groupBox1);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel1->Location = System::Drawing::Point(3, 3);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(244, 437);
			this->panel1->TabIndex = 1;
			// 
			// reachedEpsLabel
			// 
			this->reachedEpsLabel->AutoSize = true;
			this->reachedEpsLabel->Location = System::Drawing::Point(135, 404);
			this->reachedEpsLabel->Name = L"reachedEpsLabel";
			this->reachedEpsLabel->Size = System::Drawing::Size(10, 13);
			this->reachedEpsLabel->TabIndex = 9;
			this->reachedEpsLabel->Text = L"-";
			// 
			// itNumLabel
			// 
			this->itNumLabel->AutoSize = true;
			this->itNumLabel->Location = System::Drawing::Point(131, 380);
			this->itNumLabel->Name = L"itNumLabel";
			this->itNumLabel->Size = System::Drawing::Size(10, 13);
			this->itNumLabel->TabIndex = 8;
			this->itNumLabel->Text = L"-";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(9, 404);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(123, 13);
			this->label7->TabIndex = 7;
			this->label7->Text = L"Достигнутая точность:";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(9, 380);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(113, 13);
			this->label6->TabIndex = 6;
			this->label6->Text = L"Затрачено итераций:";
			// 
			// solveButton
			// 
			this->solveButton->Location = System::Drawing::Point(12, 287);
			this->solveButton->Name = L"solveButton";
			this->solveButton->Size = System::Drawing::Size(75, 23);
			this->solveButton->TabIndex = 5;
			this->solveButton->Text = L"Решить";
			this->solveButton->UseVisualStyleBackColor = true;
			this->solveButton->Click += gcnew System::EventHandler(this, &MainWindow::solveButton_Click);
			// 
			// groupBox3
			// 
			this->groupBox3->Controls->Add(this->dimYNumericUpDown);
			this->groupBox3->Controls->Add(this->dimXNumericUpDown);
			this->groupBox3->Controls->Add(this->label2);
			this->groupBox3->Controls->Add(this->label1);
			this->groupBox3->Location = System::Drawing::Point(12, 103);
			this->groupBox3->Name = L"groupBox3";
			this->groupBox3->Size = System::Drawing::Size(217, 70);
			this->groupBox3->TabIndex = 4;
			this->groupBox3->TabStop = false;
			this->groupBox3->Text = L"Параметры разностной схемы";
			// 
			// dimYNumericUpDown
			// 
			this->dimYNumericUpDown->Location = System::Drawing::Point(149, 41);
			this->dimYNumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10000, 0, 0, 0 });
			this->dimYNumericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3, 0, 0, 0 });
			this->dimYNumericUpDown->Name = L"dimYNumericUpDown";
			this->dimYNumericUpDown->Size = System::Drawing::Size(62, 20);
			this->dimYNumericUpDown->TabIndex = 3;
			this->dimYNumericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3, 0, 0, 0 });
			// 
			// dimXNumericUpDown
			// 
			this->dimXNumericUpDown->Location = System::Drawing::Point(149, 15);
			this->dimXNumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10000, 0, 0, 0 });
			this->dimXNumericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3, 0, 0, 0 });
			this->dimXNumericUpDown->Name = L"dimXNumericUpDown";
			this->dimXNumericUpDown->Size = System::Drawing::Size(62, 20);
			this->dimXNumericUpDown->TabIndex = 2;
			this->dimXNumericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3, 0, 0, 0 });
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(7, 43);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(135, 13);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Размерность сетки по Y:";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(7, 20);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(135, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Размерность сетки по X:";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->nmaxNumericUpDown);
			this->groupBox2->Controls->Add(this->epsTextBox);
			this->groupBox2->Controls->Add(this->label5);
			this->groupBox2->Controls->Add(this->label4);
			this->groupBox2->Controls->Add(this->omegaTextBox);
			this->groupBox2->Controls->Add(this->label3);
			this->groupBox2->Location = System::Drawing::Point(12, 179);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(217, 93);
			this->groupBox2->TabIndex = 3;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Параметры метода";
			// 
			// nmaxNumericUpDown
			// 
			this->nmaxNumericUpDown->Location = System::Drawing::Point(144, 14);
			this->nmaxNumericUpDown->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 100000, 0, 0, 0 });
			this->nmaxNumericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->nmaxNumericUpDown->Name = L"nmaxNumericUpDown";
			this->nmaxNumericUpDown->Size = System::Drawing::Size(67, 20);
			this->nmaxNumericUpDown->TabIndex = 6;
			this->nmaxNumericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 3, 0, 0, 0 });
			// 
			// epsTextBox
			// 
			this->epsTextBox->Location = System::Drawing::Point(142, 40);
			this->epsTextBox->Name = L"epsTextBox";
			this->epsTextBox->Size = System::Drawing::Size(69, 20);
			this->epsTextBox->TabIndex = 5;
			this->epsTextBox->Text = L"1e-12";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(6, 39);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(57, 13);
			this->label5->TabIndex = 4;
			this->label5->Text = L"Точность:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(7, 16);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(106, 13);
			this->label4->TabIndex = 3;
			this->label4->Text = L"Макс. число шагов:";
			// 
			// omegaTextBox
			// 
			this->omegaTextBox->Location = System::Drawing::Point(142, 64);
			this->omegaTextBox->Name = L"omegaTextBox";
			this->omegaTextBox->Size = System::Drawing::Size(69, 20);
			this->omegaTextBox->TabIndex = 2;
			this->omegaTextBox->Text = L"1.5";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(7, 66);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(44, 13);
			this->label3->TabIndex = 1;
			this->label3->Text = L"Omega:";
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->solutionGridView);
			this->panel2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel2->Location = System::Drawing::Point(253, 3);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(582, 437);
			this->panel2->TabIndex = 2;
			// 
			// solutionGridView
			// 
			this->solutionGridView->AllowUserToAddRows = false;
			this->solutionGridView->AllowUserToDeleteRows = false;
			this->solutionGridView->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->solutionGridView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->solutionGridView->Location = System::Drawing::Point(0, 0);
			this->solutionGridView->Name = L"solutionGridView";
			this->solutionGridView->ReadOnly = true;
			this->solutionGridView->RowHeadersWidth = 80;
			this->solutionGridView->Size = System::Drawing::Size(582, 437);
			this->solutionGridView->TabIndex = 0;
			// 
			// solutionEpsLlabel
			// 
			this->solutionEpsLlabel->AutoSize = true;
			this->solutionEpsLlabel->Location = System::Drawing::Point(131, 356);
			this->solutionEpsLlabel->Name = L"solutionEpsLlabel";
			this->solutionEpsLlabel->Size = System::Drawing::Size(10, 13);
			this->solutionEpsLlabel->TabIndex = 11;
			this->solutionEpsLlabel->Text = L"-";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(9, 356);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(104, 13);
			this->label9->TabIndex = 10;
			this->label9->Text = L"Точность решения:";
			// 
			// MainWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(838, 443);
			this->Controls->Add(this->tableLayoutPanel1);
			this->MinimumSize = System::Drawing::Size(854, 482);
			this->Name = L"MainWindow";
			this->Text = L"Контрольная работа №1";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->tableLayoutPanel1->ResumeLayout(false);
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->groupBox3->ResumeLayout(false);
			this->groupBox3->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dimYNumericUpDown))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dimXNumericUpDown))->EndInit();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->nmaxNumericUpDown))->EndInit();
			this->panel2->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->solutionGridView))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion

private: System::Void solveButton_Click(System::Object^  sender, System::EventArgs^  e) {

	int xDim = Convert::ToInt32(dimXNumericUpDown->Value);
	int yDim = Convert::ToInt32(dimYNumericUpDown->Value);
	int nMax = Convert::ToInt32(nmaxNumericUpDown->Value);
	double eps = Convert::ToDouble(epsTextBox->Text);
	double reachedEps=0, solutionEps=0;
	int iterationsNumber=0;
	D_TestTask task;
	D_TaskStatus stat;
	task.SetGridDimentions(xDim, yDim, &stat);

	if (jacobyRadioButton->Checked == true)
	{
		SolveWithJacoby(&task, nMax, eps, reachedEps, iterationsNumber, stat);
	}
	else if (zeidelRadioButton->Checked == true)
	{
		SolveWithZeidel(&task, nMax, eps, reachedEps, iterationsNumber, stat);
	}
	else
	{
		double omega = Convert::ToDouble(omegaTextBox->Text->Replace(".", ","));
		SolveWithURelax(&task, nMax, eps, reachedEps, iterationsNumber, omega, stat);
	}

	itNumLabel->Text = iterationsNumber.ToString();
	reachedEpsLabel->Text = reachedEps.ToString("E4");

	this->solutionGridView->Columns->Clear();
	array<System::Windows::Forms::DataGridViewTextBoxColumn^>^ columns = gcnew array<System::Windows::Forms::DataGridViewTextBoxColumn^>(xDim+1);
	array<System::String^>^ cellValues = gcnew array<System::String^>(xDim + 1);

	double* grid = task.GetGridPtr(&stat);
	double x, h = 2.0 / xDim;
	double y, k = 2.0 / yDim;
	for (int i = 0; i <= xDim; i++)
	{
		x = 1 - i*h;
		columns[i] = gcnew System::Windows::Forms::DataGridViewTextBoxColumn();
		columns[i]->Width = 60;
		columns[i]->ReadOnly = true;
		columns[i]->Name = i.ToString();
		columns[i]->HeaderText = x.ToString("F3");
		cellValues[i] = gcnew System::String("");
	}

	solutionGridView->Columns->AddRange(columns);

	for (int j = 0; j <= yDim; j++)
	{
		y = 1 - j*k;
		System::Windows::Forms::DataGridViewRow ^ tmpRow = gcnew System::Windows::Forms::DataGridViewRow;
		tmpRow->HeaderCell->Value = y.ToString("F3");

		for (int i = 0; i <= xDim; i++)
		{
			x = 1 - i*h;
			if (Math::Abs(grid[i + (xDim + 1)*(yDim-j)] - (1 - x*x - y*y)) > solutionEps)
				solutionEps = Math::Abs(grid[i + (xDim + 1)*(yDim - j)] - (1 - x*x - y*y));
			cellValues[i] = grid[i + (xDim + 1)*(yDim - j)].ToString("F5");
		}

		tmpRow->CreateCells(solutionGridView, cellValues);
		solutionGridView->Rows->Add(tmpRow);
	}
	solutionEpsLlabel->Text = solutionEps.ToString("E4");
}
};
}
