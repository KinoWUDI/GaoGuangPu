#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>

typedef struct //矩阵结构体
{
	int row;
	int column;
	double *data;
}Matrix;

bool InitMatrix(Matrix *matrix, int row, int column)//初始化矩阵
{
	int matrix_size = row * column * sizeof(double);
	if (matrix_size <= 0)
		return false;
	matrix->data = (double*)malloc(matrix_size);
	if (matrix->data)
	{
		matrix->row = row;
		matrix->column = column;
		return true;
	}
	else
	{
		matrix->row = 0;
		matrix->column = 0;
		return false;
	}
}

void PrintMatrix(Matrix *matrix)//打印矩阵
{
	int matrix_num = matrix->row*matrix->column;
	for (int i = 0; i < matrix_num; i++)
	{
		printf("%12.4g  ", matrix->data[i]);
		if ((i + 1) % (matrix->column) == 0)
			printf("\n");
	}
	printf("\n");

}

int GetMatrixSize(Matrix *matrix)//返回矩阵大小
{
	return matrix->row*matrix->column;
}
void SetMatrixZeros(Matrix *matrix)//清零
{
	int matrix_num = GetMatrixSize(matrix);
	for (int i = 0; i < matrix_num; i++)
		matrix->data[i] = 0;
}
/************************************************************************
函数功能：判断一个矩阵是否为空
输入：一个矩阵matrix
输出：为空则true，否则为false
************************************************************************/
bool IsNullMatrix(Matrix *matrix)
{
	int matrix_num = GetMatrixSize(matrix);
	if ((matrix_num <= 0) || (matrix->data == NULL))
		return true;
	else
		return false;
}

/************************************************************************
函数功能：释放掉一个矩阵
输入：一个矩阵matrix
输出：无
************************************************************************/
void DestroyMatrix(Matrix *matrix)
{
	if (!IsNullMatrix(matrix))
	{
		matrix->data = NULL;
		matrix->row = 0;
		matrix->column = 0;
		free(matrix->data);
	}
}
/************************************************************************
函数功能：计算一个矩阵的2范数，即求模
输入：一个矩阵matrix
输出：所求的范数结果norm2_ans
************************************************************************/
double MatrixNorm2(Matrix *matrix)
{
	double norm2_ans = 0.0;
	int matrix_num = GetMatrixSize(matrix);
	for (int i = 0; i < matrix_num; i++)
		norm2_ans += (matrix->data[i]) * (matrix->data[i]);
	norm2_ans = (double)sqrt(norm2_ans);
	return norm2_ans;
}

/************************************************************************
函数功能：把一个矩阵复制
输入：需要进行复制的矩阵matrix_A，复制得到的一个矩阵matrix_B
输出：无
************************************************************************/
void CopyMatrix(Matrix *matrix_A, Matrix *matrix_B)
{
	if (matrix_B->row != matrix_A->row)
		matrix_B->row = matrix_A->row;
	if (matrix_B->column != matrix_A->column)
		matrix_B->column = matrix_A->column;
	int size_A = GetMatrixSize(matrix_A);
	for (int i = 0; i < size_A; i++)
		matrix_B->data[i] = matrix_A->data[i];
}

/************************************************************************
函数功能：对一个方阵A进行QR分解
输入：需要分解的矩阵A、分解后的正交矩阵Q和上三角矩阵R
输出：无
************************************************************************/
void QR(Matrix *A, Matrix *Q, Matrix *R)
{
	Matrix col_A, col_Q;
	InitMatrix(&col_A, A->row, 1);
	SetMatrixZeros(&col_A); //用来存A的每一列
	InitMatrix(&col_Q, A->row, 1);
	SetMatrixZeros(&col_Q);  //用来存Q的每一列

	if (A->row != A->column)
		printf("A is not a square matrix!");

	int A_size = GetMatrixSize(A);
	int Q_size = GetMatrixSize(Q);
	int R_size = GetMatrixSize(R);

	if (Q_size != A_size)
	{
		DestroyMatrix(Q);
		InitMatrix(Q, A->row, A->column);
		SetMatrixZeros(Q);
	}
	else
	{
		Q->row = A->row;
		Q->column = A->column;
		SetMatrixZeros(Q);
	}

	if (R_size != A_size)
	{
		DestroyMatrix(R);
		InitMatrix(R, A->row, A->column);
		SetMatrixZeros(R);
	}
	else
	{
		R->row = A->row;
		R->column = R->column;
		SetMatrixZeros(R);
	}

	//施密特正交化
	for (int j = 0; j < A->column; j++)
	{
		for (int i = 0; i < A->column; i++)//把A的第j列存入col_A中
		{
			col_A.data[i] = A->data[i * A->column + j];
			col_Q.data[i] = A->data[i * A->column + j];
		}
		for (int k = 0; k < j; k++)//计算第j列以前
		{
			R->data[k * R->column + j] = 0;
			for (int i1 = 0; i1 < col_A.row; i1++)
			{//R=Q'A(Q'即Q的转置) 即Q的第k列和A的第j列做内积
				R->data[k * R->column + j] += col_A.data[i1] * Q->data[i1 * Q->column + k];//Q的第k列
			}
			for (int i2 = 0; i2 < A->column; i2++)
			{
				col_Q.data[i2] -= R->data[k * R->column + j] * Q->data[i2 * Q->column + k];
			}
		}

		double temp = MatrixNorm2(&col_Q);
		R->data[j * R->column + j] = temp;
		for (int i3 = 0; i3 < Q->column; i3++)
		{
			//单位化Q
			Q->data[i3 * Q->column + j] = col_Q.data[i3] / temp;
		}
	}

	DestroyMatrix(&col_A);
	DestroyMatrix(&col_Q);
}
/************************************************************************
函数功能：给特征值排序，当flag=1时，则升序，当flag=0，则降序
输入：需要排序的序列eValue，升序还是降序的选择flag
输出：排序成功则返回true，否则返回false
************************************************************************/
bool SortEigenValues(Matrix *eValue, int flag)
{
	int size = GetMatrixSize(eValue);

	for (int i = 0; i < size - 1; i++)
	{
		int k = i;
		for (int j = i + 1; j < size; j++)
		{
			if (flag == 1)
			{
				if (eValue->data[k] > eValue->data[j])
				{
					k = j;
				}
			}
			else if (flag == 0)
			{
				if (eValue->data[k] < eValue->data[j])
				{
					k = j;
				}
			}
			else
				return false;
		}
		if (k != i)
		{
			double temp;
			temp = eValue->data[i];
			eValue->data[i] = eValue->data[k];
			eValue->data[k] = temp;
		}
	}
	return true;
}

/************************************************************************
函数功能：计算两个矩阵相乘C=A*B
输入：用来存计算结果的矩阵C、需要进行乘法计算的两个矩阵A和B
输出：计算成功则输出true，失败则false
************************************************************************/
bool MatrixMulMatrix(Matrix *C, Matrix *A, Matrix *B)
{
	if ((IsNullMatrix(A)) || (IsNullMatrix(B)))
		return false;

	int A_col = A->column;
	int B_row = B->row;
	InitMatrix(C, A->row, B->column);
	SetMatrixZeros(C);

	if (A_col != B_row)
	{
		printf("A_col!=B_row!");
		return false;
	}

	for (int i = 0; i < A->row; i++)
	{
		for (int j = 0; j < B->column; j++)
		{
			for (int k = 0; k < A->column; k++)
				C->data[i*C->row + j] += A->data[i*A->row + k] * B->data[k*B->column + j];
		}
	}

	return true;
}

/************************************************************************
函数功能：已知一个矩阵的特征值求对应的特征向量
输入：一个矩阵A、用来存结果的特征向量eigenVector、已知的特征值eigenValue
输出：无
************************************************************************/
void Eig(Matrix *A, Matrix *eigenVector, Matrix *eigenValue)
{
	int num = A->column;
	double eValue;
	Matrix temp;
	InitMatrix(&temp, A->row, A->column);
	//CopyMatrix(A, &temp);
	for (int count = 0; count < num; count++)
	{
		eValue = eigenValue->data[count];//当前的特征值
		CopyMatrix(A, &temp);//这个每次都要重新复制，因为后面会破坏原矩阵(刚开始没注意到这个找bug找了好久。。)
		for (int i = 0; i < temp.row; i++)
		{
			temp.data[i * temp.column + i] -= eValue;
		}
		//将temp化为阶梯型矩阵(归一性)对角线值为一
		for (int i = 0; i < temp.row - 1; i++)
		{
			double coe = temp.data[i * temp.column + i];
			for (int j = i; j < temp.column; j++)
			{
				temp.data[i * temp.column + j] /= coe;//让对角线值为一
			}
			for (int i1 = i + 1; i1 < temp.row; i1++)
			{
				coe = temp.data[i1 * temp.column + i];
				for (int j1 = i; j1 < temp.column; j1++)
				{
					temp.data[i1 * temp.column + j1] -= coe * temp.data[i * temp.column + j1];
				}
			}
		}
		//让最后一行为1
		double sum1 = eigenVector->data[(eigenVector->row - 1) * eigenVector->column + count] = 1;
		for (int i2 = temp.row - 2; i2 >= 0; i2--)
		{
			double sum2 = 0;
			for (int j2 = i2 + 1; j2 < temp.column; j2++)
			{
				sum2 += temp.data[i2 * temp.column + j2] * eigenVector->data[j2 * eigenVector->column + count];
			}
			sum2 = -sum2 / temp.data[i2 * temp.column + i2];
			sum1 += sum2 * sum2;
			eigenVector->data[i2 * eigenVector->column + count] = sum2;
		}
		sum1 = sqrt(sum1);//当前列的模
		for (int i = 0; i < eigenVector->row; i++)
		{
			//单位化
			eigenVector->data[i * eigenVector->column + count] /= sum1;
		}
	}
	DestroyMatrix(&temp);
}

int main()
{
	const unsigned NUM = 50; //最大迭代次数，让数据更准确
	Matrix mymatrix, temp, temp_Q, temp_R, eValue;
	int row, col;
	while (1)//VS里面运行写scanf会报错，改成scanf_s就可以了
	{
		printf("请输入要进行计算的矩阵行与列(以逗号隔开)：");
		scanf_s("%d,%d", &row, &col);

		InitMatrix(&mymatrix, row, col);
		InitMatrix(&temp, row, col);
		SetMatrixZeros(&temp);
		SetMatrixZeros(&mymatrix);

		int num = row * col;
		printf("按照以行的输入顺序依次输入矩阵内的元素，一共输入%d个元素：", num);
		int data;
		for (int i = 0; i < num; i++)
		{
			scanf_s("%d", &data);
			mymatrix.data[i] = data;
		}
		printf("输入矩阵如下：\n");
		PrintMatrix(&mymatrix);

		CopyMatrix(&mymatrix, &temp);

		InitMatrix(&temp_Q, mymatrix.row, mymatrix.column);
		InitMatrix(&temp_R, mymatrix.row, mymatrix.column);
		InitMatrix(&eValue, mymatrix.row, 1);

		//使用QR分解求矩阵特征值
		for (int k = 0; k < NUM; ++k)
		{
			QR(&temp, &temp_Q, &temp_R);
			MatrixMulMatrix(&temp, &temp_R, &temp_Q);//R*Q
		}

		/*printf("Q&R:\n");
		PrintMatrix(&temp_Q);
		PrintMatrix(&temp_R);*/

		//获取特征值，将之存储于eValue
		for (int k = 0; k < temp.column; ++k)
		{
			eValue.data[k] = temp.data[k * temp.column + k];
		}

		SortEigenValues(&eValue, 1);//给特征值排序，1为升序，0为降序
		printf("特征值：\n");
		PrintMatrix(&eValue);

		Eig(&mymatrix, &temp_Q, &eValue);
		printf("特征向量：\n");
		PrintMatrix(&temp_Q);

		DestroyMatrix(&eValue);
		DestroyMatrix(&mymatrix);
		DestroyMatrix(&temp);
		DestroyMatrix(&temp_Q);
		DestroyMatrix(&temp_R);
	}
	return 0;
}

