#pragma once
#include "fstream"
#include "iostream"
#include "math.h"

namespace tds
{
	class NNetwork
	{
	public:
		NNetwork()
		{
			Clear = true;
		}
		NNetwork(int Neurons[], int Layers)
		{
			this->Layers = Layers; // ������������ ���������� �����
			this->Neurons = new int[Layers]; // ������������ ������� � ����������� �������� �� ������ ����

			for (int l = 0; l < Layers; l++)
			{
				this->Neurons[l] = Neurons[l];
			}

			// �������� ������� ������ 3� ������� ����� �����, ���������� ����� ��� ���������� ����������� ����� ������ "Layers - 1" :
			Weights = new double** [Layers - 1];

			/* �������� ������� ������ 2� ������� ����� ����� �������� ��������,
			���������� ����� ��� ���������� ����������� ����� ������ "Layers - 1" : */
			BNWeights = new double* [Layers - 1];


			for (int i = 0; i < Layers - 1; i++) // ���� i �� ������� �� ���������� ���������� ����� ������
			{
				int first = Neurons[i]; // ���������� �������� ������ �� ���������� ����, "i"
				int second = Neurons[i + 1]; // ���������� �������� ������� �� ���������� ����, "i + 1"

				/* �������� ������� ������ 3� ������� ������� �����,
					���������� ����� ��� ���������� �������� ������ �� ���������� ���� "first" : */
				Weights[i] = new double* [first];

				/* �������� ������� ������ 2� ������� ������� ����� �������� ��������,
					���������� ����� ��� ���������� �������� ������ �� ���������� ���� "first" : */
				BNWeights[i] = new double[second];

				for (int s = 0; s < second; s++) // ���� s �� ������� �� ���������� ������� ������� �� ���������� ����
					// ������������ ���������� ���� ���������� i, ������� �������� � ������� �� ������� ���� s :
					BNWeights[i][s] = Rand();


				for (int f = 0; f < first; f++) // ���� f �� ������� �� ���������� ������� ������ �� ���������� ����
				{
					/* �������� �������� ������ 3� ������� ������� �����,
					���������� ����� ��� ���������� �������� ������� �� ���������� ���� "second" : */
					Weights[i][f] = new double[second];

					for (int s = 0; s < second; s++) // ���� s �� ������� �� ���������� ������� ������� �� ���������� ����
						// ������������ ���������� ���� ���������� i, ������� �� ������ ���� f � ������� �� ������� ���� s :
						Weights[i][f][s] = Rand();
				}
			}

			// �������� 2� ������ �������� �������� �������� "Outputs" � ������ �������� "Errors" :
			Outputs = new double* [Layers];
			Errors = new double* [Layers];

			for (int i = 0; i < Layers; i++) // ���� i �� ������� �� ���������� ����
			{
				Outputs[i] = new double[Neurons[i]]; // �������� ���������� �������� �������� ��� ������� ����
				Errors[i] = new double[Neurons[i]]; // �������� ���������� ������ ��� ������� ����
			}

			Clear = false;

		}
		NNetwork(const NNetwork& other)
		{
			this->Layers = other.Layers; // ������������ ���������� �����
			this->Neurons = new int[Layers]; // ������������ ������� � ����������� �������� �� ������ ����

			for (int l = 0; l < Layers; l++)
			{
				this->Neurons[l] = other.Neurons[l];
			}

			// �������� ������� ������ 3� ������� ����� �����, ���������� ����� ��� ���������� ����������� ����� ������ "Layers - 1" :
			Weights = new double** [Layers - 1];

			/* �������� ������� ������ 2� ������� ����� ����� �������� ��������,
			���������� ����� ��� ���������� ����������� ����� ������ "Layers - 1" : */
			BNWeights = new double* [Layers - 1];


			for (int i = 0; i < Layers - 1; i++) // ���� i �� ������� �� ���������� ���������� ����� ������
			{
				int first = Neurons[i]; // ���������� �������� ������ �� ���������� ����, "i"
				int second = Neurons[i + 1]; // ���������� �������� ������� �� ���������� ����, "i + 1"

				/* �������� ������� ������ 3� ������� ������� �����,
					���������� ����� ��� ���������� �������� ������ �� ���������� ���� "first" : */
				Weights[i] = new double* [first];

				/* �������� ������� ������ 2� ������� ������� ����� �������� ��������,
					���������� ����� ��� ���������� �������� ������ �� ���������� ���� "first" : */
				BNWeights[i] = new double[second];

				for (int s = 0; s < second; s++) // ���� s �� ������� �� ���������� ������� ������� �� ���������� ����
					// ������������ ���� ���������� i, ������� �������� � ������� �� ������� ���� s :
					BNWeights[i][s] = other.BNWeights[i][s];


				for (int f = 0; f < first; f++) // ���� f �� ������� �� ���������� ������� ������ �� ���������� ����
				{
					/* �������� �������� ������ 3� ������� ������� �����,
					���������� ����� ��� ���������� �������� ������� �� ���������� ���� "second" : */
					Weights[i][f] = new double[second];

					for (int s = 0; s < second; s++) // ���� s �� ������� �� ���������� ������� ������� �� ���������� ����
						// ������������ ���� ���������� i, ������� �� ������ ���� f � ������� �� ������� ���� s :
						Weights[i][f][s] = other.Weights[i][f][s];
				}
			}

			// �������� 2� ������ �������� �������� �������� "Outputs" � ������ �������� "Errors" :
			Outputs = new double* [Layers];
			Errors = new double* [Layers];

			for (int i = 0; i < Layers; i++) // ���� i �� ������� �� ���������� ����
			{
				Outputs[i] = new double[Neurons[i]]; // �������� ���������� �������� �������� ��� ������� ����
				Errors[i] = new double[Neurons[i]]; // �������� ���������� ������ ��� ������� ����
			}

			LearnQ = other.LearnQ;
			AFunc = other.AFunc;
			Clear = false;
		}
		~NNetwork()
		{
			Clean();
		}
		void Work(double Inputs[])
		{
			for (int n = 0; n < Neurons[0]; n++) // ���� �� ������� �� ���������� ������� ������� ����
				Outputs[0][n] = Inputs[n]; // ������������ �������� �������� �������� ������� ����


			for (int i = 0; i < Layers - 1; i++) // ���� i �� ������� �� ���������� ���������� ����� ������
			{
				for (int s = 0; s < Neurons[i + 1]; s++) // ���� s �� ������� �� ���������� ������� ������� �� ���������� ����
				{
					/* ���������� ����� ������������ �������� �������� �� �������
					����������� ������� �� ���� ����� � ����� ��������� : */
					double Sum = 0;

					for (int f = 0; f < Neurons[i]; f++) // ���� f �� ������� �� ���������� ������� ������ �� ���������� ����
					{
						/* ����������� ���������� ������������ ��������� �������
						����������� ������� �� ��� ����� � ���� �������� : */
						Sum += Outputs[i][f] * Weights[i][f][s];
					}

					/* ����������� ���������� ������������ ��������� �������
						������� �������� �� ��� ����� � ���� �������� : */
					Sum += 1 * BNWeights[i][s];

					/* ������� ����� ����� ������� ��������� � ������������
					���������� � ������ ������� �� ���������� ���� : */
					Outputs[i + 1][s] = AFunc(Sum, false);
				}
			}

		}
		void Errs(double TrueOutputs[])
		{
			for (int n = 0; n < Neurons[Layers - 1]; n++) // // ���� �� ������� �� ���������� ������� ���������� ����
				Errors[Layers - 1][n] = TrueOutputs[n] - Outputs[Layers - 1][n]; // ������������ ������ �������� ���������� ����

			for (int i = Layers - 2; i > 0; i--) // ���� i �� ���������� ���������� �� �������
			{
				for (int f = 0; f < Neurons[i]; f++) // ���� f �� ������� �� ���������� ������� ������ �� ���������� ����
				{
					double Error = 0; // ���������� ������ ��� ������� ������� ������ �� ���������� ����
					for (int s = 0; s < Neurons[i + 1]; s++) // ���� s �� ������� �� ���������� ������� ������� �� ���������� ����
					{
						/* ����������� � ���������� ������ ��������� ������������ ������ ������� �������
						�� ���������� ���� �� ��� ����� � ���� �������� : */
						Error += Errors[i + 1][s] * Weights[i][f][s];
					}
					Errors[i][f] = Error; // ������������ ������ ������� ������ �� ���������� ����
				}
			}
		}
		void Learn()
		{
			for (int i = 0; i < Layers - 1; i++)  // ���� i �� ������� �� ���������� ���������� ����� ������
			{
				for (int s = 0; s < Neurons[i + 1]; s++) // ���� s �� ������� �� ���������� ������� ������� �� ���������� ����
				{
					for (int f = 0; f < Neurons[i]; f++) // ���� f �� ������� �� ���������� ������� ������ �� ���������� ����
					{
						/* ���������� ����� ����� ������� �� ���������� �������
							� ����� �� ���������� �������� : */
						Weights[i][f][s] += LearnQ * Errors[i + 1][s] * AFunc(Outputs[i + 1][s], true) * Outputs[i][f];
					}

					/* ���������� ����� ����� ������� �� ���������� �������
							� ������� �������� : */
					BNWeights[i][s] += LearnQ * Errors[i + 1][s] * AFunc(Outputs[i + 1][s], true);
				}
			}
		}
		void GetOut(double Out[])
		{
			for (int n = 0; n < Neurons[Layers - 1]; n++)
				Out[n] = Outputs[Layers - 1][n];
		}
		void GetErrs(double Errs[])
		{
			for (int n = 0; n < Neurons[Layers - 1]; n++)
				Errs[n] = Errors[Layers - 1][n];
		}
		void SetLearnQ(double LearnQ)
		{
			this->LearnQ = LearnQ;
		}
		void SetMath(double (*AFunc)(double, bool))
		{
			this->AFunc = AFunc;
		}
		bool Save(std::string path)
		{
			path += ".net";
			File.open(path);
			File.close();

			File.open(path, std::fstream::app | std::fstream::binary | std::fstream::out);

			if (File.is_open())
			{
				File.write((char*)&Layers, sizeof(int));

				for (int l = 0; l < Layers; l++)
					File.write((char*)&Neurons[l], sizeof(int));

				for (int i = 0; i < Layers - 1; i++)
				{
					int first = Neurons[i];
					int second = Neurons[i + 1];

					for (int f = 0; f < first; f++)
					{
						for (int s = 0; s < second; s++)
						{
							File.write((char*)&Weights[i][f][s], sizeof(double));
						}
					}
				}

				for (int i = 0; i < Layers - 1; i++)
				{
					int second = Neurons[i + 1];

					for (int s = 0; s < second; s++)
						File.write((char*)&BNWeights[i][s], sizeof(double));

				}

				File.close();
				return true;
			}
			else {
				File.close();
				return false;
			}
		}
		bool Load(std::string path)
		{
			path += ".net";

			File.open(path, std::fstream::app | std::fstream::binary | std::fstream::in);

			if (File.is_open())
			{
				Clean();

				File.read((char*)&Layers, sizeof(int));

				Neurons = new int[Layers];

				for (int l = 0; l < Layers; l++)
					File.read((char*)&Neurons[l], sizeof(int));

				Weights = new double** [Layers - 1];

				for (int i = 0; i < Layers - 1; i++)
				{
					int first = Neurons[i];
					int second = Neurons[i + 1];

					Weights[i] = new double* [first];

					for (int f = 0; f < first; f++)
					{
						Weights[i][f] = new double[second];

						for (int s = 0; s < second; s++)
						{
							double w;
							File.read((char*)&w, sizeof(double));
							Weights[i][f][s] = w;
						}
					}
				}

				BNWeights = new double* [Layers - 1];

				for (int i = 0; i < Layers - 1; i++)
				{
					int second = Neurons[i + 1];

					BNWeights[i] = new double[second];

					for (int s = 0; s < second; s++)
						File.read((char*)&BNWeights[i][s], sizeof(double));


				}

				Outputs = new double* [Layers];
				Errors = new double* [Layers];

				for (int i = 0; i < Layers; i++) // ���� i �� ������� �� ���������� ����
				{
					Outputs[i] = new double[Neurons[i]]; // �������� ���������� �������� �������� ��� ������� ����
					Errors[i] = new double[Neurons[i]]; // �������� ���������� ������ ��� ������� ����
				}

				Clear = false;


				File.close();
				return true;
			}
			else {
				File.close();
				return false;
			}

		}

	private:
		int Layers;
		int* Neurons;
		double*** Weights;
		double** BNWeights;
		double** Outputs;
		double** Errors;
		double LearnQ = 0.5;
		bool Clear;
		std::fstream File;
		double (*AFunc)(double, bool);
		double Rand()
		{
			return (double)rand() / ((RAND_MAX) / 2) - 1;
		}
		void Clean()
		{
			if (!Clear)
			{
				delete[] Neurons; // �������� ������� � ����������� �������� �� ������ ����;

				for (int i = 0; i < Layers - 1; i++) // ���� i �� ������� �� ���������� ���������� ����� ������
				{
					int first = Neurons[i]; // ���������� �������� ������ �� ���������� ����, "i"
					int second = Neurons[i + 1]; // ���������� �������� ������� �� ���������� ����, "i + 1"

					/* �������� ������� ������ 2� ������� ������� ����� �������� ��������,
						���������� ����� ��� ���������� �������� ������ �� ���������� ���� "first" : */
					delete[] BNWeights[i];

					for (int f = 0; f < first; f++) // ���� f �� ������� �� ���������� ������� ������ �� ���������� ����

						/* �������� �������� ������ 3� ������� ������� �����,
						���������� ����� ��� ���������� �������� ������� �� ���������� ���� "second" : */
						delete[] Weights[i][f];

					/* �������� ������� ������ 3� ������� ������� �����,
						���������� ����� ��� ���������� �������� ������ �� ���������� ���� "first" : */
					delete[] Weights[i];
				}

				// �������� ������� ������ 3� ������� ����� �����, ���������� ����� ��� ���������� ����������� ����� ������ "Layers - 1" :
				delete[] Weights;

				/* �������� ������� ������ 2� ������� ����� ����� �������� ��������,
				���������� ����� ��� ���������� ����������� ����� ������ "Layers - 1" : */
				delete[] BNWeights;

				for (int i = 0; i < Layers; i++) // ���� i �� ������� �� ���������� ����
				{
					delete[] Outputs[i]; // �������� ���������� �������� �������� ��� ������� ����
					delete[] Errors[i]; // �������� ���������� ������ ��� ������� ����
				}

				// �������� 2� ������ �������� �������� �������� "Outputs" � ������ �������� "Errors" :
				delete[] Outputs;
				delete[] Errors;

				Clear = true;
			}
		}
	};

	namespace afuncs
	{
		double Sigmoid(double x, bool der)
		{
			if (der) return x * (1 - x);
			return 1 / (1 + exp(-x));
		}
		double DoubleLeakyReLu(double x, bool der)
		{
			if (!der)
			{
				if (x >= 0 && x <= 1)
					return x;
				if (x < 0)
					return 0.01 * x;
				if (x > 1)
					return 1 + 0.01 * (x - 1);
			}
			else
			{
				if (x >= 0 && x <= 1)
					return 1;
				if (x < 0)
					return 0.01;
				if (x > 1)
					return 0.01;
			}
		}
	}
}

