int Compute_Score(int x, int y, BYTE **Rimg, BYTE **Limg, int w, int h, int Win, int dx)
{
	int m, n;  //Win x Win ������ �� �ȼ�index(m:x��index, n:y��index)
	int sum = 0; //�¿쿵�� ������ �� �ȼ� �� ���� ���� ������ ����
	int half = (int)(Win / 2);  //������ �߽��ȼ��� ���� �׵θ��ȼ����� �Ÿ�(=������� ���� �����׵θ� �β�)

	//window �� �¿� ���� �ȼ��� ���� ������ ��� ���ϱ�
	for (n = 0; n < Win; n++) {
		for (m = 0; m < Win; m++) {
			sum += abs(Rimg[y - half + n][x - half + m] - Limg[y - half + n][x + dx - half + m]);
		}
	}

	return sum;
}

BYTE Compute_Disparity(int x, int y, BYTE **Rimg, BYTE **Limg, int w, int h, int Win, int Dist)
{
	int dx, Score, min; //dx:���������ȼ��κ��� ���������ȼ����� ������ �Ÿ�.
	BYTE disparity; //���� disparity ���� ���� ����

	//0~Dist��ŭ ������ �ȼ����� score�� �� ���� ���� ���� indexã��. diparity = index. 
	min = 9999;
	for (dx = 0; dx < Dist; dx++) {
		Score = Compute_Score(x, y, Rimg, Limg, w, h, Win, dx);
		if (Score < min) {
			min = Score;
			disparity = dx;
		}
	}

	return disparity;
}


void CHomework::OnClickedStereo()
{
	// ���� ���� �����ϱ�
	int x0, y0, w, h, x, y;
	//Result:normalize�� disparity map, Result1: 7*7 normalize�� disparity map, Result2: 21*21 normalize�� disparity map
	BYTE **Limg, **Rimg, **Result, **Result1, **Result2;

	Display_BMP_File("c:\\Rimg2.bmp", 0, 0); //Rimg1 BITMAP������ �о (0,0)��ġ�� ���÷���
	GetCurrentImageInfo(&w, &h, &x0, &y0); // �Է¿��� ���� �б�
	Rimg = cmatrix(h, w); // BYTE** �ڷ��� �Ҵ��ϱ�
	GetCurrentImageGray(Rimg); // ȸ���� ���� �б�

	Display_BMP_File("c:\\Limg2.bmp", w, 0); //Limg1 BITMAP������ �о (w,0)��ġ�� ���÷���	
	GetCurrentImageInfo(&w, &h, &x0, &y0); // �Է¿��� ���� �б�
	Limg = cmatrix(h, w); // BYTE** �ڷ��� �Ҵ��ϱ�
	GetCurrentImageGray(Limg); // ȸ���� ���� �б�

	//BYTE** �ڷ��� �Ҵ��ϱ�
	Result1 = cmatrix(h, w);
	Result2 = cmatrix(h, w);
	Result = cmatrix(h, w);

	//ó���� Disparity map�� ��� �ȼ��� 0���� �ʱ�ȭ --> ������� ���� �ȼ��� 0���� ������ֱ� ����.
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			Result1[y][x] = 0;
			Result2[y][x] = 0;
		}
	}

	//7*7 window�� Disparity ����� ���� Result1�� �����ϱ�.
	for (y = 3; y < h - 3; y++) {
		for (x = 3; x < w - 3 - 30; x++) {
			Result1[y][x] = Compute_Disparity(x, y, Rimg, Limg, w, h, 7, 30);
		}
	}

	//21*21 window�� Disparity ����� ���� Result2�� �����ϱ�.
	for (y = 10; y < h - 10; y++) {
		for (x = 10; x < w - 10 - 30; x++) {
			Result2[y][x] = Compute_Disparity(x, y, Rimg, Limg, w, h, 21, 30);
		}
	}

	//7*7 ����� ȭ�鿡 ����ϱ� ���� normalize�ϱ�. BYTE to BYTE.
	CNormalize2D(Result1, Result, w, h);

	//7*7 Disparity map�� ���÷���
	DisplayCimage2D(Result, w, h, x0 - w, y0 + h, true);

	//21*21 ����� ȭ�鿡 ����ϱ� ���� normalize�ϱ�. BYTE to BYTE.
	CNormalize2D(Result2, Result, w, h);

	//21*21 Disparity map�� ���÷���
	DisplayCimage2D(Result, w, h, x0, y0 + h, true);

	//BYTE** �ڷ��� �Ҵ� �����ϱ�
	free_cmatrix(Limg, h, w);
	free_cmatrix(Rimg, h, w);
	free_cmatrix(Result1, h, w);
	free_cmatrix(Result2, h, w);
	free_cmatrix(Result, h, w);
}