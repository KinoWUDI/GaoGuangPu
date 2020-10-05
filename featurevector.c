#include"featurevector.h"
#include <omp.h>
#include <time.h>

bool InitMatrix(Matrix* matrix, int row, int column)//��ʼ������
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
void PrintMatrix(Matrix* matrix)//��ӡ����
{
	int matrix_num = matrix->row * matrix->column;
	for (int i = 0; i < matrix_num; i++)
	{
		printf("%12.4g  ", matrix->data[i]);
		if ((i + 1) % (matrix->column) == 0)
			printf("\n");
	}
	printf("\n");

}
int GetMatrixSize(Matrix* matrix)//���ؾ����С
{
	return matrix->row * matrix->column;
}
void SetMatrixZeros(Matrix* matrix)//����
{
	int matrix_num = GetMatrixSize(matrix);
	for (int i = 0; i < matrix_num; i++)
		matrix->data[i] = 0;
}
/************************************************************************
�������ܣ��ж�һ�������Ƿ�Ϊ��
���룺һ������matrix
�����Ϊ����true������Ϊfalse
************************************************************************/
bool IsNullMatrix(Matrix* matrix)
{
	int matrix_num = GetMatrixSize(matrix);
	if ((matrix_num <= 0) || (matrix->data == NULL))
		return true;
	else
		return false;
}
/************************************************************************
�������ܣ��ͷŵ�һ������
���룺һ������matrix
�������
************************************************************************/
void DestroyMatrix(Matrix* matrix)
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
�������ܣ�����һ�������ģ
���룺һ������matrix
�����������
************************************************************************/
double MatrixNorm2(Matrix* matrix)
{
	double norm2_ans = 0.0;
	int matrix_num = GetMatrixSize(matrix);
	for (int i = 0; i < matrix_num; i++)
		norm2_ans += (matrix->data[i]) * (matrix->data[i]);
	norm2_ans = (double)sqrt(norm2_ans);
	return norm2_ans;
}

/************************************************************************
�������ܣ���һ��������
���룺��Ҫ���и��Ƶľ���matrix_A�����Ƶõ���һ������matrix_B
�������
************************************************************************/
void CopyMatrix(Matrix* matrix_A, Matrix* matrix_B)
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
�������ܣ���һ������A����QR�ֽ�
���룺��Ҫ�ֽ�ľ���A���ֽ�����������Q�������Ǿ���R
�������
************************************************************************/
void QR(Matrix* A, Matrix* Q, Matrix* R)
{
	Matrix col_A, col_Q;
	InitMatrix(&col_A, A->row, 1);
	SetMatrixZeros(&col_A); //������A��ÿһ��
	InitMatrix(&col_Q, A->row, 1);
	SetMatrixZeros(&col_Q);  //������Q��ÿһ��

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

	//ʩ����������
	for (int j = 0; j < A->column; j++)
	{
		for (int i = 0; i < A->column; i++)//��A�ĵ�j�д���col_A��
		{
			col_A.data[i] = A->data[i * A->column + j];
			col_Q.data[i] = A->data[i * A->column + j];
		}
		for (int k = 0; k < j; k++)//�����j����ǰ
		{
			R->data[k * R->column + j] = 0;
			for (int i1 = 0; i1 < col_A.row; i1++)
			{//R=Q'A(Q'��Q��ת��) ��Q�ĵ�k�к�A�ĵ�j�����ڻ�
				R->data[k * R->column + j] += col_A.data[i1] * Q->data[i1 * Q->column + k];//Q�ĵ�k��
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
			//��λ��Q
			Q->data[i3 * Q->column + j] = col_Q.data[i3] / temp;
		}
	}

	DestroyMatrix(&col_A);
	DestroyMatrix(&col_Q);
}
/************************************************************************
�������ܣ�������ֵ���򣬵�flag=1ʱ�������򣬵�flag=0������
���룺��Ҫ���������eValue�������ǽ����ѡ��flag
���������ɹ��򷵻�true�����򷵻�false
************************************************************************/
bool SortEigenValues(Matrix* eValue, int flag)
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
�������ܣ����������������C=A*B
���룺������������ľ���C����Ҫ���г˷��������������A��B
���������ɹ������true��ʧ����false
************************************************************************/
bool MatrixMulMatrix(Matrix* C, Matrix* A, Matrix* B)
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

	// ��GPU����
	for (int i = 0; i < A->row; i++)
	{
		for (int j = 0; j < B->column; j++)
		{
			for (int k = 0; k < A->column; k++)
				C->data[i * C->row + j] += A->data[i * A->row + k] * B->data[k * B->column + j];
		}
	}

	return true;
}

/************************************************************************
�������ܣ���֪һ�����������ֵ���Ӧ����������
���룺һ������A��������������������eigenVector����֪������ֵeigenValue
�������
************************************************************************/
void Eig(Matrix* A, Matrix* eigenVector, Matrix* eigenValue)
{
	int num = A->column;
	double eValue;
	Matrix temp;
	InitMatrix(&temp, A->row, A->column);

	int numProcs = omp_get_num_procs();
	// printf("%d\n", numProcs);// ����Ϊ12

	//CopyMatrix(A, &temp);

	int count = 0;
// #pragma omp parallel for num_threads(numProcs)
#pragma omp parallel for default(shared) private(eValue) num_threads(numProcs)
	for (count = 0; count < num; count++)
	{
		eValue = eigenValue->data[count];//��ǰ������ֵ
		CopyMatrix(A, &temp);//ÿ�ζ�Ҫ���¸��ƣ�������ƻ�ԭ����
		for (int i = 0; i < temp.row; i++)
		{
			temp.data[i * temp.column + i] -= eValue;
		}
		//��temp��Ϊ�����;���(��һ��)�Խ���ֵΪһ

		//printf(numProcs);
		//printf("\n");
		//int abc = 2 * numProcs - 1;
		//printf(abc);
		//printf("\n");

		//int i;
// #pragma omp parallel for num_threads(numProcs)
		for (int i = 0; i < temp.row - 1; i++)
		{
			double coe = temp.data[i * temp.column + i];
			for (int j = i; j < temp.column; j++)
			{
				temp.data[i * temp.column + j] /= coe;//�öԽ���ֵΪһ
			}
			for (int i1 = i + 1; i1 < temp.row; i1++)
			{
				coe = temp.data[i1 * temp.column + i];

				int j1;			
				for (j1 = i; j1 < temp.column; j1++)
				{
					temp.data[i1 * temp.column + j1] -= coe * temp.data[i * temp.column + j1];
				}
			}
		}
		//�����һ��Ϊ1
		double sum1 = eigenVector->data[(eigenVector->row - 1) * eigenVector->column + count] = 1;

		//int i2;
// #pragma omp parallel for num_threads(numProcs)
		for (int i2 = temp.row - 2; i2 >= 0; i2--)
		{
			double sum2 = 0;

			int j2;
			for (j2 = i2 + 1; j2 < temp.column; j2++)
			{
				sum2 += temp.data[i2 * temp.column + j2] * eigenVector->data[j2 * eigenVector->column + count];
			}
			sum2 = -sum2 / temp.data[i2 * temp.column + i2];
			sum1 += sum2 * sum2;
			eigenVector->data[i2 * eigenVector->column + count] = sum2;
		}
		sum1 = sqrt(sum1);//��ǰ�е�ģ
		for (int i = 0; i < eigenVector->row; i++)
		{
			//��λ��
			eigenVector->data[i * eigenVector->column + count] /= sum1;
		}
	}
	DestroyMatrix(&temp);
}

void featVec(CovData* ImageCov, Matrix* K)
{
	// ��ʱ��ʼ
	clock_t t_start, t_stop;
	double duration;
	t_start = clock();
		
	const unsigned NUM = 10; //�����������������ݸ�׼ȷ
	Matrix mymatrix, temp, temp_Q, temp_R, eValue;
	int row = ImageCov->row;
	int col = ImageCov->column;
	//-------------------
	//��ʼ��0����
	InitMatrix(&mymatrix, row, col);
	InitMatrix(&temp, row, col);
	SetMatrixZeros(&temp);
	SetMatrixZeros(&mymatrix);
	//-------------------
	int i, k = 0;
	for (i = 0; i < row * col; i++)
	{
		mymatrix.data[k] = ImageCov->data[k++];
	}
	CopyMatrix(&mymatrix, &temp);
	InitMatrix(&temp_Q, mymatrix.row, mymatrix.column);
	InitMatrix(&temp_R, mymatrix.row, mymatrix.column);
	InitMatrix(&eValue, mymatrix.row, 1);

	for (k = 0; k < NUM; ++k)//ʹ��QR�ֽ����������ֵ
	{
		QR(&temp, &temp_Q, &temp_R);
		MatrixMulMatrix(&temp, &temp_R, &temp_Q);//R*Q
	}

	for (k = 0; k < temp.column; ++k)//��ȡ����ֵ����֮�洢��eValue
	{
		eValue.data[k] = temp.data[k * temp.column + k];
	}

	SortEigenValues(&eValue, 0);//������ֵ����1Ϊ����0Ϊ����

	Eig(&mymatrix, &temp_Q, &eValue);//������������		4800ms 20.09.11   1489ms 09.19


	//printf("������������ : %dms\n", GetTickCount() - tick_cov);
	//���ض���temp_Q��eValue
	//PrintMatrix(&eValue);//����ֵ
	//PrintMatrix(&temp_Q);//��������

	K->data = (VBYTE*)malloc(sizeof(VBYTE) * row * col);
	K->column = col;
	K->row = row;
	k = 0;
	for (i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
			K->data[j * col + i] = temp_Q.data[k++];
	}
	DestroyMatrix(&eValue);
	DestroyMatrix(&mymatrix);
	DestroyMatrix(&temp);
	DestroyMatrix(&temp_Q);
	DestroyMatrix(&temp_R);
	//printf("(featurevector.c)���������������\n");

	// ��ʱ����
	t_stop = clock();
	duration = (double)(t_stop - t_start) / CLK_TCK;
	printf("def featVec������ֵ/����. ʱ�䣺%fs\n", duration);

}