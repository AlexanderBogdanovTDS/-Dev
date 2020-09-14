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
			this->Layers = Layers; // Установление количества слоев
			this->Neurons = new int[Layers]; // Установление массива с количеством нейронов на каждом слое

			for (int l = 0; l < Layers; l++)
			{
				this->Neurons[l] = Neurons[l];
			}

			// Создание первого уровня 3х массива весов связи, количество ячеек это количество промежутков между слоями "Layers - 1" :
			Weights = new double** [Layers - 1];

			/* Создание первого уровня 2х массива весов связи нейронов смещения,
			количество ячеек это количество промежутков между слоями "Layers - 1" : */
			BNWeights = new double* [Layers - 1];


			for (int i = 0; i < Layers - 1; i++) // Цикл i от первого до последнего промежутка между слоями
			{
				int first = Neurons[i]; // Количество нейронов левого от промежутка слоя, "i"
				int second = Neurons[i + 1]; // Количество нейронов правого от промежутка слоя, "i + 1"

				/* Создание второго уровня 3х мерного массива весов,
					количество ячеек это количество нейронов левого от промежутка слоя "first" : */
				Weights[i] = new double* [first];

				/* Создание второго уровня 2х мерного массива весов нейронов смещения,
					количество ячеек это количество нейронов левого от промежутка слоя "first" : */
				BNWeights[i] = new double[second];

				for (int s = 0; s < second; s++) // Цикл s от первого до последнего нейрона правого от промежутка слоя
					// Присваивание случайного веса промежутка i, нейрона смещения и нейрона из правого слоя s :
					BNWeights[i][s] = Rand();


				for (int f = 0; f < first; f++) // Цикл f от первого до последнего нейрона левого от промежутка слоя
				{
					/* Создание третьего уровня 3х мерного массива весов,
					количество ячеек это количество нейронов правого от промежутка слоя "second" : */
					Weights[i][f] = new double[second];

					for (int s = 0; s < second; s++) // Цикл s от первого до последнего нейрона правого от промежутка слоя
						// Присваивание случайного веса промежутка i, нейрона из левого слоя f и нейрона из правого слоя s :
						Weights[i][f][s] = Rand();
				}
			}

			// Создание 2х мерных массивов выходных сигналов "Outputs" и ошибок нейронов "Errors" :
			Outputs = new double* [Layers];
			Errors = new double* [Layers];

			for (int i = 0; i < Layers; i++) // Цикл i от первого до последнего слоя
			{
				Outputs[i] = new double[Neurons[i]]; // Создание подмассива выходных сигналов для каждого слоя
				Errors[i] = new double[Neurons[i]]; // Создание подмассива ошибок для каждого слоя
			}

			Clear = false;

		}
		NNetwork(const NNetwork& other)
		{
			this->Layers = other.Layers; // Установление количества слоев
			this->Neurons = new int[Layers]; // Установление массива с количеством нейронов на каждом слое

			for (int l = 0; l < Layers; l++)
			{
				this->Neurons[l] = other.Neurons[l];
			}

			// Создание первого уровня 3х массива весов связи, количество ячеек это количество промежутков между слоями "Layers - 1" :
			Weights = new double** [Layers - 1];

			/* Создание первого уровня 2х массива весов связи нейронов смещения,
			количество ячеек это количество промежутков между слоями "Layers - 1" : */
			BNWeights = new double* [Layers - 1];


			for (int i = 0; i < Layers - 1; i++) // Цикл i от первого до последнего промежутка между слоями
			{
				int first = Neurons[i]; // Количество нейронов левого от промежутка слоя, "i"
				int second = Neurons[i + 1]; // Количество нейронов правого от промежутка слоя, "i + 1"

				/* Создание второго уровня 3х мерного массива весов,
					количество ячеек это количество нейронов левого от промежутка слоя "first" : */
				Weights[i] = new double* [first];

				/* Создание второго уровня 2х мерного массива весов нейронов смещения,
					количество ячеек это количество нейронов левого от промежутка слоя "first" : */
				BNWeights[i] = new double[second];

				for (int s = 0; s < second; s++) // Цикл s от первого до последнего нейрона правого от промежутка слоя
					// Присваивание веса промежутка i, нейрона смещения и нейрона из правого слоя s :
					BNWeights[i][s] = other.BNWeights[i][s];


				for (int f = 0; f < first; f++) // Цикл f от первого до последнего нейрона левого от промежутка слоя
				{
					/* Создание третьего уровня 3х мерного массива весов,
					количество ячеек это количество нейронов правого от промежутка слоя "second" : */
					Weights[i][f] = new double[second];

					for (int s = 0; s < second; s++) // Цикл s от первого до последнего нейрона правого от промежутка слоя
						// Присваивание веса промежутка i, нейрона из левого слоя f и нейрона из правого слоя s :
						Weights[i][f][s] = other.Weights[i][f][s];
				}
			}

			// Создание 2х мерных массивов выходных сигналов "Outputs" и ошибок нейронов "Errors" :
			Outputs = new double* [Layers];
			Errors = new double* [Layers];

			for (int i = 0; i < Layers; i++) // Цикл i от первого до последнего слоя
			{
				Outputs[i] = new double[Neurons[i]]; // Создание подмассива выходных сигналов для каждого слоя
				Errors[i] = new double[Neurons[i]]; // Создание подмассива ошибок для каждого слоя
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
			for (int n = 0; n < Neurons[0]; n++) // Цикл от первого до последнего нейрона первого слоя
				Outputs[0][n] = Inputs[n]; // Установление выходных значений нейронов первого слоя


			for (int i = 0; i < Layers - 1; i++) // Цикл i от первого до последнего промежутка между слоями
			{
				for (int s = 0; s < Neurons[i + 1]; s++) // Цикл s от первого до последнего нейрона правого от промежутка слоя
				{
					/* Переменная суммы произведений выходных сигналов от каждого
					предыдущего нейрона на веса связи с этими нейронами : */
					double Sum = 0;

					for (int f = 0; f < Neurons[i]; f++) // Цикл f от первого до последнего нейрона левого от промежутка слоя
					{
						/* Прибавление очередного произведения выходного сигнала
						предыдущего нейрона на вес связи с этим нейроном : */
						Sum += Outputs[i][f] * Weights[i][f][s];
					}

					/* Прибавление очередного произведения выходного сигнала
						нейрона смещения на вес связи с этим нейроном : */
					Sum += 1 * BNWeights[i][s];

					/* Пропуск суммы через функцию активации и присваивание
					результата в нейрон правого от промежутка слоя : */
					Outputs[i + 1][s] = AFunc(Sum, false);
				}
			}

		}
		void Errs(double TrueOutputs[])
		{
			for (int n = 0; n < Neurons[Layers - 1]; n++) // // Цикл от первого до последнего нейрона последнего слоя
				Errors[Layers - 1][n] = TrueOutputs[n] - Outputs[Layers - 1][n]; // Установление ошибок нейронов последнего слоя

			for (int i = Layers - 2; i > 0; i--) // Цикл i от последнего промежутка ко второму
			{
				for (int f = 0; f < Neurons[i]; f++) // Цикл f от первого до последнего нейрона левого от промежутка слоя
				{
					double Error = 0; // Переменная ошибок для каждого нейрона левого от промежутка слоя
					for (int s = 0; s < Neurons[i + 1]; s++) // Цикл s от первого до последнего нейрона правого от промежутка слоя
					{
						/* Прибавление к переменной ошибки очередное произведение ошибки нейрона правого
						от промежутка слоя на вес связи с этим нейроном : */
						Error += Errors[i + 1][s] * Weights[i][f][s];
					}
					Errors[i][f] = Error; // Присвяивание ошибки нейрону левого от промежутка слоя
				}
			}
		}
		void Learn()
		{
			for (int i = 0; i < Layers - 1; i++)  // Цикл i от первого до последнего промежутка между слоями
			{
				for (int s = 0; s < Neurons[i + 1]; s++) // Цикл s от первого до последнего нейрона правого от промежутка слоя
				{
					for (int f = 0; f < Neurons[i]; f++) // Цикл f от первого до последнего нейрона левого от промежутка слоя
					{
						/* Обновление весов связи правого от промежутка нейрона
							и левых от промежутка нейронов : */
						Weights[i][f][s] += LearnQ * Errors[i + 1][s] * AFunc(Outputs[i + 1][s], true) * Outputs[i][f];
					}

					/* Обновление весов связи правого от промежутка нейрона
							и нейрона смещения : */
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

				for (int i = 0; i < Layers; i++) // Цикл i от первого до последнего слоя
				{
					Outputs[i] = new double[Neurons[i]]; // Создание подмассива выходных сигналов для каждого слоя
					Errors[i] = new double[Neurons[i]]; // Создание подмассива ошибок для каждого слоя
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
				delete[] Neurons; // Удаление массива с количеством нейронов на каждом слое;

				for (int i = 0; i < Layers - 1; i++) // Цикл i от первого до последнего промежутка между слоями
				{
					int first = Neurons[i]; // Количество нейронов левого от промежутка слоя, "i"
					int second = Neurons[i + 1]; // Количество нейронов правого от промежутка слоя, "i + 1"

					/* Удаление второго уровня 2х мерного массива весов нейронов смещения,
						количество ячеек это количество нейронов левого от промежутка слоя "first" : */
					delete[] BNWeights[i];

					for (int f = 0; f < first; f++) // Цикл f от первого до последнего нейрона левого от промежутка слоя

						/* Удаление третьего уровня 3х мерного массива весов,
						количество ячеек это количество нейронов правого от промежутка слоя "second" : */
						delete[] Weights[i][f];

					/* Удаление второго уровня 3х мерного массива весов,
						количество ячеек это количество нейронов левого от промежутка слоя "first" : */
					delete[] Weights[i];
				}

				// Удаление первого уровня 3х массива весов связи, количество ячеек это количество промежутков между слоями "Layers - 1" :
				delete[] Weights;

				/* Удаление первого уровня 2х массива весов связи нейронов смещения,
				количество ячеек это количество промежутков между слоями "Layers - 1" : */
				delete[] BNWeights;

				for (int i = 0; i < Layers; i++) // Цикл i от первого до последнего слоя
				{
					delete[] Outputs[i]; // Удаление подмассива выходных сигналов для каждого слоя
					delete[] Errors[i]; // Удаление подмассива ошибок для каждого слоя
				}

				// Удаление 2х мерных массивов выходных сигналов "Outputs" и ошибок нейронов "Errors" :
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

